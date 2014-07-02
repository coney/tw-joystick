#include <stdio.h>
#include <errno.h>
#include "js_button.h"
#include "joy-dev.h"
#include "joy-common.h"

using namespace std;

#define LOGPREFIX   "[joydev]"

int JoyDevice::updateButtonStatus( unsigned int player, 
                                  unsigned int button, 
                                  bool pressed )
{
    if (!device_file) {
        logerror(LOGPREFIX "open device file failed");
        return -EIO;
    }

    int key = pressed ? button : -button;

    logtrace(LOGPREFIX "button:%u pressed:%u player:%u key:%d",
        button, pressed, player, key);
    fprintf(device_file, "%u %d", player, key);
    fflush(device_file);


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
