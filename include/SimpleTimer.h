// SimpleTimer.h
#ifndef SIMPLETIMER_H
#define SIMPLETIMER_H

#include <Arduino.h>

class SimpleTimer {
public:
    SimpleTimer();
    void start();
    void stop(const char* timerName);
    void restart(); // 重新开始计时，保留历史数据

private:
    unsigned long startTime;
    unsigned long accumulatedTime; // 累积时间
    bool isRunning; // 计时器是否正在运行
};

#endif // SIMPLETIMER_H