#include <alloca.h>
#include "smessage.h"

void SMessage::onUnhandledMessage( SMessagePDU::MessageCallbackProc callback ,
                                  void *userdata)
{
    m_msgPdu.onUnhandledMessage(callback, userdata);
}

void SMessage::onMessage( uint8_t type,
                         SMessagePDU::MessageCallbackProc callback,
                         void *userdata )
{
    m_msgPdu.onMessage(type, callback, userdata);
}

int SMessage::feed()
{
    char buffer[256];
    int ret = m_msgSrouce->receive(buffer, sizeof(buffer));
    if (ret > 0) {
        for (size_t i = 0; i < (size_t)ret; ++i) {
            m_msgPdu.feed(buffer[i]);
        }
    } 
    return ret;
}

bool SMessage::send( uint8_t type, const char *buffer, size_t size )
{
    // todo - worse case
    size_t outSize = size * 2 + 2; 
    char *encodedData = (char *)alloca(outSize);
    if (!m_msgPdu.encode(type, buffer, size, encodedData, &outSize)) {
        return false;
    }

    return (size_t)m_msgSrouce->send(encodedData, outSize) == outSize;
}

SMessage::~SMessage()
{

}

SMessage::SMessage( SMessageSource *msgSource )
{
    m_msgSrouce = msgSource;
}
