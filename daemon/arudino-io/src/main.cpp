//#line 2 "src/sketch.ino"
#include "application.h"

void setup()
{
    Application::getInstance()->setup();
}

void loop()
{
    Application::getInstance()->run();
}
