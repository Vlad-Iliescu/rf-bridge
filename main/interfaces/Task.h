#ifndef RF_BRIDGE_TASK_H
#define RF_BRIDGE_TASK_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "../common.h"

class Task {
public:
    virtual void run() = 0;

    static void runAsync(void *self);
};

#endif //RF_BRIDGE_TASK_H
