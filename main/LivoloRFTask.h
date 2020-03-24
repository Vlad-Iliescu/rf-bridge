#ifndef HELLO_WORLD_LIVOLORFTASK_H
#define HELLO_WORLD_LIVOLORFTASK_H

#include <driver/gpio.h>
#include "interfaces/Task.h"

typedef unsigned char byte;

class LivoloRFTask : public Task {
public:
    explicit LivoloRFTask(gpio_num_t txPin);

    void sendSignal(unsigned int remoteId, byte keycode);

    void run() override;

private:
    gpio_num_t txPin;
    bool high = false; // pulse "sign" (low|high)

    void selectPulse(byte bit);

    void sendPulse(byte pulse);
};


#endif //HELLO_WORLD_LIVOLORFTASK_H
