#include "Configuration.h"
#include "State.h"

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <stdint.h>
#include "arduino.h"

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 4 line display

int8_t read_encoder() {
  static int8_t enc_states_LUT[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
  static uint8_t AB_last = 0;
  static uint8_t counter = 0;

  AB_last <<= 2;                   //remember previous state
  AB_last |= ( ENC_PORT & 0x03 );  //add current state

  if (enc_states_LUT[( AB_last & 0x0f )] != 0) {
    counter++;
    if (counter > 1) {
      counter = 0;
      return enc_states_LUT[( AB_last & 0x0f )];
    }
  }
  return 0;
}

bool update_multiplier() {
  bool button_state = digitalRead(ENC_PUSH);
  static bool last_button_state = false;
  static uint8_t counter = 0;
  static uint8_t transition = 1;
  
  const float multipliers[] = {0.5, 1, 10, 100};

  if (button_state != last_button_state ) {
    transition++;
  }

  if (transition > 1) {
    counter++;
    transition = 0;
    if (counter > 3) {
      counter = 0;
    }
    CS_multiplier = multipliers[counter];
  }
  last_button_state = button_state;

  return true;
  
}

void setup_display() {

  lcd.init(); 
  lcd.backlight();  //start the backlight 
  
}

void update_display() {
  static char last_mode;
    
  switch (CS_mode) {
  
    case 'j':
      if (last_mode != CS_mode) {
          lcd.setCursor(0, 0);            // go to the top left corner
          lcd.print("Jog Mode");
          cursor[0] = 0;
          cursor[1] = 1;  
      }
      last_mode = CS_mode;
      
      lcd.setCursor(cursor[0], cursor[1]);       
      lcd.print(CS_position);
      lcd.print(" mm");
      cursor[1] = 2;
      lcd.setCursor(cursor[0], cursor[1]);       
      lcd.print("Multiplier: ");
      lcd.print(CS_multiplier);
      cursor[1] = 1;
      
    break;

    case 'm':
      if (last_mode != CS_mode) {
          lcd.setCursor(0, 0);            // go to the top left corner
          lcd.print("MDI Mode");
          cursor[0] = 0;
          cursor[1] = 1;  
      }
      last_mode = CS_mode;
      
      lcd.setCursor(cursor[0], cursor[1]);       
      lcd.print(CS_position);
      lcd.print(" mm");
      cursor[1] = 2;
      lcd.setCursor(cursor[0], cursor[1]);       
      lcd.print("Multiplier: ");
      lcd.print(CS_multiplier);
      cursor[1] = 1;      
      
    break;
  
  }
}

void setup_keypad() {
  pinMode(KEYPAD, INPUT);
  pinMode(KEYPAD_PWR, OUTPUT);

  digitalWrite(KEYPAD_PWR, HIGH);
}

uint8_t read_keypad() {

  //uint8_t val = read_analog_pin(KEYPAD);
  uint8_t val = analogRead(KEYPAD);
  Serial.println(val, DEC);

  if     (val > 980) return 1;  // -
  else if(val > 880) return 2;  // /
  else if(val > 80) return 3;  // 0
  else if(val > 15) return 4;  // _ or .
  else if(val > 660) return 5;  // +
  else if(val > 110) return 6;  // 9
  else if(val > 82) return 7;  // 8
  else if(val > 45) return 8;  // 7
  else if(val > 500) return 9;  // mode
  else if(val > 475) return 10; // 6
  else if(val > 455) return 11; // 5
  else if(val > 425) return 12; // 4
  else if(val > 360) return 13; // send
  else if(val > 60) return 14; // 3
  else if(val > 13) return 15; // 2
  else if(val > 200) return 16; // 1
  else return 0;
  
}


