#ifndef __SMESSAGE_H__
#define __SMESSAGE_H__

#include <stdint.h>
#include "smsg-pdu.h"
#include "smsg-source.h"

class SMessage {
public:
    SMessage(SMessageSource *msgSource);
    virtual ~SMessage();

    template< class T>
    bool send(uint8_t type, const T& data){
        return send(type, (const char *)&data, sizeof(data));
    }
    bool send(uint8_t type, const char *buffer, size_t size);

    int feed();
    void onMessage(uint8_t type, 
        SMessagePDU::MessageCallbackProc callback,
        void *userdata = NULL);
    void onUnhandledMessage(SMessagePDU::MessageCallbackProc callback,
        void *userdata = NULL);
private:
    SMessagePDU     m_msgPdu;
    SMessageSource  *m_msgSrouce;
};

#endif // SMESSAGE_H__
