#ifndef U_TIMER_H
#define U_TIMER_H

#include <Arduino.h>

// Select the timing precision
// #define MILLI
#define MICRO

void toolTimer_init();
void toolTimer_start();
void toolTimer_stop(const char* timerName);
void toolTimer_restart();

#endif // U_TIMER_H
