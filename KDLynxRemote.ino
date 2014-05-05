/*
 Team Lynx Graduation Project.
 Remote control code.
 */

#define CH_TIME_QUANTUM 0

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

Alarm G_Alarm;
Radio G_Radio;
MyKeypad G_Keypad;
Debug G_Debug;
Buzzer G_Buzzer;
LEDs G_LEDs;

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
    // Start Red LED thread
    chThdCreateStatic(waRedLEDThread, sizeof(waRedLEDThread),
                      NORMALPRIO + 1, RedLEDThread, NULL);

    chThdCreateStatic(waGreenLEDThread, sizeof(waGreenLEDThread),
                      NORMALPRIO + 1, GreenLEDThread, NULL);

    chThdCreateStatic(waBuzzerThread, sizeof(waBuzzerThread),
                      NORMALPRIO + 1, BuzzerThread, NULL);

    while (1) {
        Serial.print('*');

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

        // Check for messages from radio
        G_RadioBufLen = sizeof(G_RadioBuf);
        if (G_Radio.recv(G_RadioBuf, &G_RadioBufLen)) {
            switch (G_RadioBuf[0]) {
            case 'a':
                // Alarm! Next byte is alarm level
                G_Alarm.setLevel((Alarm::AlarmLevel)G_RadioBuf[1]);
                break;

            default:
                Serial.println("Invalid message.");
            }
        }

        // Read keypad
        // If a key gets pressed, we send it to the Car
        G_Key = G_Keypad.getKey();

        // If the 'F' key is pressed we lower the alarm level in the remote. Like a silence button.
        if (G_Key == 'F') {
            G_Alarm.lowerLevel(Alarm::AlarmAttention);

            //TODO: Add code to check up on alarms with car
        } else {
            G_Alarm.setLevel((Alarm::AlarmLevel)(G_Key - '0'));
            // First byte is 'c', which means command. 2nd byte is the key code.
            G_RadioBuf[0] = 'c';
            G_RadioBuf[1] = G_Key;
            G_LEDs.turnOn(LEDs::LEDGreen);
            if (!G_Radio.send(G_RadioBuf, 2)) {
                G_Alarm.raiseLevel(Alarm::AlarmRadioSignal);
            }
            G_LEDs.turnOff(LEDs::LEDGreen);
        }
    }
}

void setup()
{
    // Init Serial with PC
    Serial.begin(9600);
    Serial.setTimeout(100);

    // Init emulated radio serial
    Serial1.begin(9600);
    Serial1.setTimeout(100);

    if (CH_TIME_QUANTUM) {
        Serial.println("You must set CH_TIME_QUANTUM zero in");
        Serial.println("libraries/ChibiOS_AVR/utility/chconf.h");
        Serial.println("to enable cooperative scheduling.");
        while (1);
    }

    // Initialize Radio
    G_Radio.init();
    //SPI.setClockDivider(SPI_CLOCK_DIV128);

    // Welcome pattern
    G_Buzzer.salute();
    G_LEDs.setPattern(LEDs::LEDGreen, LEDs::Blink2ShortStop);

    Serial.println("Send 'd' to enter debug mode.");

    // start ChibiOS
    chBegin(mainThread);
}

void loop()
{
    // Never reached. Does nothing.
}
