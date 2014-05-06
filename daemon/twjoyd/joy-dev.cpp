#include <stdio.h>
#include <errno.h>
#include "js_button.h"
#include "joy-dev.h"
#include "joy-common.h"

using namespace std;

#define LOGPREFIX   "[joydev]"

int JoyDevice::updateButtonStatus( unsigned int arduino_button, bool pressed )
{
    if (!device_file) {
        logerror(LOGPREFIX "open device file failed");
        return -EIO;
    }
    const DriverButton *button = mapButton(arduino_button);
    if (button) {
        unsigned int player = button->first;
        int key = button->second;
        if (!pressed) {
            // above zero is pressed, below zero is released
            key = -key;
        }

        logtrace(LOGPREFIX "button:%u pressed:%u player:%u key:%d",
            arduino_button, pressed, player, key);
        fprintf(device_file, "%u %d", player, key);
        fflush(device_file);
    }
    else {
        logwarn(LOGPREFIX "unmapped arduino button %u", arduino_button);
        return -EINVAL;
    }

    return 0;
}

JoyDevice::~JoyDevice()
{
    if (device_file) {
        fclose(device_file);
        device_file = NULL;
    }
}

JoyDevice::JoyDevice( const char *device_path )
{
    device_file = fopen(device_path, "w");
}

int JoyDevice::loadKeyMap()
{
    unsigned int arudino_button = 0;
    for (unsigned int player = 0; player < JS_PLAYER_COUNT; ++player) {
        for (unsigned int button = JSBTN_UP; button <= JSBTN_X; ++button) {
            button_map[arudino_button] = make_pair(player, button);
            //logtrace("add key map arduino[%u] -> player[%u] button[%u]",
            //    arudino_button, player, button);
            ++arudino_button;
        }
    }
    return 0;
}

const JoyDevice::DriverButton * JoyDevice::mapButton( ArduinoButton button ) const
{
    map<ArduinoButton, DriverButton>::const_iterator it = button_map.find(button);
    return it == button_map.end() ? NULL : &it->second;
}
