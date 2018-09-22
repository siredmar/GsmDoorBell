#include "Sim800.h"

Sim800::Sim800()
    : rxPin(SIM800_DEFAULT_RX)
    , txPin(SIM800_DEFAULT_TX)
{
    mySerial = new SoftwareSerial(rxPin, txPin);
    mySerial->begin(57600);
}

Sim800::Sim800(int rx, int tx)
    : rxPin(rx)
    , txPin(tx)
{
    mySerial = new SoftwareSerial(rxPin, txPin);
    mySerial->begin(57600);
}

bool Sim800::init(String pin)
{
    Serial.print(F("Trying to find Sim800..."));
    sendCmd("AT+CMEE=1");
    while (!testAT())
    {
        delay(100);
        Serial.print(F("."));
    }
    Serial.println(F(" found"));

    Serial.print(F("Enabling network..."));
    functionMode(true);
    Serial.println(F(" done"));
    Serial.print(F("Unlocking SIM..."));
    setPin(pin);
    delay(5000);
    if (!checkPin())
    {
        Serial.println(F(" failed"));
    }
    Serial.println(F(" done"));

    Serial.print(F("Trying to find network..."));
    while (!networkStatus())
    {
        delay(1000);
        Serial.print(F("."));
    }
    Serial.println(F(" connected"));
    return true;
}

bool Sim800::testAT()
{
    return sendCmd("AT");
}

bool Sim800::networkStatus()
{
    sendCmd("AT+CREG?");
    return readResponse(",1");
}

bool Sim800::functionMode(bool enabled)
{
    String command;
    command = "AT+CFUN=";
    if (enabled)
    {
        command += "1";
    }
    else
    {
        command += "4";
    }

    mySerial->println(command);
    return readResponse("OK");
}

bool Sim800::sendSms(String number, String text)
{
    String _buffer;
    mySerial->print(F("AT+CMGF=1\r"));
    _buffer = readSerial();
    mySerial->print(F("AT+CMGS=\""));
    mySerial->print(number);
    mySerial->print(F("\"\r"));
    _buffer = readSerial();
    mySerial->print(text);
    mySerial->print(F("\r"));
    _buffer = readSerial();
    mySerial->print((char)26);
    _buffer = readSerial(60000);
    if ((_buffer.indexOf("OK")) == -1)
    {
        return false;
    }
    else
        return true;
}

bool Sim800::callNumber(String number)
{
    String command;
    command = "ATD";
    command += number;
    command += ";";

    mySerial->println(command);
    return readResponse("OK");
}

bool Sim800::hangupCall()
{
    sendCmd("ATH");
    return readResponse("OK");
}

bool Sim800::setPin(String pin)
{
    String command;
    command = "AT+CPIN=";
    command += pin;

    mySerial->println(command);
    return readResponse("OK");
}

bool Sim800::checkPin()
{
    String command;
    command = "AT+CPIN?";

    mySerial->println(command);
    return readResponse("READY");
}

String Sim800::readSerial()
{
    return readSerial(TIME_OUT_READ_SERIAL);
}

String Sim800::readSerial(uint32_t timeout)
{
    uint64_t timeOld = millis();

    while (!mySerial->available() && !(millis() > timeOld + timeout))
    {
        delay(13);
    }

    String str;

    while (mySerial->available())
    {
        if (mySerial->available() > 0)
        {
            str += (char)mySerial->read();
        }
    }

    return str;
}

bool Sim800::readResponse(String pattern)
{
    String str;
    str = readSerial();
#ifdef SIM800_DEBUG
    Serial.println(str);
#endif
    if (str.indexOf(pattern) == -1)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Sim800::sendCmd(String text)
{
#ifdef SIM800_DEBUG
    Serial.println(text);
#endif
    mySerial->println(text);
    delay(100);
    return true;
}