#include <p2_rc.h>
#include <avr/interrupt.h>

P2_RC channels;

// internal functions
void parse(const uint8_t *__payload)
{
    _channels[0] = (uint16_t)((__payload[0] | __payload[1] << 8) & 0x07FF);
    _channels[1] = (uint16_t)((__payload[1] >> 3 | __payload[2] << 5) & 0x07FF);
    _channels[2] = (uint16_t)((__payload[2] >> 6 | __payload[3] << 2 | __payload[4] << 10) & 0x07FF);
    _channels[3] = (uint16_t)((__payload[4] >> 1 | __payload[5] << 7) & 0x07FF);
    _channels[4] = (uint16_t)((__payload[5] >> 4 | __payload[6] << 4) & 0x07FF);
    _channels[5] = (uint16_t)((__payload[6] >> 7 | __payload[7] << 1 | __payload[8] << 9) & 0x07FF);
    _channels[6] = (uint16_t)((__payload[8] >> 2 | __payload[9] << 6) & 0x07FF);

    if (_sbus_payload[22] & 0x04 || _sbus_payload[22] & 0x08)
    {
    }
    else if (_channels[0] > 363 && _channels[1] > 363 && _channels[2] > 363 && _channels[3] > 363 && _channels[4] > 363 && _channels[5] > 363 && _channels[6] > 363)
    {
        channels.roll = _channels[0];
        channels.pitch = _channels[1];
        channels.throttle = _channels[2];
        channels.yaw = _channels[3];
        channels.left_dial = _channels[4];
        channels.aux_1 = _channels[5];
        channels.aux_2 = _channels[6];

        p2_rc_receiver_cn(&channels);
    }
}


// for arduino uno and arduino mega
#if defined(__AVR_ATmega328P) || defined(__AVR_ATmega328) || defined(__AVR_ATmega2560__)
void p2_rc_init()
{
    UBRR0H = BAUD(100000) >> 8;
    UBRR0L = BAUD(100000);

    // enable u2x mode
    UCSR0A |= (1 << U2X0);

    // enable rx complete interrupt
    UCSR0B = (1 << RXEN0) | (1 << RXCIE0);

    // 8 bits, 2 stop bits, even party
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00) | (1 << UPM01) | (1 << USBS0);
}


ISR(USART_RX_vect)
{
    // read once and process the char
    char _curByte = UDR0;
    // fint the header
    if (_incr == 0)
    {
        if ((_curByte == _head_sbus) && ((_prev_byte == _footer_sbus) || ((_prev_byte & _sbus2Mask) == _sbus2Footer)))
        {
            _incr++;
        }
        else
        {
            _incr = 0;
        }
    }
    else
    {
        // strip off the data
        if ((_incr - 1) < 24)
        {
            _sbus_payload[_incr - 1] = _curByte;
            _incr++;
        }

        // check the end byte
        if ((_incr - 1) == 24)
        {
            if ((_prev_byte == _footer_sbus) || ((_prev_byte & _sbus2Mask) == _sbus2Footer))
            {
                _incr = 0;
                _available = true;
                parse(_sbus_payload);
            }
            else
            {
                _incr = 0;
                _available = false;
            }
        }
    }
    _prev_byte = _curByte;
}

#endif


// special for arduino mega2560
#if defined(__AVR_ATmega2560__) 
void p2_rc_init1()
{
}

void p2_rc_init2()
{
    UBRR2H = BAUD(100000) >> 8;
    UBRR2L = BAUD(100000);

    // enable u2x mode
    UCSR2A |= (1 << U2X2);

    // enable rx complete interrupt
    UCSR2B = (1 << RXEN2) | (1 << RXCIE2);

    // 8 bits, 2 stop bits, even party
    UCSR2C = (1 << UCSZ21) | (1 << UCSZ20) | (1 << UPM21) | (1 << USBS2);
}

void p2_rc_init3()
{
}

ISR(USART2_RX_vect)
{
    // read once and process the char
    char _curByte = UDR2;
    // fint the header
    if (_incr == 0)
    {
        if ((_curByte == _head_sbus) && ((_prev_byte == _footer_sbus) || ((_prev_byte & _sbus2Mask) == _sbus2Footer)))
        {
            _incr++;
        }
        else
        {
            _incr = 0;
        }
    }
    else
    {
        // strip off the data
        if ((_incr - 1) < 24)
        {
            _sbus_payload[_incr - 1] = _curByte;
            _incr++;
        }

        // check the end byte
        if ((_incr - 1) == 24)
        {
            if ((_prev_byte == _footer_sbus) || ((_prev_byte & _sbus2Mask) == _sbus2Footer))
            {
                _incr = 0;
                _available = true;
                parse(_sbus_payload);
            }
            else
            {
                _incr = 0;
                _available = false;
            }
        }
    }
    _prev_byte = _curByte;
}

#endif