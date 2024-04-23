// hw_mux.h
#ifndef _HW_MUX_H
#define _HW_MUX_H

#include <Arduino.h>

#define D0_pin 37
#define D1_pin 35
#define D2_pin 33
#define D3_pin 31
#define D4_pin 29

//按照F+F- S+S-连接
#define MUX_WR_1 51
#define MUX_WR_2 25
#define MUX_WR_3 53
#define MUX_WR_4 23


// F_1 and F_0 are connect to the VCC
#define S_plus 0
#define S_minus 0

void setupMux();

void handleMux(const uint8_t *config);

#endif // _HW_MUX_H
