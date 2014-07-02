#include "button-event-source.h"

static struct PinMapEntry{
    int pin;
    uint8_t player;
    uint8_t button;
} pin_to_button_map[] = { 
    {2, 0, JSBTN_UP},
    {3, 0, JSBTN_DOWN},
    {4, 0, JSBTN_LEFT},
    {5, 0, JSBTN_RIGHT},
    {6, 0, JSBTN_A},
    {7, 0, JSBTN_B},
    {8, 0, JSBTN_C},
    {9, 0, JSBTN_X},
    {A4, 0, JSBTN_Y},
    {A5, 0, JSBTN_Z},

    {A3, 1, JSBTN_UP},
    {A2, 1, JSBTN_DOWN},
    {A1, 1, JSBTN_LEFT},
    {A0, 1, JSBTN_RIGHT},
    {13, 1, JSBTN_A},
    {12, 1, JSBTN_B},
    {11, 1, JSBTN_C},
    {10, 1, JSBTN_X},
    {A6, 1, JSBTN_Y},
    {A7, 1, JSBTN_Z}
};

void ButtonEventSource::poll( unsigned int interval_ms /*= 5*/ )
{
    for (unsigned int i = 0; i < ARRAY_SIZE(pin_to_button_map); ++i) {
        PinMapEntry *ent = pin_to_button_map + i;
        int status = pinRead(ent->pin);
        if (status_table[i] != status) {
            status_table[i] = status;
            callback(ButtonEvent(ent->player, ent->button, status == LOW));
        }
    }
    delay(interval_ms);
}

ButtonEventSource::~ButtonEventSource()
{
    free(status_table);
}

ButtonEventSource::ButtonEventSource(ButtonEventCallback callback )
{
    this->callback = callback;
    status_table = (char *)malloc(ARRAY_SIZE(pin_to_button_map));
    memset(status_table, HIGH, ARRAY_SIZE(pin_to_button_map));
}

int ButtonEventSource::pinRead( int pin )
{
    if (pin == A6 || pin == A7) {
        // A6 and A7 can't use as a digital pin
        return analogRead(pin) > 256 ? HIGH : LOW;
    }
    else {
        return digitalRead(pin);
    }
}

void ButtonEventSource::setup()
{
    for (unsigned int i = 0; i < ARRAY_SIZE(pin_to_button_map); ++i) {
        int pin = pin_to_button_map[i].pin;

        if (pin >= MIN_ANALOG_PIN && pin <= MIN_ANALOG_PIN) {
            // set all analog pins to digital pull-up mode
            pinMode(pin, INPUT);
            digitalWrite(pin, HIGH);
        }
        else {
            pinMode(pin, INPUT_PULLUP);
        }
    }
}
