/*
 * This file is part of LynxGradProject.
 *
 * LynxGradProject is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LynxGradProject is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LynxGradProject.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "radio.h"

#include <Arduino.h>

#include "globals.h"

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
            s_RadioManager.setTimeout(200);
            s_RadioManager.setRetries(5);

            // Use a reasonable comm. speed
            s_RadioDriver.setModemConfig(RH_RF22::FSK_Rb9_6Fd45);

            // Use max Tx power
            s_RadioDriver.setTxPower(RH_RF22_TXPOW_20DBM);
        } else {
#ifdef DEBUG
            Serial.println("Radio init failed. Switching to Serial1.");
#endif
            s_CommSysID = 1;
        }

        if (s_SerialManager.init()) {
            s_SerialManager.setTimeout(100);
            s_SerialManager.setRetries(5);
        }
        else {
#ifdef DEBUG
            Serial.println("Serial1 init failed.");
#endif
        }
    }
}

bool Radio::send(uint8_t* buf, uint8_t bufLen)
{
    if(s_CommSysID == 0) {
        if (!s_RadioManager.sendtoWait(buf, bufLen, c_CarAddress)) {
#ifdef DEBUG
            Serial.println("sendtoWait failed");
#endif
            return false;
        } else {
#ifdef DEBUG
            Serial.println("sendtoWait succeeded");
#endif
        }
    } else {
        if (!s_SerialManager.sendtoWait(buf, bufLen, c_CarAddress)) {
#ifdef DEBUG
            Serial.println("sendtoWait failed");
#endif
            return false;
        } else {
#ifdef DEBUG
            Serial.println("sendtoWait succeeded");
#endif
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

#ifdef DEBUG
    Serial.print("got msg from : 0x");
    Serial.print(from, HEX);
    Serial.print(": ");
    Serial.println((char*)buf);
#endif

    if (from != c_CarAddress) {
        return false;
    }

    return true;
}

void Radio::switchCommSystem(byte sysID)
{
    s_CommSysID = sysID;
}
