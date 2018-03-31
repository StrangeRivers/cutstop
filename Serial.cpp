#include "Serial.h"
#include "State.h"

uint8_t rx_buffer_S1[];
uint8_t num_received = 0;
bool rx_ack_S1 = false;
const uint8_t start_marker = 0x3C;
const uint8_t end_marker = 0x3E;

bool new_data = false;

bool serial_check(uint8_t *receive_buffer) {
  
    static bool receive_in_progress = false;
    static uint8_t ndx = 0;

    uint8_t rb;
  
    while (Serial.available() > 0 && new_data == false) {

        rx_ack_S1 = false;
        rb = Serial.read();

        //TODO: add counter to ignore partial messages
        if (receive_in_progress == true) {
          
            if (rb != end_marker) {
                receive_buffer[ndx] = rb;
                ndx++;
                if (ndx >= num_bytes) {
                    ndx = num_bytes - 1;
                }
            }
            
            else {
                receive_buffer[ndx] = '\0'; // terminate the string
                receive_in_progress = false;
                num_received = ndx;  // save the number for use when printing
                ndx = 0;
                new_data = true;
            }
        }

        else if (rb == start_marker) {
            receive_in_progress = true;
        }
    }

  return new_data;
}

void parse_new_data(uint8_t *receive_buffer) {

  // Current interface is a character byte followed by four bytes that make up a float - a key/value pair
  // would be great to update this in the future to be more general and able to handle more complex messages (JSON?)
  
  if (new_data == true) {
    uint8_t key = receive_buffer[0];
    union {float value; uint8_t convert[4];} u;

    for (uint8_t i=1; i < num_received; i++) {
      u.convert[i-1] = receive_buffer[i];
    }

    new_data = false; // overwrite the S1 rx buffer next round

    switch ((char)key) {

      case 'u': // incoming message is effort report
        MS_u = u.value;
      break;

      case 'r': // setpoint report
        MS_r = u.value;
      break;

      case 'y': // measured angle report
        MS_y = u.value;
      break;

      case 'v': // velocity report
        MS_v = u.value;
      break;

      case 'e': // error report
        MS_e = u.value;
      break;

      case 'z': // zero angle report
        MS_zero_angle = u.value;
      break;

      case 'm': // mode report
        MS_mode = u.value;
      break;
    }
  }
}

void send_data(char key, float value, int repeat) {
  // repeated sends are a crappy form of error correction for now
  
  union {float float_variable; uint8_t temp_array[4];} u;

  u.float_variable = value;

  for (int i = 0; i < repeat; i++) {
    Serial.write(start_marker);
    Serial.write(key);
    for (int i = 0; i<4; i++) {
      Serial.write(u.temp_array[i]);
    }
    Serial.write(end_marker);
  }
  
}

