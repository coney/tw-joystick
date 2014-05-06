#ifndef __JOY_DEV_H__
#define __JOY_DEV_H__

#include <map>

// using proc instead of dev ioctl
class JoyDevice {
public:
    typedef std::pair<unsigned int, unsigned int>    DriverButton;
    typedef unsigned int                             ArduinoButton;
    JoyDevice (const char *device_path);
    virtual ~JoyDevice();

    const DriverButton *mapButton(ArduinoButton button) const;
    int loadKeyMap();
    int updateButtonStatus(unsigned int button, bool pressed);
private:
    std::map<ArduinoButton, DriverButton>            button_map;
    FILE                                            *device_file;
};

#endif // JOY_DEV_H__
