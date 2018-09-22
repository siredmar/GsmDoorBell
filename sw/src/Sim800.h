
#pragma once
#include <Arduino.h>
#include <SoftwareSerial.h>

// #define SIM800_DEBUG

#define SIM800_DEFAULT_RX 8
#define SIM800_DEFAULT_TX 7

#define TIME_OUT_READ_SERIAL 5000
class Sim800
{
public:
    Sim800();
    Sim800(int rx, int tx);
    bool init(String pin);
    bool testAT();
    bool networkStatus();
    bool functionMode(bool enabled);
    bool sendSms(String number, String text);
    bool callNumber(String number);
    bool hangupCall();
    bool setPin(String pin);
    bool checkPin();

private:
    SoftwareSerial* mySerial;
    String readSerial();
    String readSerial(uint32_t timeout);
    bool sendCmd(String text);
    bool readResponse(String str);
    int rxPin;
    int txPin;
};