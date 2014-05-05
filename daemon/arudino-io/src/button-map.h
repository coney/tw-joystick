#ifndef __BUTTON_MAP_H__
#define __BUTTON_MAP_H__

#include "arduino-io.h"

class ButtonMap {
public:
    ButtonMap() {
        memset(button_to_pin_map,0 , sizeof(button_to_pin_map));
        memset(pin_to_button_map,0 , sizeof(pin_to_button_map));
    }

    void updateMap(unsigned int pin, unsigned int button) {
        printf("update %u => %u\r\n", pin, button);
        button_to_pin_map[button] = pin;
        pin_to_button_map[pin] = button;
    }

    unsigned int getButton(unsigned int pin) const{
        // no sanity check
        return pin_to_button_map[pin];
    }

    unsigned int getPin(unsigned int button) const {
        return button_to_pin_map[button];
    }

    unsigned int getButtonCount() const{
        return ARRAY_SIZE(button_to_pin_map);
    }

private:
    unsigned int pin_to_button_map[MAX_ANALOG_PIN + 1];
    unsigned int button_to_pin_map[BUTTONS_COUNT];
};

#endif // BUTTON_MAP_H__
