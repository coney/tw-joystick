/**
  * @file   js_proc.h
  * @brief  Joystick procfs header
  * @author ConeyWu
  * @date   10/01/2014
  */
#ifndef __JS_PROC_H__
#define __JS_PROC_H__

#define JS_PROC_NAME            "twjoystick"
#define JS_PROC_CONFIG          "config"
#define JS_PROC_TEST            "test"
#define JS_PROC_BUFSIZE         2048

int js_proc_init(void);
void js_proc_clear(void);

#endif // __JS_PROC_H__
