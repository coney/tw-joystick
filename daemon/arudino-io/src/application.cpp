#include "application.h"


void Application::eventCallback(const ButtonEvent &event)
{
    getInstance()->onButtonEvent(event);
}

void Application::onButtonEvent(const ButtonEvent &event)
{
    //printf("button[%u] => %u\r\n", button, pressed);

//#ifdef DEBUG
//    event.button += 'a';
//    event.pressed += '0';
//#endif // DEBUG
  
    smessage->send(ButtonEventMessageId, event);
}

Application::Application()
{
    smessage = new SMessage(new SerialSource(&serial));
    event_source = new ButtonEventSource(eventCallback);
}

void Application::run()
{
    event_source->poll();
}

void Application::setup()
{
    serial.begin(SERIAL_BUAD_RATE);
    event_source->setup();
}

Application * Application::getInstance()
{
    static Application app;
    return &app;
}
