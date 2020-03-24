#ifndef RF_BRIDGE_MQTTEVENT_H
#define RF_BRIDGE_MQTTEVENT_H

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


#endif //RF_BRIDGE_MQTTEVENT_H
