#ifndef MAIN_H
#define MAIN_H

#include <vector>
#include <string.h>

#include <Arduino.h>

#include <Arduino.h>
#include <ad5940_bia.h>
#include <hw_mux.h>

enum {
    SET_PATTERN_BEGIN = 126,
    SET_PATTERN_END = 127,
    GET_MEASUREMENT = 128,
};

#endif //MAIN_H