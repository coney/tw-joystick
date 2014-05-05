#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "arduino-io.h"

#include "button-map.h"
#include "button-event-source.h"

#include "arduino-serial-source.h"
#include "smessage.h"
#include "button-event.h"

class Application {
public:
    static Application *getInstance() {
        static Application app;
        return &app;
    }

    void setup() {
        setup_serial();
        setup_input();
    }

    void run() {
        event_source->poll();
    }

private:
    Application() {
        smessage = new SMessage(new SerialSource(&serial));
        button_map = new ButtonMap();
        event_source = new ButtonEventSource(button_map, eventCallback);
    }


    void onButtonEvent(unsigned int button, bool pressed) {
        //printf("button[%u] => %u\r\n", button, pressed);
        ButtonEvent event;
        event.button = button;
        event.pressed = !!pressed;

#ifdef DEBUG
        event.button += 'a';
        event.pressed += '0';
#endif // DEBUG

        smessage->send(ButtonEventMessageId, event);
    }

    static void eventCallback(unsigned int button, bool pressed) {
        getInstance()->onButtonEvent(button, pressed);
    }

    void setup_serial() {
        serial.begin(SERIAL_BUAD_RATE);
    }

    void setup_input() {
        unsigned int index = 0;
        for (unsigned int pin = MIN_DIGITAL_PIN; pin <= MAX_DIGITAL_PIN; ++pin) {
            pinMode(pin, INPUT_PULLUP);
            button_map->updateMap(pin, index++);
        }

        // set all analog pins to digital pull-up mode
        for (unsigned int pin = MIN_ANALOG_PIN; pin <= MAX_ANALOG_PIN; ++pin) {
            pinMode(pin, INPUT);
            digitalWrite(pin, HIGH);
            button_map->updateMap(pin, index++);
        }
    }

    ButtonMap           *button_map;
    ButtonEventSource   *event_source;
    SMessage            *smessage;
};

#endif // APPLICATION_H__
