#pragma once
#include <Arduino.h>

enum class BlinkState
{
    OFF,
    WAITING_FOR_ON,
    ON,
    WAITING_FOR_OFF
};

class Led
{
public:
    Led(int pin, bool activeLow, unsigned long blinkMs = 1000)
        : state(false)
        , pin(pin)
        , activeLow(activeLow)
        , blinkMs(blinkMs)
        , oldTime(0)
        , blinkState(BlinkState::ON)
    {
        pinMode(pin, OUTPUT);
    }

    void on()
    {
        state = true;
        if (activeLow)
        {
            digitalWrite(pin, LOW);
        }
        else
        {
            digitalWrite(pin, HIGH);
        }
    }

    void off()
    {
        state = false;
        if (activeLow)
        {
            digitalWrite(pin, HIGH);
        }
        else
        {
            digitalWrite(pin, LOW);
        }
    }

    void toggle()
    {
        if (state == false)
        {
            on();
        }
        else
        {
            off();
        }
    }

    void blink()
    {
        switch (blinkState)
        {
            case BlinkState::OFF:
                blinkState = BlinkState::WAITING_FOR_ON;
                oldTime = millis();
                off();
                break;

            case BlinkState::WAITING_FOR_ON:
                if (millis() - oldTime >= blinkMs)
                    blinkState = BlinkState::ON;
                break;

            case BlinkState::ON:
                blinkState = BlinkState::WAITING_FOR_OFF;
                oldTime = millis();
                on();
                break;

            case BlinkState::WAITING_FOR_OFF:
                if (millis() - oldTime >= blinkMs)
                    blinkState = BlinkState::OFF;
                break;
        }
    }

private:
    bool state;
    int pin;
    bool activeLow;
    unsigned long blinkMs;
    unsigned long oldTime;
    BlinkState blinkState;
};