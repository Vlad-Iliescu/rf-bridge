#include "Task.h"

void Task::runAsync(void *data) {
    auto *task = (Task *) data;
    task->run();
}
