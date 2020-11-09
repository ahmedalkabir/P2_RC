#ifndef P2_RC_H_
#define P2_RC_H_

// for now i'll develop for
// arduino uno and mega

// preprocessors
#include <avr/io.h>
#include <stdbool.h>

#define BAUD(X) (F_CPU / 4 / X - 1) / 2

#define P2_RC_YIELD() asm("nop")

// internal variables
typedef struct {
    uint16_t roll;
    uint16_t pitch;
    uint16_t throttle;
    uint16_t yaw;
    uint16_t left_dial;
    uint16_t aux_1;
    uint16_t aux_2;
} P2_RC;

static uint8_t _head_sbus = 0x0F;
static uint8_t _footer_sbus = 0x00;
static uint8_t _sbus2Footer = 0x04;
static uint8_t _sbus2Mask = 0x0F;

static uint8_t _sbus_payload[24] = {0};
static uint8_t _incr = 0;
static char _prev_byte = 0;
static uint16_t _channels[7] = {0};
static uint8_t _length = 0;

static uint8_t _buffer[30];
static uint8_t _pos = 0;
// flags 
static bool _start_fill = false;
static bool _available = false;

// internal functions
static void parse(const uint8_t *__payload);

// external variables and functions

extern P2_RC channels;

#ifdef __cplusplus
extern "C" {
#endif

void p2_rc_init();
void p2_rc_init1();
void p2_rc_init2();
void p2_rc_init3();
void p2_rc_receiver_cn(P2_RC *channels);

#ifdef __cplusplus
}
#endif



#endif