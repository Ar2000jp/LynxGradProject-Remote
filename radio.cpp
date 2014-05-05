#include "radio.h"

RHReliableDatagram Radio::s_Manager(s_Driver, c_RCAddress);
RH_RF22 Radio::s_Driver;

bool Radio::s_Initialized = false;

//const uint8_t Radio::s_radioBufLen = sizeof(s_radioBuf);
//uint16_t Radio::s_radioRecvTimeout = 50;

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
        if (!s_Manager.init()) {
            Serial.println("Radio init failed");
        }

        // Use a reasonable comm. speed
        s_Driver.setModemConfig(RH_RF22::FSK_Rb9_6Fd45);

        // Use max Tx power
        s_Driver.setTxPower(RH_RF22_TXPOW_20DBM);
    }
}

bool Radio::send(uint8_t* buf, uint8_t bufLen)
{
    if (!s_Manager.sendtoWait(buf, bufLen, c_CarAddress)) {
        Serial.println("sendtoWait failed");
        return false;
    }

    return true;
}

bool Radio::recv(uint8_t* buf, uint8_t* bufLen)
{
    uint8_t from;
    if (s_Manager.available() < 1) {
        return false;
    }

    if (!s_Manager.recvfromAckTimeout(buf, bufLen, 300, &from)) {
        return false;
    }

    Serial.print("got msg from : 0x");
    Serial.print(from, HEX);
    Serial.print(": ");
    Serial.println((char*)buf);

    if (from != c_RCAddress) {
        return false;
    }

    return 0;
}
