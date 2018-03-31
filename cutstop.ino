#include "Configuration.h"
#include "State.h"
#include "Serial.h"
#include "Utils.h"

const uint8_t start_marker = 0x3C;
const uint8_t end_marker = 0x3E;


void setup() {
  pinMode(ENC_A, INPUT);
  digitalWrite(ENC_A, HIGH);
  pinMode(ENC_B, INPUT);
  digitalWrite(ENC_B, HIGH);
  pinMode(ENC_PUSH, INPUT);
  digitalWrite(ENC_PUSH, HIGH);

  setup_display();
  setup_keypad();
  
  Serial.begin (115200);
  Serial.println("Start");

}

void loop() {

  serial_check(rx_buffer_S1);

  // update state
  CS_position += (read_encoder() * CS_multiplier);
  CS_r = CS_position / CS_scale_factor;

  switch (CS_mode) {
    case 'j':
      jog_mode();
    break;

//    case 'm':
//      // mdi_mode();
//    break;
  }

  update_display();

}





