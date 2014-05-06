#ifndef __JOY_APP_H__
#define __JOY_APP_H__

#include <stdint.h>
#include <string>
#include <Poco/Util/ServerApplication.h>
#include <Poco/Net/DatagramSocket.h>
#include <SerialPort.h>
#include <list>
#include "smessage.h"
#include "joy-dev.h"

class JoyApp : public Poco::Util::ServerApplication {
public:
    JoyApp();

protected:
    void initialize(Application & self);
    virtual int main(const std::vector < std::string > & args);
    virtual void defineOptions(Poco::Util::OptionSet & options);
    virtual void handleOption(const std::string & name,
        const std::string & value);

private:
    bool setupSerial();
    bool setupJoyDevice();

    Poco::Logger::Channel * getFileChannel(const std::string &path);
    std::string                 m_serial_port;
    std::string                 m_log_file;
    std::string                 m_joy_device_path;
    JoyDevice                   *m_joydev;
    SMessage                    *m_serial_msg;
};

#endif // JOY_APP_H__
