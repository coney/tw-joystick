/**
  * @file   js_log.h
  * @brief  A simple log implementation
  * @author ConeyWu
  * @date   10/01/2014
  */
#ifndef __JS_LOG_H__
#define __JS_LOG_H__


#define __JS_TOSTRING(x)        #x
#define _JS_TOSTRING(x)         __JS_TOSTRING(x)

#define writelog(level, fmt...)        \
    printk(level __FILE__ ":" _JS_TOSTRING(__LINE__)" - " fmt)

#define logdebug(fmt...)        writelog(KERN_DEBUG, fmt)
#define loginfo(fmt...)         writelog(KERN_INFO, fmt)
#define logerror(fmt...)        writelog(KERN_ERR, fmt)

#endif // __JS_LOG_H__