#ifndef __JS_BUTTON_H__
#define __JS_BUTTON_H__

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

#define JS_PLAYER_COUNT 2

#define JS_IS_DIRECTION(btn) ((btn) >= JSBTN_UP && (btn) <= JSBTN_RIGHT)
#define JS_IS_BUTTON(btn) ((btn) >= JSBTN_A)

#endif // JS_BUTTON_H__
