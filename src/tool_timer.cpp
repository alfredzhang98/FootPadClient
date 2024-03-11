// tool_timer.cpp
#include "tool_timer.h"
#include "tool_logger.h"

tool_timer::tool_timer() : accumulatedTime(0), isRunning(false) {}

void tool_timer::start() {
    if (!isRunning) {
        startTime = millis();
        isRunning = true;
    }
}

void tool_timer::stop(const char* timerName) {
    if (isRunning) {
        unsigned long endTime = millis();
        unsigned long duration = endTime - startTime;
        accumulatedTime += duration;
        logger.logln(timerName);
        logger.loglnf(" Time spent: %d  ms", duration);
        logger.loglnf(" Time total: %d  ms", accumulatedTime);
        isRunning = false;
    }
}

void tool_timer::restart() {
    if (isRunning) {
        stop("Intermediate");
    }
    start();
}
