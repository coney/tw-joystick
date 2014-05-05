#include <vector>

#include "spec_helper.h"
#include "smsg-coder.h"

using namespace std;

TEST(SMessageCoder, EncodeShouldEscapeBufferAndAddDelimiter) {
    const char *rawBuffer = "hello|kitty\\";

    SMessageCoder smcoder;
    char outbuf[64] = { 0 };
    size_t outlen = sizeof(outbuf);

    bool result = smcoder.encode(rawBuffer, strlen(rawBuffer), outbuf, &outlen);
    ASSERT_TRUE(result);

    const char *expectedBuffer = "hello\\|kitty\\\\|";
    ASSERT_EQ(strlen(expectedBuffer), outlen);
    EXPECT_EQ(0, strcmp(expectedBuffer, outbuf));
}

TEST(SMessageCoder, EncodeShouldReturnFalseIfOutBufferIsNotBigEnough) {
    SMessageCoder smcoder;
    char outbuf[8] = { 0 };
    size_t outlen = sizeof(outbuf);
    EXPECT_FALSE(smcoder.encode("12345678", 8, outbuf, &outlen));
    EXPECT_FALSE(smcoder.encode("1234|56", 7, outbuf, &outlen));
    EXPECT_FALSE(smcoder.encode("123\\456", 7, outbuf, &outlen));
}

// can't compile gtest & gmock with -std=c++0x
// lambda is not working 
//TEST(SMessageCoder, ShouldCallTheCallbackProcWhenDataIsDecoded) {
//    SMessageCoder smcoder;
//    char decodedBuf[64] = { 0 };
//    size_t decodedSize = 0;
//    int userdata = 0x9527;
//    smcoder.onDecode([&](const char *buf, size_t size, void *arg) {
//        ASSERT_GE(sizeof(decodedBuf), size);
//        memcpy(decodedBuf, buf, size);
//        decodedSize = size;
//        EXPECT_EQ(&userdata, (int *)arg);
//
//    }, (void*)&userdata);
//}


struct DecodedMesssage {
    size_t  size;
    char    buffer[64];
};
typedef std::vector<DecodedMesssage> Messages;

static void DecodeCallback(const char *buf, size_t size, void *userdata) {
    DecodedMesssage msg;
    ASSERT_GE(sizeof(msg.buffer), size);
    memcpy(msg.buffer, buf, size);
    msg.size = size;

    Messages *messages = (Messages *)userdata;
    messages->push_back(msg);
}

TEST(SMessageCoder, ShouldCallTheCallbackProcWhenDataIsDecoded) {
    SMessageCoder smcoder;
    Messages messages;
    smcoder.onDecode(DecodeCallback, &messages);

    const char *encodedMessage = "hello\\|kitty\\\\|";
    for (size_t i = 0; i < strlen(encodedMessage); ++i) {
        smcoder.feed(encodedMessage[i]);
    }

    ASSERT_EQ((size_t)1, messages.size());
    const char *decodedMessage = "hello|kitty\\";
    ASSERT_EQ(strlen(decodedMessage), messages[0].size);
    ASSERT_EQ(0, memcmp(decodedMessage,
        messages[0].buffer, messages[0].size));
}

TEST(SMessageCoder, ShouldCallTheCallbackForEachMessage) {
    SMessageCoder smcoder;
    Messages messages;
    smcoder.onDecode(DecodeCallback, &messages);

    const char *encodedMessage = "hello|kitty|";
    for (size_t i = 0; i < strlen(encodedMessage); ++i) {
        smcoder.feed(encodedMessage[i]);
    }

    const char *decodedMessage[] = {
        "hello",
        "kitty"
    };

    ASSERT_EQ((size_t)2, messages.size());
    ASSERT_EQ(0, memcmp(decodedMessage[0],
        messages[0].buffer, strlen(decodedMessage[0])));
    ASSERT_EQ(0, memcmp(decodedMessage[1],
        messages[1].buffer, strlen(decodedMessage[1])));
}
