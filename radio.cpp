#include "radio.h"

//RHReliableDatagram Radio::s_Manager(s_Driver, c_RCAddress);
RH_RF22 Radio::s_Driver;

bool Radio::s_Initialized = false;

const uint8_t Radio::s_radioBufLen = sizeof(s_radioBuf);
uint16_t Radio::s_radioRecvTimeout = 50;

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
        if (!s_Driver.init()) {
            Serial.println("Radio init failed");
        }

        // Use a reasonable comm. speed
        s_Driver.setModemConfig(RH_RF22::FSK_Rb9_6Fd45);

	// Use max Tx power
	s_Driver.setTxPower(RH_RF22_TXPOW_20DBM);
    }
}

int Radio::send(uint8_t* buf, uint8_t bufLen)
{

    return 0;
}

int Radio::recv(uint8_t* buf, uint8_t bufLen)
{
available();
    return 0;
}
