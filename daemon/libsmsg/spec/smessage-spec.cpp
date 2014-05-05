#include <vector>

#include "spec_helper.h"
#include "smessage.h"

using namespace std;

template <class Iter>
void dump_buffer(Iter begin, Iter end) {
    while (begin != end) {
        printf("%02hhX ", *begin++);
    }
    puts("\n");
}

class FakeSource : public SMessageSource {
public:
    virtual int send(const char *buffer, size_t size) {
        sendBuffer.assign(buffer, buffer + size);
        return size;
    }
    virtual int receive(char *outBuffer, size_t outSize) {
        if (receiveBuffer.size() > outSize) {
            return -1;
        }

        memcpy(outBuffer, &receiveBuffer[0], receiveBuffer.size());
        return receiveBuffer.size();
    }
    vector<char> sendBuffer;
    vector<char> receiveBuffer;
};

TEST(SMessage, ShouldSendEncodedDataViaSourceInterface){
    FakeSource fakeSource;
    SMessage smsg(&fakeSource);

    int data = 0x9527;
    ASSERT_TRUE(smsg.send(3, data));
    ASSERT_EQ(3 + sizeof(data), fakeSource.sendBuffer.size());
    EXPECT_EQ(3, fakeSource.sendBuffer[0]);
    EXPECT_EQ(sizeof(data), (size_t)fakeSource.sendBuffer[1]);
    EXPECT_EQ(data, *(int*)&fakeSource.sendBuffer[2]);
}

static void MessageCallback(SMessagePDU::Message *msg, void *userdata) {
    *(SMessagePDU::Message **)userdata = msg;
}

TEST(SMessage, ShouldCallTheCorrespondingCallbackOnReceive){
    FakeSource fakeSource;
    SMessage smsg(&fakeSource);
    SMessagePDU::Message *message = NULL;
    smsg.onMessage(3, MessageCallback, &message);

    fakeSource.receiveBuffer.push_back(0x03);
    fakeSource.receiveBuffer.push_back(0x04);
    fakeSource.receiveBuffer.push_back(0xcc);
    fakeSource.receiveBuffer.push_back(0x55);
    fakeSource.receiveBuffer.push_back(0x00);
    fakeSource.receiveBuffer.push_back(0xff);
    fakeSource.receiveBuffer.push_back('|');

    ASSERT_EQ(fakeSource.receiveBuffer.size(), (size_t)smsg.feed());
    ASSERT_NE((void *)NULL, message);
    EXPECT_EQ(3, message->type);
    EXPECT_EQ(4, message->size);
    EXPECT_EQ((char)0xcc, message->data[0]);
    EXPECT_EQ((char)0x55, message->data[1]);
    EXPECT_EQ((char)0x00, message->data[2]);
    EXPECT_EQ((char)0xff, message->data[3]);
}