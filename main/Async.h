#ifndef HELLO_WORLD_ASYNC_H
#define HELLO_WORLD_ASYNC_H


#include "interfaces/Task.h"

class Async : public Task {
    void run() override;
};


#endif //HELLO_WORLD_ASYNC_H
