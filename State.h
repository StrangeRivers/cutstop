#ifndef __STATE_H__
#define __STATE_H__

#include <stdint.h>

// Mecaduino state
extern float MS_u;            // control effort
extern float MS_r;            // current setpoint
extern float MS_y;            // measured angle
extern float MS_v;            // estimated velocity (velocity loop)
extern float MS_e;            // e = r-y (error)
extern float MS_zero_angle;   // calibrated zero angle
extern char MS_mode;          // device state

// Controller state
extern float CS_position;     // trolley position (position + offset)
extern float CS_offset;       // calibrated distance from blade
extern float CS_r;            // requested setpoint
extern float CS_multiplier;   // jog wheel multiplier
extern char  CS_mode;         // device state
extern float CS_scale_factor;

// Display state
extern int8_t cursor[2];

void jog_mode();

void mdi_mode();

#endif
