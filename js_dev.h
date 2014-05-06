/**
  * @file   js_dev.h
  * @brief  Virtual input device
  * @author ConeyWu
  * @date   10/01/2014
  */
#ifndef __JS_DEV_H__
#define __JS_DEV_H__

int js_device_init(void);
void js_device_clear(void);

/**
  * @brief Process button event
  * @param INPUT button button value
  * @return 
  */
int js_device_process(int player, int button, int value);

#endif // __JS_DEV_H__
