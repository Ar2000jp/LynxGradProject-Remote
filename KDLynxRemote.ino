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

/*
 Team Lynx Graduation Project.
 Remote control code.
 */


#include <stdint.h>

#include <Keypad.h>
#include <SPI.h>
#include <RH_RF22.h>
#include <RHReliableDatagram.h>
#include <ChibiOS_AVR.h>

#include "alarm.h"
#include "myserial.h"
#include "radio.h"
#include "mykeypad.h"
#include "debug.h"
#include "buzzer.h"
#include "leds.h"
#include "globals.h"
#include "mymutexes.h"

const unsigned int c_UpdateInterval = 10000;
unsigned long G_LastUpdateTime = 0;
byte G_LastAlarmID = 255;

Alarm G_Alarm;
Radio G_Radio;
MyKeypad G_Keypad;
Buzzer G_Buzzer;
LEDs G_LEDs;

#ifdef DEBUG
Debug G_Debug;
#endif

char G_Key = 0;
uint8_t G_RadioBuf[RH_RF22_MAX_MESSAGE_LEN];
uint8_t G_RadioBufLen = sizeof(G_RadioBuf);

static WORKING_AREA(waRedLEDThread, 64);

static msg_t RedLEDThread(void* arg)
{
    G_LEDs.run(LEDs::LEDRed);
    return 0;
}

static WORKING_AREA(waGreenLEDThread, 64);

static msg_t GreenLEDThread(void* arg)
{
    G_LEDs.run(LEDs::LEDGreen);
    return 0;
}

static WORKING_AREA(waBuzzerThread, 64);

static msg_t BuzzerThread(void* arg)
{
    G_Buzzer.run();
    return 0;
}

void mainThread()
{
    // Start threads
    chThdCreateStatic(waRedLEDThread, sizeof(waRedLEDThread),
                      NORMALPRIO, RedLEDThread, NULL);

    chThdCreateStatic(waGreenLEDThread, sizeof(waGreenLEDThread),
                      NORMALPRIO, GreenLEDThread, NULL);

    chThdCreateStatic(waBuzzerThread, sizeof(waBuzzerThread),
                      NORMALPRIO, BuzzerThread, NULL);

    while (1) {
#ifdef DEBUG
        Serial.print('M');
#endif

#ifdef DEBUG
        chThdYield();

        // Check for data on Serial
        if (Serial.available() > 0) {
            if (Serial.read() == 'd') {
                G_Debug.start();
            } else {
                // Clear buffer if 'd' is not received.
                drainSerial();
            }
        }
#endif

        chThdYield();

        // Check for messages from radio
        G_RadioBuf[0] = 0;
        G_RadioBuf[1] = 0;
        G_RadioBuf[2] = 0;
        G_RadioBuf[3] = 0;
        G_RadioBufLen = sizeof(G_RadioBuf);
        if(G_Radio.recv(G_RadioBuf, &G_RadioBufLen)) {
            if ((G_RadioBuf[0] == 's') && (G_RadioBuf[3] == 'm')) {
                // Car status message:
                // [0] = 's';
                // [1] = Alarm level;
                // [2] = Alarm ID;
                // [3] = 'm';
                G_LastUpdateTime = millis();
                G_LEDs.setPattern(LEDs::LEDGreen, LEDs::Blink2ShortStop);
                if (G_RadioBuf[2] != G_LastAlarmID) {
                    // New Alarm! Set Alarm level.
                    G_Alarm.setLevel((Alarm::AlarmLevel)G_RadioBuf[1]);
                    G_LastAlarmID = G_RadioBuf[2];
#ifdef DEBUG
                    Serial.println("New Alarm.");
#endif
                }
            } else {
#ifdef DEBUG
                Serial.println("Invalid message.");
#endif
            }
        }


        chThdYield();

        // Read keypad
        // If a key gets pressed, we send it to the Car
        G_Key = G_Keypad.getKey();

        // Do nothing if no key is pressed
        if (G_Key != 0) {
            if ((G_Key == 'F') || (G_Key == 'C') || (G_Key == 'D')) {
                // If the 'F', 'C', or 'D' key is pressed we lower the alarm level in the remote. Like a silence button.
                G_Alarm.lowerLevel(Alarm::AlarmAttention);
            }

            // First byte is 'c', which means command. 2nd byte is the key code.
            G_RadioBuf[0] = 'c';
            G_RadioBuf[1] = G_Key;
            G_LEDs.turnOn(LEDs::LEDGreen);
            if (!G_Radio.send(G_RadioBuf, 2)) {
                G_Alarm.raiseLevel(Alarm::AlarmRadioSignal);
            }
            G_LEDs.turnOff(LEDs::LEDGreen);
        }

        chThdYield();

        if (millis() > (G_LastUpdateTime + c_UpdateInterval)) {
            // Haven't received a status message in a while. Radio problems.
            G_Alarm.raiseLevel(Alarm::AlarmRadioSignal);
        }
        else {
            if(G_Alarm.getLevel() == Alarm::AlarmRadioSignal) {
                G_Alarm.setLevel(Alarm::AlarmOff);
            }
        }
    }
}

void setup()
{
    // Turn on onboard LED while initializing
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);

    // Init Serial with PC
    Serial.begin(115200);
    Serial.setTimeout(100);

    // Init backup comm. channel
    Serial1.begin(9600);
    Serial1.setTimeout(100);

    pinMode(40, INPUT_PULLUP);
    if(digitalRead(40) == false)
    {
        G_Radio.switchCommSystem(1);
    }

    // Initialize Radio
    G_Radio.init();

    // Welcome pattern
    G_Buzzer.salute();
    G_LEDs.setPattern(LEDs::LEDGreen, LEDs::Blink2ShortStop);
    G_LEDs.setPattern(LEDs::LEDRed, LEDs::Blink2ShortStop);

#ifdef DEBUG
    Serial.println("Send 'd' to enter debug mode.");
#endif

    // Done initializing. Turn off LED
    digitalWrite(13, LOW);

    // start ChibiOS
    chBegin(mainThread);
}

void loop()
{
    // Never reached. Does nothing.
}
