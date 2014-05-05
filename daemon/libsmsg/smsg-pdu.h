#ifndef __SMSG_PDU_H__
#define __SMSG_PDU_H__

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "smsg-coder.h"

#define SMSG_PDU_MESSAGE_SIZE_VALIDATION
#define SMSG_PDU_DEFALFT_CALLBACK_INDEX     0
#define SMSG_PDU_MAX_CALLBACK_COUNT         32

class SMessagePDU {
public:
    struct Message {
        template <class T>
        T *get() {
#ifdef SMSG_PDU_MESSAGE_SIZE_VALIDATION
            if (sizeof(T) != size) {
                return NULL;
            }
#endif // SMSG_PDU_MESSAGE_SIZE_VALIDATION
            return (T*)data;
        }
        uint8_t type;
        uint8_t size;
        char    data[];
    };
    
    typedef void(*MessageCallbackProc)(Message *message, void *userdata);

    SMessagePDU();
    virtual ~SMessagePDU();

    template <class T>
    bool encode(uint8_t type, const T *data,
        char *outbuf, size_t *outsize) {
        return encode(type, (const char *)data, sizeof(T),
            outbuf, outsize);
    }
    bool encode(uint8_t type, const char *inbuf, size_t insize,
        char *outbuf, size_t *outsize);


    void feed(const char data);
    void onMessage(uint8_t type, MessageCallbackProc callback, void *userdata = NULL);
    void onUnhandledMessage(MessageCallbackProc callback, void *userdata = NULL);
private:
    struct MessageCallbackEntry {
        uint8_t             type;
        MessageCallbackProc callback;
        void                *userdata;
    };
    void processRawMessage(const char *buf, size_t size);
    static void rawMessageCallback(const char *buf, size_t size, void *arg);
    void setupRawMessageCallback();

    SMessageCoder                   m_coder;
    MessageCallbackEntry            m_callbacks[SMSG_PDU_MAX_CALLBACK_COUNT];
    MessageCallbackEntry            m_default_callback;
};

#endif // __SMSG_PDU_H__
