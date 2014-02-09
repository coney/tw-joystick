/**
  * @file   js-input-gpio
  * @brief  get joystick input from gpio(for raspi)
  * @author ConeyWu
  * @date   02/09/2014
  */
#ifndef __JS_INPUT_GPIO_H__
#define __JS_INPUT_GPIO_H__

int js_input_gpio_init(void);
void js_input_gpio_clear(void);

#endif // __JS_INPUT_GPIO_H__