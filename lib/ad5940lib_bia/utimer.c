#include "utimer.h"

// Global variables to replace class members
static unsigned long startTime = 0;
static unsigned long accumulatedTime = 0;
static int isRunning = 0; // Use int as a boolean (0 = false, 1 = true)

void toolTimer_init() {
    accumulatedTime = 0;
    isRunning = 0;
}

#ifdef MILLI

void toolTimer_start() {
    if (!isRunning) {
        startTime = millis();
        isRunning = 1;
    }
}

void toolTimer_stop(const char* timerName) {
    if (isRunning) {
        unsigned long endTime = millis();
        unsigned long duration = endTime - startTime;
        accumulatedTime += duration;
        printf("%s \n", timerName);
        printf(" Time spent: %lu ms\n", duration);
        isRunning = 0;
    }
}

#else // MICRO

void toolTimer_start() {
    if (!isRunning) {
        startTime = micros();
        isRunning = 1;
    }
}

void toolTimer_stop(const char* timerName) {
    if (isRunning) {
        unsigned long endTime = micros();
        unsigned long duration = endTime - startTime;
        accumulatedTime += duration;
        printf("%s \n", timerName);
        printf(" Time spent: %lu us\n", duration);
        isRunning = 0;
    }
}

#endif

void toolTimer_restart() {
    if (isRunning) {
        toolTimer_stop("Intermediate");
    }
    toolTimer_start();
}
