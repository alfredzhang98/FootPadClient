// main.h
#ifndef _MAIN_H
#define _MAIN_H

#include <vector>
#include <string.h>

#include <Arduino.h>
#include <hw_mux.h>

enum {
    SET_PATTERN_BEGIN = 126,
    SET_PATTERN_END = 127,
    GET_MEASUREMENT = 128,
    GET_MEASUREMENT_1 = 129,
    SET_PATTERN_INDEX = 130,
    SWITCH_PRINT = 0x90,
};

#endif // _MAIN_H
