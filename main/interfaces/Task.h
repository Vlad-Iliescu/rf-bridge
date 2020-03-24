#ifndef HELLO_WORLD_TASK_H
#define HELLO_WORLD_TASK_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "../common.h"

class Task {
public:
    virtual void run() = 0;

    static void runAsync(void *self);
};

#endif //HELLO_WORLD_TASK_H
