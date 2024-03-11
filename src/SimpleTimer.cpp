// SimpleTimer.cpp
#include "SimpleTimer.h"

SimpleTimer::SimpleTimer() : accumulatedTime(0), isRunning(false) {}

void SimpleTimer::start() {
    if (!isRunning) {
        startTime = millis();
        isRunning = true;
    }
}

void SimpleTimer::stop(const char* timerName) {
    if (isRunning) {
        unsigned long endTime = millis();
        unsigned long duration = endTime - startTime;
        accumulatedTime += duration;
        Serial.print(timerName);
        Serial.print(" TIme spent: ");
        Serial.print(duration);
        Serial.print(" ms");
        Serial.print(" TIme total: ");
        Serial.print(accumulatedTime);
        Serial.println(" ms");
        isRunning = false;
    }
}

void SimpleTimer::restart() {
    if (isRunning) {
        stop("Intermediate");
    }
    start();
}
