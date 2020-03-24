#ifndef HELLO_WORLD_MQTTEVENT_H
#define HELLO_WORLD_MQTTEVENT_H

typedef enum {
    OFF = 0,
    ON = 1
} DeviceState;

class MqttEvent {
public:
    MqttEvent(unsigned int remoteId, unsigned char key, DeviceState state);

    MqttEvent();

    virtual ~MqttEvent();

public:
    unsigned int remoteId{};
    unsigned char key{};
    DeviceState state;
};


#endif //HELLO_WORLD_MQTTEVENT_H
