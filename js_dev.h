/**
  * @file   js_dev.h
  * @brief  Virtual input device
  * @author ConeyWu
  * @date   10/01/2014
  */
#ifndef __JS_DEV_H__
#define __JS_DEV_H__

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
    JSBTN_Z
};

#define JS_IS_DIRECTION(btn) ((btn) >= JSBTN_UP && (btn) <= JSBTN_RIGHT)
#define JS_IS_BUTTON(btn) ((btn) >= JSBTN_A)


int js_device_init(void);
void js_device_clear(void);

/**
  * @brief Process button event
  * @param INPUT button button value
  * @return 
  */
int js_device_process(int id, int button, int value);

#endif // __JS_DEV_H__
