#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "arduino-io.h"

#include "button-event-source.h"

#include "arduino-serial-source.h"
#include "smessage.h"
#include "button-event.h"

class Application {
public:
    static Application *getInstance();

    void setup();

    void run();

private:
    Application();
    
    void onButtonEvent(const ButtonEvent &event);

    static void eventCallback(const ButtonEvent &event);
    ButtonEventSource   *event_source;
    SMessage            *smessage;
};

#endif // APPLICATION_H__
