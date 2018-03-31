#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "arduino.h"

  const int num_bytes = 32;
  extern uint8_t rx_buffer_S1[num_bytes];
  
  bool serial_check(uint8_t receive_buffer[]);              //checks serial port for commands.  Must include this in loop() for serial interface to work

  void parse_new_data(uint8_t receive_buffer[]);

  void send_data(char key, float value, int repeat);

#endif
