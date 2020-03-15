#ifndef HELLO_WORLD_TASK_H
#define HELLO_WORLD_TASK_H

#include "../common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class Task {
public:
    virtual void run() = 0;

    static void runAsync(void *data);
};

#endif //HELLO_WORLD_TASK_H
