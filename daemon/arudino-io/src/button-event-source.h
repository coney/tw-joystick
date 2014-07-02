#ifndef __BUTTON_EVENT_SOURCE_H__
#define __BUTTON_EVENT_SOURCE_H__

#include "arduino-io.h"
#include "button-event.h"

class ButtonEventSource {
public:
    typedef void(*ButtonEventCallback)(const ButtonEvent &event);
    ButtonEventSource(ButtonEventCallback callback);

    ~ButtonEventSource();

    void poll(unsigned int interval_ms = 5);
    void setup();
private:
    int pinRead(int pin);

    char *status_table;
    ButtonEventCallback callback;
};

#endif // BUTTON_EVENT_SOURCE_H__
