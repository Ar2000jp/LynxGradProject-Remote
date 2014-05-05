/*
 Team Lynx Graduation Project.
 Remote control code.
 */

#include <Keypad.h>

#include "alarm.h"
#include "myserial.h"
//#include "radio.h"
#include "mykeypad.h"
#include "debug.h"
#include "buzzer.h"
#include "leds.h"

//const int spiMISO = 50;
//const int spiMOSI = 51;
//const int spiSCK = 52;

// Function declarations
//char readKeypad();

// Flags

// Variables
//MySerial G_MySerial;
Alarm G_Alarm;
//Radio G_Radio;
MyKeypad G_Keypad;
//Debug G_Debug;
Buzzer G_Buzzer;
LEDs G_LEDs;

char G_Key = 0;

void setup()
{
    // Init Serial with PC
    Serial.begin(9600);
    Serial.setTimeout(100);

    // Init emulated radio serial
    Serial1.begin(9600);
    Serial1.setTimeout(100);

    // Initialize Radio


    // Welcome pattern
    G_Buzzer.salute();
    G_LEDs.setPattern(LEDs::LEDGreen, LEDs::Blink2ShortStop);

    Serial.println("Send 'd' to enter debug mode.");
}

void loop()
{
    // Check for data on Serial
    if (Serial.available() > 0) {
        if (Serial.read() == 'd') {
//            G_Debug.start();
        } else {
            // Clear buffer if 'd' is not received.
            drainSerial();
        }
    }

    /*
    // Check for messages from radio
     if(manager.recvfromAckTimeout(radioBuf, &radioBufLen, radioRecvTimeout, &radioFrom, &radioTo)){
     Serial.print("Got msg: ");
     Serial.println((char*)radioBuf);
     Serial.print("From: ");
     Serial.println(radioFrom);
     Serial.print("To: ");
     Serial.println(radioTo);

     if(radioFrom == SERVER_ADDRESS && radioTo == CLIENT_ADDRESS){
     switch(radioBuf[0]){
     case 'a':
     // Alarm! Next byte is alarm level
     raiseAlarm(radioBuf[1]);
     break;

     default:
     Serial.println("Invalid message.");
     }
     }
     else{
     Serial.println("Msg not for us.");
     }
     }
     else{
     //Serial.println("recvfromAckTimeout failed");
     }*/

    // Read keypad
    // If a key gets pressed, we send it to the Car
    if (G_Keypad.getKey() != 0) {
        // If the 'F' key is pressed we lower the alarm level in the remote. Like a silence button.
        if (G_Key == 'F') {
            if (G_Alarm.getLevel() > Alarm::AlarmAttention) {
                G_Alarm.setLevel(Alarm::AlarmAttention);
            }

            //Add code to check up on alarms with car
        }        else {
            G_Alarm.setLevel((Alarm::AlarmLevel)(G_Key - '0'));
            // First byte is 'c', which means command. 2nd byte is the key code.
            //radioBuf[0] = 'c';
            //radioBuf[1] = key;
            G_LEDs.turnOn(LEDs::LEDGreen);
            //if (!manager.sendtoWait(radioBuf, 2, SERVER_ADDRESS))
            {
                //Serial.println("sendtoWait failed");
                //activateGreenLED(false);
                //activateRedLED(true);
            }
        }
    }

    //G_Buzzer.update();
    //G_LEDs.update();
    G_Alarm.update();

    Serial.println("Hi");
}


//bool sendtoWait(uint8_t* buf, uint8_t len, uint8_t address);
//bool recvfromAck(uint8_t* buf, uint8_t* len, uint8_t* from = NULL, uint8_t* to = NULL, uint8_t* id = NULL, uint8_t* flags = NULL);
//bool recvfromAckTimeout(uint8_t* buf, uint8_t* len,  uint16_t timeout, uint8_t* from = NULL, uint8_t* to = NULL, uint8_t* id = NULL, uint8_t* flags = NULL);
