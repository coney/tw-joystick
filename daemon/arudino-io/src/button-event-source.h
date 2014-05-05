#ifndef __BUTTON_EVENT_SOURCE_H__
#define __BUTTON_EVENT_SOURCE_H__

#include "arduino-io.h"
#include "button-map.h"

class ButtonEventSource {
public:
    typedef void(*ButtonEventCallback)(unsigned int button, bool pressed);
    ButtonEventSource(const ButtonMap *button_map, ButtonEventCallback callback) {
        this->callback = callback;

        this->button_map = button_map;
        status_table = (char *)malloc(button_map->getButtonCount());
        memset(status_table, HIGH, button_map->getButtonCount());
    }

    ~ButtonEventSource() {
        free(status_table);
    }

    void poll(unsigned int interval_ms = 5) {
        for (unsigned int btn = 0; btn < button_map->getButtonCount(); ++btn) {
            unsigned int status = digitalRead(button_map->getPin(btn));
            if (status_table[btn] != status) {
                status_table[btn] = status;
                // using the pull-up mode, LOW is button pressed
                callback(btn, status == LOW);
            }
        }
        delay(interval_ms);
    }

private:
    char *status_table;
    ButtonEventCallback callback;
    const ButtonMap *button_map;
};

#endif // BUTTON_EVENT_SOURCE_H__
