#ifndef __SMSG_SOURCE_H__
#define __SMSG_SOURCE_H__

class SMessageSource {
public:
    virtual ~SMessageSource() {}
    virtual int send(const char *buffer, size_t size) = 0;
    virtual int receive(char *outBuffer, size_t outSize) = 0;
};

class SMessageStreamSource : public SMessageSource {
public:
    virtual ~SMessageStreamSource() {}
    int send(const char *buffer, size_t size){
        for (size_t i = 0; i < size; ++i) {
            send(buffer[i]);
        }
        return size;
    }

    int receive(char *outBuffer, size_t outSize) {
        if (!outSize) {
            return 0;
        }

        size_t pos = 0;
        do {
            outBuffer[pos++] = receive();
        }while (available() && pos < outSize);

        return (int)pos;
    }
protected:
    virtual void send(const char data) = 0;
    virtual char receive() = 0;
    // optional 
    virtual bool available() {return false;}
};




#endif // SMSG_SOURCE_H__