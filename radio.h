#ifndef RADIO_H
#define RADIO_H

#include <SPI.h>
#include <RH_RF22.h>
#include <RH_Serial.h>
#include <RHReliableDatagram.h>

class Radio
{
public:
    Radio();
    ~Radio();

    void init();
    bool send(uint8_t* buf, uint8_t bufLen);
    bool recv(uint8_t* buf, uint8_t* bufLen);
    void switchCommSystem(byte sysID);

private:
    static bool s_Initialized;
    static byte s_CommSysID;

    static const uint8_t c_RCAddress = 1;
    static const uint8_t c_CarAddress = 2;

    static RH_RF22 s_RadioDriver;
    static RH_Serial s_SerialDriver;
    static RHReliableDatagram s_RadioManager;
    static RHReliableDatagram s_SerialManager;

};

#endif
