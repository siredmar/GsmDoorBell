#pragma once
#include "Configuration.h"
#include "Sim800.h"
#include <Arduino.h>

class Contact
{
public:
    Contact(Sim800* m, Configuration* configuration);
    bool init();
    bool sendSms(String number, String text);
    bool callNumber(String number, int seconds);
    void process();
    bool registerNumber(String number, bool call = true, int seconds = 0, bool sms = false);
    bool deleteRegistration(String number);
    void status(int index);
    void status();

private:
    Sim800* modem;
    Configuration* configuration;
    int findRegistrationIndex(String number);
};