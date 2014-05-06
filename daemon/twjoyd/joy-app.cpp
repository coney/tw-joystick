#include <Poco/FileChannel.h>
#include <Poco/Thread.h>

#include "joy-common.h"
#include "joy-app.h"
#include "serial-source.h"
#include "button-event.h"

using namespace std;
using namespace Poco;
using namespace Poco::Util;
using namespace Poco::Net;

JoyApp::JoyApp()
{
    m_serial_port = DEFAULT_SERIAL_PORT;
    m_log_file = DEFAULT_LOG_PATH;
    m_joy_device_path = DEFAULT_JOY_DEVICE_PATH;

    m_serial_msg = NULL;
    m_joydev = NULL;
}

int JoyApp::main( const std::vector < std::string > & args )
{
    loginfo("start processing");
    m_serial_msg->onUnhandledMessage([](SMessagePDU::Message *msg, void *arg) {
        JoyDevice *joydev = (JoyDevice *)arg;
        ButtonEvent *event = msg->get<ButtonEvent>();
        //logtrace("[serial]read msg type:%hhd size:%hhd button:%hhd pressed:%hhd", 
        //    msg->type, msg->size,
        //    event ? event->button : -1,
        //    event ? event->pressed : -1);
        if (event) {
            joydev->updateButtonStatus(event->button, event->pressed);
        }
    }, m_joydev);

    while (1) {
        m_serial_msg->feed();
    }
    return EXIT_OK;
}

Poco::Logger::Channel * JoyApp::getFileChannel(const std::string &path)
{
    Poco::FileChannel *channel = new Poco::FileChannel(path);
    channel->setProperty("rotation", "4 M");
    return channel;
}


void JoyApp::initialize( Application & self )
{
    ServerApplication::initialize(self);
    DEFAULT_LOGGER.setLevel(Poco::Message::PRIO_TRACE);

    if (config().getBool("application.runAsDaemon", false)) {
        logdebug("write log to %s", m_log_file.c_str());
        DEFAULT_LOGGER.setChannel(getFileChannel(m_log_file));
    }

    loginfo("log level is %d", DEFAULT_LOGGER.getLevel());
    loginfo("using serial port %s", m_serial_port.c_str());

    if (!setupSerial()) {
        logerror("can't connect to %s", m_serial_port.c_str());
        exit(EXIT_IOERR);
    }

    if (!setupJoyDevice()) {
        logerror("open joy device %s failed", m_joy_device_path.c_str());
        exit(EXIT_IOERR);
    }
}


void JoyApp::defineOptions( OptionSet & options )
{
    ServerApplication::defineOptions(options);
    options.addOption(Option("serial-port", "s").argument("serial-port"));
    options.addOption(Option("logfile", "l").argument("logfile"));
    options.addOption(Option("joydev", "j").argument("joydev"));
}

void JoyApp::handleOption( const std::string & name, const std::string & value )
{
    ServerApplication::handleOption(name, value);

    if (name == "serial-port") {
        m_serial_port = value;
    } 
    else if (name == "logfile") {
        m_log_file = value;
    }
    else if (name == "joydev")
    {
        m_joy_device_path = value;
    }
}

bool JoyApp::setupSerial()
{
    try {
        SerialPort *serial_port = new SerialPort(m_serial_port);
        serial_port->Open(SerialPort::BAUD_9600,
            SerialPort::CHAR_SIZE_8,
            SerialPort::PARITY_NONE,
            SerialPort::STOP_BITS_1,
            SerialPort::FLOW_CONTROL_NONE);

        if (!serial_port->IsOpen()) {
            delete serial_port;
            return false;
        }

        m_serial_msg = new SMessage(new SerialSource(serial_port));
    }
    catch (exception &e) {
        logerror("open '%s' failed, e:%s", m_serial_port.c_str(), e.what());
        return false;
    }
    
    return true;
}

bool JoyApp::setupJoyDevice()
{
    m_joydev = new JoyDevice(m_joy_device_path.c_str());
    return m_joydev->loadKeyMap() == 0;
}


int main(int argc, char **argv) {
    return JoyApp().run(argc, argv);
}