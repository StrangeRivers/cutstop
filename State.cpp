#include "State.h"
#include "Serial.h"
#include "Utils.h"

// Mecaduino state
float MS_u = 0.0;            // control effort
float MS_r = 0.0;            // current setpoint
float MS_y = 0.0;            // measured angle
float MS_v = 0.0;            // estimated velocity (velocity loop)
float MS_e = 0.0;            // e = r-y (error)
float MS_zero_angle = 0.0;   // calibrated zero angle
char MS_mode = 0.0;          // device state

// Controller state
float CS_position = 0.0;     // trolley position (position + offset)
float CS_offset = 0.0;       // calibrated distance from blade
float CS_r = 0.0;                // requested setpoint
float CS_multiplier = 10;   // jog wheel multiplier
char  CS_mode = 'j';              // device state j = jog, m = mdi
float CS_scale_factor = 0.1666666667; // linear travel/degree (belt pitch * teeth) / 360 

// Display state
int8_t cursor[2];

float set_point_last = 0.0;

void jog_mode() {

  update_multiplier();

  if (CS_r != set_point_last) {
    send_data('r', CS_r, 1);
  }

  set_point_last = CS_r;

//  switch (read_keypad()){
//    case 9 :
//      CS_mode = 'm';
//    break;
//  }
  
}

void mdi_mode() {

  update_multiplier();

  switch (read_keypad()){
    case 13 :
      send_data('r', CS_r, 1);
    break;
    
    case 9 :
      CS_mode = 'j';
    break;
  }

}

