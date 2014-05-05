#ifndef RADIO_H
#define RADIO_H

#include <SPI.h>
#include <RH_RF22.h>
#include <RHReliableDatagram.h>

class Radio
{
public:
    Radio();
    ~Radio();

    void init();
    bool send(uint8_t* buf, uint8_t bufLen);
    bool recv(uint8_t* buf, uint8_t* bufLen);

private:
    static bool s_Initialized;

    //static uint8_t s_radioBuf[RH_RF22_MAX_MESSAGE_LEN];
    //static const uint8_t s_radioBufLen;
    //static uint16_t s_radioRecvTimeout;

    static const uint8_t c_RCAddress = 1;
    static const uint8_t c_CarAddress = 2;

    static RH_RF22 s_Driver;
    static RHReliableDatagram s_Manager;

};

#endif
