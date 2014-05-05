#ifndef __ARDUINO_SERIAL_SOURCE_H__
#define __ARDUINO_SERIAL_SOURCE_H__

#include "smsg-source.h"

class SerialSource : public SMessageStreamSource {
public:
    SerialSource(Stream *comm) {
        m_comm = comm;
    }
protected:
    virtual void send(const char data){
        m_comm->print(data);
    }
    virtual char receive() {
        return m_comm->read();
    }
    virtual bool available() {
        return m_comm->available();
    }
private:
    Stream      *m_comm;
};


#endif // ARDUINO_SERIAL_SOURCE_H__
