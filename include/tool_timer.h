// tool_timer.h
#ifndef _TOOL_TIMER_H
#define _TOOL_TIMER_H

#include <Arduino.h>

// #define milli
#define micro

class tool_timer {
public:
    tool_timer();
    void start();
    void stop(const char* timerName);
    void restart(); 

private:
    unsigned long startTime;
    unsigned long accumulatedTime;
    bool isRunning; 
};

#endif // _TOOL_TIMER_H