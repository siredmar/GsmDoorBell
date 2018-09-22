#include "Configuration.h"
#include "EEPROMAnything.h"

void Configuration::reset()
{
    configuration.initialized = '1';

    configuration.energy.wattsTrigger = 0.0;
    configuration.energy.wattsRelease = 0.0;
    configuration.energy.wattsTimeout = 0;
    configuration.energy.emonCalibration = 0.0;

    configuration.sim.pin[0] = '\0';
    configuration.sim.enabled = false;
    configuration.sim.reserved[0] = '\0';

    sprintf(configuration.contacts.text, "I\'d like to inform you that i\'m done");
    configuration.contacts.numberOfRegisteredContacts = 0;

    for (int i = 0; i < (int)NumberOfContacts::MAX_NUMBER_OF_CONTACTS; i++)
    {
        configuration.contacts.contact[i].registered = false;
        configuration.contacts.contact[i].phoneNumber[0] = '\0';
        configuration.contacts.contact[i].sendSms = false;
        configuration.contacts.contact[i].call = false;
        configuration.contacts.contact[i].seconds = 10;
    }

    EEPROM_writeAnything(0, configuration);
}

void Configuration::resetFactoryEeprom(int size)
{
    for (int i = 0; i < size; i++)
    {
        EEPROM.write(i, 0xFF);
    }
}

char Configuration::initialized()
{
    EEPROM_readAnything(0, configuration);
    return configuration.initialized;
}

const char* Configuration::simPin()
{
    EEPROM_readAnything(0, configuration);
    return configuration.sim.pin;
}

void Configuration::simPin(const char* pin)
{
    memcpy(&configuration.sim.pin[0], pin, strlen(pin));
    configuration.sim.pin[strlen(pin)] = '\0';
    EEPROM_writeAnything(0, configuration);
}

bool Configuration::simEnabled()
{
    EEPROM_readAnything(0, configuration);
    return configuration.sim.enabled;
}

void Configuration::simEnabled(bool en)
{
    configuration.sim.enabled = true;
    EEPROM_writeAnything(0, configuration);
}

Configuration::contactType Configuration::readContact(int index)
{
    EEPROM_readAnything(0, configuration);
    return configuration.contacts.contact[index];
}

int Configuration::numberOfRegisteredContacts()
{
    EEPROM_readAnything(0, configuration);
    return configuration.contacts.numberOfRegisteredContacts;
}

void Configuration::numberOfRegisteredContacts(int number)
{
    configuration.contacts.numberOfRegisteredContacts = number;
    EEPROM_writeAnything(0, configuration);
}

void Configuration::text(const char* text)
{
    memcpy(configuration.contacts.text, text, strlen(text));
    configuration.contacts.text[strlen(text)] = '\0';
    EEPROM_writeAnything(0, configuration);
}

const char* Configuration::text()
{
    EEPROM_readAnything(0, configuration);
    return configuration.contacts.text;
}

void Configuration::contactRegistered(int index, bool flag)
{
    configuration.contacts.contact[index].registered = flag;
    EEPROM_writeAnything(0, configuration);
}

bool Configuration::contactRegistered(int index)
{
    EEPROM_readAnything(0, configuration);
    return configuration.contacts.contact[index].registered;
}

void Configuration::contactNumber(int index, const char* number)
{
    memcpy(configuration.contacts.contact[index].phoneNumber, number, strlen(number));
    configuration.contacts.contact[index].phoneNumber[strlen(number)] = '\0';
    EEPROM_writeAnything(0, configuration);
}

const char* Configuration::contactNumber(int index)
{
    EEPROM_readAnything(0, configuration);
    return configuration.contacts.contact[index].phoneNumber;
}

void Configuration::contactSmsFlag(int index, bool flag)
{
    configuration.contacts.contact[index].sendSms = flag;
    EEPROM_writeAnything(0, configuration);
}

bool Configuration::contactSmsFlag(int index)
{
    EEPROM_readAnything(0, configuration);
    return configuration.contacts.contact[index].sendSms;
}

void Configuration::contactCallFlag(int index, bool flag)
{
    configuration.contacts.contact[index].call = flag;
    EEPROM_writeAnything(0, configuration);
}

bool Configuration::contactCallFlag(int index)
{
    EEPROM_readAnything(0, configuration);
    return configuration.contacts.contact[index].call;
}

void Configuration::contactCallSeconds(int index, int seconds)
{
    configuration.contacts.contact[index].seconds = seconds;
    EEPROM_writeAnything(0, configuration);
}

int Configuration::contactCallSeconds(int index)
{
    EEPROM_readAnything(0, configuration);
    return configuration.contacts.contact[index].seconds;
}

void Configuration::energyEmonCalibration(double calib)
{
    configuration.energy.emonCalibration = calib;
    EEPROM_writeAnything(0, configuration);
}

double Configuration::energyEmonCalibration()
{
    EEPROM_readAnything(0, configuration);
    return configuration.energy.emonCalibration;
}

void Configuration::energyWattsTimeout(int seconds)
{
    configuration.energy.wattsTimeout = seconds;
    EEPROM_writeAnything(0, configuration);
}

int Configuration::energyWattsTimeout()
{
    EEPROM_readAnything(0, configuration);
    return configuration.energy.wattsTimeout;
}

void Configuration::energyWattsTrigger(double trigger)
{
    configuration.energy.wattsTrigger = trigger;
    EEPROM_writeAnything(0, configuration);
}

double Configuration::energyWattsTrigger()
{
    EEPROM_readAnything(0, configuration);
    return configuration.energy.wattsTrigger;
}

void Configuration::energyWattsRelease(double release)
{
    configuration.energy.wattsRelease = release;
    EEPROM_writeAnything(0, configuration);
}

double Configuration::energyWattsRelease()
{
    EEPROM_readAnything(0, configuration);
    return configuration.energy.wattsRelease;
}