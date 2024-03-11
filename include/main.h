#include <Arduino.h>
#undef min
#undef max
#include <string.h>
#include <vector>
#include <SimpleTimer.h>
#include <lib_mux.h>

#define DEBUG

enum {
    SET_PATTERN_BEGIN = 126,
    SET_PATTERN_END = 127,
    GET_MEASUREMENT = 128,
};

//0B 0A 0B 0A 0B 09 0B 09 0B 08 0B 08 0B 07 0B 07 0B 06 0B 06 0B 05 0B 05 0B 04 0B 04 0B 03 0B 03 0B 02 0B 02 0B 01 0B 01 0C 0A 0C 0A 0C 09 0C 09 0C 08 0C 08 0C 07 0C 07 0C 06 0C 06 0C 05 0C 05 0C 04 0C 04 0C 03 0C 03 0C 02 0C 02 0C 01 0C 01 0D 0A 0D 0A 0D 09 0D 09 0D 08 0D 08 0D 07 0D 07 0D 06 0D 06 0D 05 0D 05 0D 04 0D 04 0D 03 0D 03 0D 02 0D 02 0D 01 0D 01 0E 0A 0E 0A 0E 09 0E 09 0E 08 0E 08 0E 07 0E 07 0E 06 0E 06 0E 05 0E 05 0E 04 0E 04 0E 03 0E 03 0E 02 0E 02 0E 01 0E 01 0F 0A 0F 0A 0F 09 0F 09 0F 08 0F 08 0F 07 0F 07 0F 06 0F 06 0F 05 0F 05 0F 04 0F 04 0F 03 0F 03 0F 02 0F 02 0F 01 0F 01 10 0A 10 0A 10 09 10 09 10 08 10 08 10 07 10 07 10 06 10 06 10 05 10 05 10 04 10 04 10 03 10 03 10 02 10 02 10 01 10 01 11 0A 11 0A 11 09 11 09 11 08 11 08 11 07 11 07 11 06 11 06 11 05 11 05 11 04 11 04 11 03 11 03 11 02 11 02 11 01 11 01 12 0A 12 0A 12 09 12 09 12 08 12 08 12 07 12 07 12 06 12 06 12 05 12 05 12 04 12 04 12 03 12 03 12 02 12 02 12 01 12 01 13 0A 13 0A 13 09 13 09 13 08 13 08 13 07 13 07 13 06 13 06 13 05 13 05 13 04 13 04 13 03 13 03 13 02 13 02 13 01 13 01 14 0A 14 0A 14 09 14 09 14 08 14 08 14 07 14 07 14 06 14 06 14 05 14 05 14 04 14 04 14 03 14 03 14 02 14 02 14 01 14 01 15 0A 15 0A 15 09 15 09 15 08 15 08 15 07 15 07 15 06 15 06 15 05 15 05 15 04 15 04 15 03 15 03 15 02 15 02 15 01 15 01 16 0A 16 0A 16 09 16 09 16 08 16 08 16 07 16 07 16 06 16 06 16 05 16 05 16 04 16 04 16 03 16 03 16 02 16 02 16 01 16 01 17 0A 17 0A 17 09 17 09 17 08 17 08 17 07 17 07 17 06 17 06 17 05 17 05 17 04 17 04 17 03 17 03 17 02 17 02 17 01 17 01 18 0A 18 0A 18 09 18 09 18 08 18 08 18 07 18 07 18 06 18 06 18 05 18 05 18 04 18 04 18 03 18 03 18 02 18 02 18 01 18 01 19 0A 19 0A 19 09 19 09 19 08 19 08 19 07 19 07 19 06 19 06 19 05 19 05 19 04 19 04 19 03 19 03 19 02 19 02 19 01 19 01 1A 0A 1A 0A 1A 09 1A 09 1A 08 1A 08 1A 07 1A 07 1A 06 1A 06 1A 05 1A 05 1A 04 1A 04 1A 03 1A 03 1A 02 1A 02 1A 01 1A 01

