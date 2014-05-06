#ifndef __SERIAL_SOURCE_H__
#define __SERIAL_SOURCE_H__

#include <SerialPort.h>
#include "smsg-source.h"

class SerialSource : public SMessageStreamSource {
public:
    SerialSource(SerialPort *comm) {
        m_comm = comm;
    }
protected:
    virtual void send(const char data){
        m_comm->WriteByte(data);
    }
    virtual char receive() {
        char data = m_comm->ReadByte();
        return data;
    }
    virtual bool available() {
        return m_comm->IsDataAvailable();
    }
private:
    SerialPort  *m_comm;
};

#endif // SERIAL_SOURCE_H__
