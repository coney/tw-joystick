#ifndef __BUTTON_EVENT_H__
#define __BUTTON_EVENT_H__

#include <stdint.h>

#define ButtonEventMessageId    0x1
struct ButtonEvent {
    uint8_t     button;
    uint8_t     pressed;
} __attribute__((packed));

#endif // BUTTON_EVENT_H__
