#include "Contact.h"

Contact::Contact(Sim800* m, Configuration* configuration)
    : modem(m)
    , configuration(configuration)
{
}

bool Contact::init()
{
    Serial.println(configuration->simPin());
    if (!modem->init(String(configuration->simPin())))
    {
        return false;
    }
    return true;
}

bool Contact::sendSms(String number, String text)
{
    return modem->sendSms(number, text);
}

bool Contact::callNumber(String number, int seconds)
{
    Serial.print("Calling number ");
    Serial.println(number);
    if (!modem->callNumber(number))
    {
        return false;
    }
    if (seconds > 30)
    {
        seconds = 30;
    }
    Serial.print("Waiting ");
    Serial.print(seconds);
    Serial.println(" seconds");
    // giving it some extra time to make the actual call
    seconds += 3;
    delay(seconds * 1000);
    Serial.println("Hangup call");
    if (!modem->hangupCall())
    {
        return false;
    }
    return true;
}

void Contact::process()
{
    Serial.println(F("Processing registrations..."));
    for (int i = 0; i < (int)NumberOfContacts::MAX_NUMBER_OF_CONTACTS; i++)
    {
        if (configuration->contactRegistered(i))
        {
            Configuration::contactType c = configuration->readContact(i);
            if (configuration->contactSmsFlag(i))
            {
                if (!sendSms(String(c.phoneNumber), String(configuration->text())))
                {
                    Serial.print(F("Sending SMS failed for number "));
                }
                else
                {
                    Serial.print(F("Sent SMS to number "));
                }
                Serial.println(c.phoneNumber);
            }

            if (configuration->contactCallFlag(i))
            {
                if (!callNumber(String(c.phoneNumber), c.seconds))
                {
                    Serial.print(F("Calling failed for number "));
                }
                else
                {
                    Serial.print(F("Called number "));
                }
                Serial.println(c.phoneNumber);
            }
            delay(500);
        }
    }
    Serial.println("processing done");
}

bool Contact::registerNumber(String number, bool call = true, int seconds = 0, bool sms = false)
{
    Serial.println(number);
    Serial.println(call);
    Serial.println(seconds);
    Serial.println(sms);
    int registeredContacts = configuration->numberOfRegisteredContacts();
    Serial.print(F("registeredContacts: "));
    Serial.println(registeredContacts);
    if (registeredContacts < (int)NumberOfContacts::MAX_NUMBER_OF_CONTACTS)
    {
        int index = findRegistrationIndex(number);
        Serial.print(F("Index: "));
        Serial.println(index);
        if (index == -1)
        {
            index = registeredContacts;
            registeredContacts++;
            configuration->numberOfRegisteredContacts(registeredContacts);
        }

        configuration->contactRegistered(index, true);
        configuration->contactNumber(index, number.c_str());
        configuration->contactSmsFlag(index, sms);
        configuration->contactCallFlag(index, call);
        configuration->contactCallSeconds(index, seconds);
        return true;
    }
    else
    {
        Serial.println(F("Already configured max numbers"));
        return false;
    }
    registeredContacts = configuration->numberOfRegisteredContacts();
    Serial.print(F("registeredContacts: "));
    Serial.println(registeredContacts);
}

bool Contact::deleteRegistration(String number)
{
    int registeredContacts = configuration->numberOfRegisteredContacts();
    int index = findRegistrationIndex(number);
    Serial.print(F("Index: "));
    Serial.println(index);
    if (index != -1)
    {
        configuration->contactRegistered(index, false);
        configuration->contactNumber(index, "");
        configuration->contactSmsFlag(index, false);
        configuration->contactCallFlag(index, false);
        configuration->contactCallSeconds(index, 10);
        configuration->numberOfRegisteredContacts(--registeredContacts);
        return true;
    }
    else
    {
        return true;
        Serial.println(F("Number not registered"));
        return false;
    }
}

void Contact::status(int index)
{
    Configuration::contactType c = configuration->readContact(index);
    Serial.print(F("Registered: "));
    Serial.println(c.registered ? F("true") : F("false"));
    Serial.print(F("Number: "));
    Serial.println(c.phoneNumber);
    Serial.print(F("SMS: "));
    Serial.println(c.sendSms ? F("true") : F("false"));
    Serial.print(F("Call: "));
    Serial.println(c.call ? F("true") : F("false"));
    Serial.print(F("Seconds: "));
    Serial.println(c.seconds);
    Serial.println(F("-------------------"));
}

void Contact::status()
{
    Serial.print(F("SIM status: "));
    Serial.println(modem->networkStatus() ? F("connected") : F("not connected"));
    Serial.print(F("registeredContacts: "));
    Serial.println(configuration->numberOfRegisteredContacts());
    Serial.print(F("Text: "));
    Serial.println(configuration->text());
    Serial.println(F("-------------------"));
    for (int i = 0; i < (int)NumberOfContacts::MAX_NUMBER_OF_CONTACTS; i++)
    {
        status(i);
    }
}

int Contact::findRegistrationIndex(String number)
{
    for (int i = 0; i < (int)NumberOfContacts::MAX_NUMBER_OF_CONTACTS; i++)
    {
        Serial.print("i: ");
        Serial.println(i);
        Configuration::contactType c = configuration->readContact(i);
        Serial.print("number: ");
        Serial.println(number);
        Serial.print("c.phoneNumber: ");
        Serial.println(c.phoneNumber);
        if (strcmp(number.c_str(), c.phoneNumber) == 0)
        {
            Serial.print("Numbers match: ");
            Serial.println(i);
            return i;
        }
    }
    return -1;
}