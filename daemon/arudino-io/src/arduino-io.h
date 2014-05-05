#ifndef __ARDUINO_IO_H__
#define __ARDUINO_IO_H__

#include <Arduino.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define serial              Serial
#define SERIAL_BUAD_RATE    9600

#define MAX_STREAM_PRINTF_BUFFER_SIZE   128
void stream_printf(Stream &stream, const char *fmt, ...) {
    va_list vl;
    va_start(vl, fmt);
    char buf[MAX_STREAM_PRINTF_BUFFER_SIZE];
    vsnprintf(buf, sizeof(buf), fmt, vl);
    stream.print(buf);
    va_end(vl);
}
#define printf(fmt...)  stream_printf(serial, fmt)

#define MIN_DIGITAL_PIN     2
#define MAX_DIGITAL_PIN     12
#define DIGITAL_PINS_COUNT  (MAX_DIGITAL_PIN - MIN_DIGITAL_PIN + 1)
#define MIN_ANALOG_PIN      A0
#define MAX_ANALOG_PIN      A5
#define ANALOG_PINS_COUNT   (MAX_ANALOG_PIN - MIN_ANALOG_PIN + 1)
#define BUTTONS_COUNT       (DIGITAL_PINS_COUNT + ANALOG_PINS_COUNT)

#endif // ARDUINO_IO_H__
