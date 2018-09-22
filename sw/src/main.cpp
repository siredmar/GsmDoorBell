#include "Configuration.h"
#include "Contact.h"
#include "Led.h"
#include "Sim800.h"
#include <Arduino.h>
#include <Cmd.h>

#define INPUT_PIN 2
#define LED_PIN 12
#define LED_BLINK_TIME_MS 1000

Sim800* modem;
Contact* contact;
Configuration* configuration;
Led led(LED_PIN, false);

void PrintHelp()
{
    Serial.println(F("GsmDoorBell - (c) 2018 - Armin Schlegel <armin.schlegel@gmx.de>"));
    Serial.println(F("Command\t\tDescription\t\t\tUsage"));
    Serial.println(F("help\t\tGet this help.\t\t\thelp"));
    Serial.println(F("status\t\tGet status information.\t\tstatus_information"));
    Serial.println(F("sim_pin\t\tSets SIM pin.\t\t\tsim_pin <pin>"));
    Serial.println(F("eeprom_reset\tResets the eeprom defaults.\teeprom_reset"));
    Serial.println(F("register\tRegisters number with flags. \tregister <number> <call> <seconds> <sms>"));
    Serial.println(F("delete\t\tdeletes registration. \t\tdelete <number>"));
    Serial.println(F("test\t\tTests processing.\t\ttest"));
    Serial.println(F("reset\t\tTriggers a software reset.\treset"));
}

void Reset(int arg_cnt, char** args)
{
    asm("jmp 0");
}

void Register(int arg_cnt, char** args)
{
    bool call = (bool)cmdStr2Num(args[2], 10);
    int seconds = (int)cmdStr2Num(args[3], 10);
    bool sms = (bool)cmdStr2Num(args[4], 10);
    contact->registerNumber(String(args[1]), call, seconds, sms);
}

void Delete(int arg_cnt, char** args)
{
    contact->deleteRegistration(String(args[1]));
}

void Help(int arg_cnt, char** args)
{
    PrintHelp();
}

void Status(int arg_cnt, char** args)
{
    Serial.print(F("EEPROM Init: "));
    Serial.println(configuration->initialized());
    Serial.print(F("SIM Pin: "));
    Serial.println(configuration->simPin());
    contact->status();
}

void SimPin(int arg_cnt, char** args)
{
    if (arg_cnt == 2)
    {
        Serial.println(args[1]);
        configuration->simPin(args[1]);
    }
    else
    {
        Serial.println(F("invalid arguments"));
    }
}

void Process(int arg_cnt, char** args)
{
    contact->process();
}

void EepromFactory(int arg_cnt, char** args)
{
    configuration->resetFactoryEeprom(1024);
}

void EepromReset(int arg_cnt, char** args)
{
    configuration->reset();
}

void setup()
{
    led.on();
    pinMode(INPUT_PIN, INPUT);
    digitalWrite(INPUT_PIN, HIGH);
    // Give the supply voltage time to settle!
    delay(5000);

    Serial.begin(115200);
    configuration = new Configuration();
    if (configuration->initialized() == 0xFF)
    {
        Serial.println(F("EEPROM is fresh -> initializing"));
        configuration->reset();
    }
    else
    {
        Serial.println(F("EEPROM properly initialized"));
        Serial.print(F("Sim Pin: "));
        Serial.println(configuration->simPin());
    }
    modem = new Sim800(6, 7);
    contact = new Contact(modem, configuration);
    contact->init();

    cmdInit(&Serial);
    cmdAdd("help", Help);
    cmdAdd("status", Status);
    cmdAdd("sim_pin", SimPin);
    cmdAdd("eeprom_reset", EepromReset);
    cmdAdd("register", Register);
    cmdAdd("delete", Delete);
    cmdAdd("test", Process);
    cmdAdd("reset", Reset);

    PrintHelp();
    Serial.print(F("CMD >> "));
    led.off();
}

void loop()
{
    static int loop = 0;
    cmdPoll();
    delay(100);

    if (modem->networkStatus() == false)
    {
        led.blink();
    }
    else
    {
        led.on();
    }

    if (digitalRead(INPUT_PIN) == LOW)
    {
        delay(100);
        Serial.print(millis());
        Serial.println(": Klingel");
        contact->process();
    }
}