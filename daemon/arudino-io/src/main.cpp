#line 2 "src/sketch.ino"

#include <Arduino.h>

void setup();
void loop();

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define serial  Serial

#define MAX_STREAM_PRINTF_BUFFER_SIZE   128
void stream_printf(Stream &stream, const char *fmt, ...) {
    va_list vl;
    va_start(vl, fmt);
    char buf[MAX_STREAM_PRINTF_BUFFER_SIZE];
    vsnprintf(buf, sizeof(buf), fmt, vl);
    stream.print(buf);
    va_end(vl);
}
#define printf(fmt...)  stream_printf(serial, fmt)

#define MIN_DIGITAL_PIN     2
#define MAX_DIGITAL_PIN     12
#define DIGITAL_PINS_COUNT  (MAX_DIGITAL_PIN - MIN_DIGITAL_PIN + 1)
#define MIN_ANALOG_PIN      A0
#define MAX_ANALOG_PIN      A5
#define ANALOG_PINS_COUNT   (MAX_ANALOG_PIN - MIN_ANALOG_PIN + 1)
#define BUTTONS_COUNT       (DIGITAL_PINS_COUNT + ANALOG_PINS_COUNT)

class ButtonMap {
public:
    ButtonMap() {
        memset(button_to_pin_map,0 , sizeof(button_to_pin_map));
        memset(pin_to_button_map,0 , sizeof(pin_to_button_map));
    }

    void updateMap(unsigned int pin, unsigned int button) {
        printf("update %u => %u\r\n", pin, button);
        button_to_pin_map[button] = pin;
        pin_to_button_map[pin] = button;
    }

    unsigned int getButton(unsigned int pin) const{
        // no sanity check
        return pin_to_button_map[pin];
    }

    unsigned int getPin(unsigned int button) const {
        return button_to_pin_map[button];
    }

    unsigned int getButtonCount() const{
        return ARRAY_SIZE(button_to_pin_map);
    }

private:
    unsigned int pin_to_button_map[MAX_ANALOG_PIN + 1];
    unsigned int button_to_pin_map[BUTTONS_COUNT];
};

static ButtonMap *get_button_map() {
    static ButtonMap button_map;
    return &button_map;
}
// assume that analog pins are continuous ascending 
// bimap for button and pin



static void setup_serial() {
}

class ButtonEventSource {
public:
    typedef void(*ButtonEventCallback)(unsigned int button, bool pressed);
    ButtonEventSource(const ButtonMap *button_map, ButtonEventCallback callback) {
        this->callback = callback;
        
        this->button_map = button_map;
        status_table = (char *)malloc(button_map->getButtonCount());
        memset(status_table, HIGH, button_map->getButtonCount());
    }

    ~ButtonEventSource() {
        free(status_table);
    }

    void poll(unsigned int interval_ms = 5) {
        for (unsigned int btn = 0; btn < button_map->getButtonCount(); ++btn) {
            unsigned int status = digitalRead(button_map->getPin(btn));
            if (status_table[btn] != status) {
                status_table[btn] = status;
                // using the pull-up mode, LOW is button pressed
                callback(btn, status == LOW);
            }
        }
        delay(5);
    }

private:
    char *status_table;
    ButtonEventCallback callback;
    const ButtonMap *button_map;
};

class Application {
public:
    static Application *getInstance() {
        static Application app;
        return &app;
    }

    void setup() {
        setup_serial();
        setup_input();
    }

    void run() {
        event_source->poll();
    }

private:
    Application() {
        button_map = new ButtonMap();
        event_source = new ButtonEventSource(button_map, eventCallback);
    }


    void onButtonEvent(unsigned int button, bool pressed) {
        printf("button[%u] => %u\r\n", button, pressed);
    }

    static void eventCallback(unsigned int button, bool pressed) {
        getInstance()->onButtonEvent(button, pressed);
    }

    void setup_serial() {
        serial.begin(9600);
    }
    
    void setup_input() {
        unsigned int index = 0;
        for (unsigned int pin = MIN_DIGITAL_PIN; pin <= MAX_DIGITAL_PIN; ++pin) {
            pinMode(pin, INPUT_PULLUP);
            button_map->updateMap(pin, index++);
        }

        // set all analog pins to digital pull-up mode
        for (unsigned int pin = MIN_ANALOG_PIN; pin <= MAX_ANALOG_PIN; ++pin) {
            pinMode(pin, INPUT);
            digitalWrite(pin, HIGH);
            button_map->updateMap(pin, index++);
        }
    }

    ButtonMap *button_map;
    ButtonEventSource *event_source;
};

void setup()
{
    Application::getInstance()->setup();
}

void loop()
{
    Application::getInstance()->run();
}
