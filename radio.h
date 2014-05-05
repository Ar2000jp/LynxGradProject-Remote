#ifndef RADIO_H
#define RADIO_H

#include <SPI.h>
#include <RH_RF22.h>
//#include <RHReliableDatagram.h>

class Radio
{
public:
    Radio();
    ~Radio();

    void init();
    int send(uint8_t* buf, uint8_t bufLen);
    int recv(uint8_t* buf, uint8_t bufLen);

private:
    static bool s_Initialized;

    static uint8_t s_radioBuf[RH_RF22_MAX_MESSAGE_LEN];
    static const uint8_t s_radioBufLen;
    static uint16_t s_radioRecvTimeout;
//uint8_t m_radioFrom = 0;
//uint8_t m_radioTo = 0;

    static const uint8_t c_RCAddress = 1;
    static const uint8_t c_CarAddress = 2;

    static RH_RF22 s_Driver;
    //static RHReliableDatagram s_Manager;

};

//const int spiNRadioSelect = 53;
//const int radioInterruptPin = 2;
//const int radioInterrupt = 0;

#endif
