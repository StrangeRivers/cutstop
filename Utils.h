#ifndef __UTILS_H__
#define __UTILS_H__

int8_t read_encoder();

bool read_digital_pin(uint8_t pin);

bool read_analog_pin(uint8_t pin);

bool update_multiplier();

void setup_display();

void update_display();

void setup_keypad();

uint8_t read_keypad();

#endif
