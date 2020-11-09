// initilaize the uart communication 
// for phantom remote

#include <p2_rc.h>

#define AUX_MIN 511
#define AUX_MID 1024
#define AUX_MAX 1541

#define MIN_OUT 364
#define MID_OUT 1024
#define MAX_OUT 1684

#define MANU_UP() channels.pitch == MAX_OUT 
#define MAIN_IDLE() channels.pitch == MID_OUT 
#define MANU_DOWN() channels.pitch == MIN_OUT 

#define OPTIONS_ENABLED() channels.aux_1 == AUX_MIN && channels.throttle == MIN_OUT
#define WAIT_FOR_CHANGE() while(OPTIONS_ENABLED() && !(MANU_UP() || MANU_DOWN())) P2_RC_YIELD()
bool manu_flag = false;

uint8_t manu_pos = 0;

void p2_rc_receiver_cn(P2_RC *_channels){
   if(OPTIONS_ENABLED()){
      manu_flag = true; 
   }
}

void calibrate_motors(){
   bool finished = false;
   Serial.println("CALIBRATING MOTORS");
   while(!finished){
      Serial.println("THROTTLE TO TOP");
      while(channels.throttle != MAX_OUT) P2_RC_YIELD();
      Serial.println("CONNECT BATTERY");
      // calibrate
      finished = true;
//      WAIT_FOR_CHANGE();
   }
}

void manu_list(uint8_t pos){
    switch(pos){
      case 1:
        Serial.println("1");
        break;
      case 2:
        calibrate_motors();
        break;
      case 3:
        Serial.println("3");
        break;
      case 4:
        Serial.println("4");
        break;      
    }  
}

void setup() {
  // put your setup code here, to run once:

  // enable uart
  p2_rc_init2();
  Serial.begin(9600);
}

void loop() {
  while(OPTIONS_ENABLED()){ 
    Serial.println("CONFIG MANUE");

    // wait for up and down
    WAIT_FOR_CHANGE();
    if(MANU_UP()){
      manu_pos += 1;
      manu_list(manu_pos);
      while(MANU_UP())
        P2_RC_YIELD();
    } else if(MANU_DOWN()){
      manu_pos -= 1;     
      manu_list(manu_pos);
      while(MANU_DOWN())
        P2_RC_YIELD();
    }
  }
}
