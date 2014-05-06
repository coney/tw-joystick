#ifndef __JOY_COMMON_H__
#define __JOY_COMMON_H__

#include <stdarg.h>
#include <Poco/Logger.h>

#define DEFAULT_SERIAL_PORT             "/dev/ttyUSB0"
#define DEFAULT_JOY_DEVICE_PATH         "/proc/twjoystick/test"
#define DEFAULT_LOG_PATH                "/var/log/twjoyd.log"

#define DEFAULT_LOGGER                  (Poco::Logger::root())          

#define APP_NAME                        "twjoyd"
inline static void writelog(Poco::Message::Priority level, const char *fmt, ...) {
    va_list vl;
    va_start(vl, fmt);
    char buf[4096];
    vsnprintf(buf, sizeof(buf), fmt, vl);
    buf[sizeof(buf) - 1] = 0;
    Poco::Message msg(APP_NAME, buf, level);
    DEFAULT_LOGGER.log(msg);
    va_end(vl);
}

#define logtrace(fmt...)  writelog(Poco::Message::PRIO_TRACE, fmt)
#define loginfo(fmt...)   writelog(Poco::Message::PRIO_INFORMATION, fmt)
#define logdebug(fmt...)  writelog(Poco::Message::PRIO_DEBUG, fmt)
#define logwarn(fmt...)  writelog(Poco::Message::PRIO_WARNING, fmt)
#define logerror(fmt...)  writelog(Poco::Message::PRIO_ERROR, fmt)

#endif // JOY_COMMON_H__
