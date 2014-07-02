#ifndef __BUTTON_EVENT_H__
#define __BUTTON_EVENT_H__

#include <stdint.h>

enum {
    JSBTN_RESERVED,
    JSBTN_UP,
    JSBTN_DOWN,
    JSBTN_LEFT,
    JSBTN_RIGHT,
    JSBTN_A,
    JSBTN_B,
    JSBTN_C,
    JSBTN_X,
    JSBTN_Y,
    JSBTN_Z,
    JSBTN_COUNT
};

#define ButtonEventMessageId    0x1
struct ButtonEvent {
    ButtonEvent(uint8_t player = 0, uint8_t button = 0, uint8_t pressed = 0) {
        this->player = player;
        this->button = button;
        this->pressed = !!pressed;
    }
    uint8_t     player;
    uint8_t     button;
    uint8_t     pressed;
} __attribute__((packed));



#endif // BUTTON_EVENT_H__
