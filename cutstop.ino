#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "Configuration.h"
#include <ArduinoJson.h>

float set_point = 0;
float set_point_last = 0;
byte set_point_buffer[4];
float multiplier = .1;
int cursorPosition[2];


LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 4 line display

void setup() {
  pinMode(ENC_A, INPUT);
  digitalWrite(ENC_A, HIGH);
  pinMode(ENC_B, INPUT);
  digitalWrite(ENC_B, HIGH);

  lcd.init();  //initialize the lcd
  lcd.backlight();  //open the backlight 
  
  lcd.setCursor(0, 0);            // go to the top left corner
  lcd.print("encoder value: ");
  cursorPosition[0] = 0;
  cursorPosition[1] = 1;  
  lcd.setCursor(cursorPosition[0], cursorPosition[1]);
  
  Serial.begin (115200);
  //Serial.println("Start");

  StaticJsonBuffer <200> json_buffer;
  JsonObject& root = json_buffer.createObject();

}

void loop() {
  
  set_point += (multiplier * read_encoder())/2;
  
  if (set_point != set_point_last) {
    float_2_bytes(set_point, &set_point_buffer[0]);
    Serial.write(0x3C); // begin tranmission with '<'
    Serial.write(0x72); // character r enables set point
    Serial.write(0x2C);
    for (int i = 0; i < 4; i++) {
      Serial.write(set_point_buffer[i]);
    }
    Serial.write(0x3E); // end tranmission with '>'
  }
//  if (set_point != set_point_last) {
//    float_2_bytes(set_point, &set_point_buffer[0]);
//    Serial.print(0x3C, HEX); // begin tranmission with '<'
//    Serial.print(0x72, HEX); // character r enables set point
//    for (int i = 0; i < 4; i++) {
//      Serial.print(set_point_buffer[i], HEX);
//    }
//    Serial.println(0x3E, HEX); // end tranmission with '>'
//  }
  set_point_last = set_point;
  
  update_display();
  
}

int8_t read_encoder() {
  static int8_t enc_states_LUT[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
  static uint8_t old_AB = 0;

  old_AB <<= 2;                   //remember previous state
  old_AB |= ( ENC_PORT & 0x03 );  //add current state
  return (enc_states_LUT[( old_AB & 0x0f )]);

}

void update_display() {

    lcd.setCursor(cursorPosition[0], cursorPosition[1]);      
    lcd.print(set_point);
}

void float_2_bytes(float val, byte *bytes_array){
  
  // Create union of shared memory space
  union {
    float float_variable;
    byte temp_array[4];
  } u;
  
  // Overwrite bytes of union with float variable
  u.float_variable = val;
  
  // Assign bytes to input array
  memcpy(bytes_array, u.temp_array, 4);
}

float bytes_2_float(byte *bytes_array){
  
  // Create union of shared memory space
  union {
    float float_variable;
    byte temp_array[4];
  } u;
  
  // Overwrite bytes of union with float variable
  memcpy(u.temp_array, bytes_array, 4);
  
  // Assign bytes to input array
  return u.float_variable;
}


