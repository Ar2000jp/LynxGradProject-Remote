#include "radio.h"

#include <Arduino.h>

RHReliableDatagram Radio::s_RadioManager(s_RadioDriver, c_RCAddress);
RHReliableDatagram Radio::s_SerialManager(s_SerialDriver, c_RCAddress);

RH_RF22 Radio::s_RadioDriver;
RH_Serial Radio::s_SerialDriver(Serial1);

bool Radio::s_Initialized = false;
byte Radio::s_CommSysID = 0;

Radio::Radio()
{

}

Radio::~Radio()
{

}

void Radio::init()
{
    if (s_Initialized == false) {
        s_Initialized = true;

        if (s_RadioManager.init()) {
            s_RadioManager.setTimeout(100);
            s_RadioManager.setRetries(5);

            // Use a reasonable comm. speed
            s_RadioDriver.setModemConfig(RH_RF22::FSK_Rb9_6Fd45);

            // Use max Tx power
            s_RadioDriver.setTxPower(RH_RF22_TXPOW_20DBM);
        } else {
            Serial.println("Radio init failed. Switching to Serial1.");
            s_CommSysID = 1;
        }

        if (s_SerialManager.init()) {
            s_SerialManager.setTimeout(100);
            s_SerialManager.setRetries(5);
        }
        else {
            Serial.println("Serial1 init failed.");
        }
    }
}

bool Radio::send(uint8_t* buf, uint8_t bufLen)
{
    if(s_CommSysID == 0) {
        if (!s_RadioManager.sendtoWait(buf, bufLen, c_CarAddress)) {
            Serial.println("sendtoWait failed");
            return false;
        } else {
            Serial.println("sendtoWait succeeded");
        }
    } else {
        if (!s_SerialManager.sendtoWait(buf, bufLen, c_CarAddress)) {
            Serial.println("sendtoWait failed");
            return false;
        } else {
            Serial.println("sendtoWait succeeded");
        }
    }

    return true;
}

bool Radio::recv(uint8_t* buf, uint8_t* bufLen)
{
    uint8_t from;

    if(s_CommSysID == 0) {
        if (s_RadioManager.available() < 1) {
            return false;
        }

        if (!s_RadioManager.recvfromAckTimeout(buf, bufLen, 300, &from)) {
            return false;
        }
    }
    else {
        if (s_SerialManager.available() < 1) {
            return false;
        }

        if (!s_SerialManager.recvfromAckTimeout(buf, bufLen, 300, &from)) {
            return false;
        }
    }

    Serial.print("got msg from : 0x");
    Serial.print(from, HEX);
    Serial.print(": ");
    Serial.println((char*)buf);

    if (from != c_CarAddress) {
        return false;
    }

    return true;
}

void Radio::switchCommSystem(byte sysID)
{
    s_CommSysID = sysID;
}
