#include "Task.h"

void Task::runAsync(void *self) {
    auto *task = (Task *) self;
    task->run();
}
