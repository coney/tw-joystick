#ifndef __SMSG_CODER_H__
#define __SMSG_CODER_H__

#include <stdlib.h>

#define SMSG_DECODER_BUFFER_SIZE    256

class SMessageCoder {
public:
    typedef void(*DecoderCallbackProc)(const char *buf, size_t size, void *userdata);

    SMessageCoder(const char delimiter = '|', const char escaper = '\\');
    virtual ~SMessageCoder();

    bool encode(const char *inbuf, size_t insize, 
        char *outbuf, size_t *outsize);

    void feed(const char data);
    void onDecode(DecoderCallbackProc callback, void *userdata);

private:
    bool needEscape(const char data);
    void feedByte(const char data);
    void endMessage();

    DecoderCallbackProc     m_decode_callback;
    void                    *m_userdata;
    const char              m_delimiter;
    const char              m_escaper;
    char                    m_buffer[SMSG_DECODER_BUFFER_SIZE];
    size_t                  m_buffer_index;
    bool                    m_escaping;
    bool                    m_error;
};

#endif // __SMSG_CODER_H__
