// main.h
#ifndef _MAIN_H
#define _MAIN_H

#include <vector>
#include <string.h>

#include <Arduino.h>
#include <hw_mux.h>
#include <u_bia.h>

enum {
    SET_PATTERN_BEGIN = 126,
    SET_PATTERN_END = 127,
    GET_MEASUREMENT = 128,
};

#endif // _MAIN_H
