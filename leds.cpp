#include "leds.h"

bool LEDs::s_Initialized = false;

LEDs::BlinkType LEDs::s_BlinkType[c_LEDCount] = {BlinkNone, BlinkNone};
unsigned long LEDs::s_BlinkTime[c_LEDCount][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}};
unsigned long LEDs::s_PrevTime[c_LEDCount] = {0, 0};
bool LEDs::s_State[c_LEDCount] = {false, false};
byte LEDs::s_BlinkStage[c_LEDCount] = {0, 0};

const byte LEDs::c_nLEDPin[c_LEDCount] = {10, 8};

LEDs::LEDs()
{
    if (s_Initialized == false) {
        s_Initialized = true;

        for (byte index = 0; index < c_LEDCount; index++) {
            pinMode(c_nLEDPin[index], OUTPUT);
            activate((LEDColor)index, false);
        }
    }
}

LEDs::~LEDs()
{

}

void LEDs::activate(LEDs::LEDColor color, bool state, LEDs::BlinkType blinkType)
{
    if (state == false) {
        s_BlinkType[color] = BlinkNone;
        s_State[color] = false;
    } else if (blinkType == BlinkNone) {
        s_BlinkType[color] = BlinkNone;
        s_State[color] = true;
    } else if (blinkType == Blink2Short) {
        s_BlinkType[color] = Blink2Short;
        s_BlinkTime[color][0] = c_Blink2ShortOnTime1;
        s_BlinkTime[color][1] = c_Blink2ShortOffTime1;
        s_BlinkTime[color][2] = c_Blink2ShortOnTime2;
        s_BlinkTime[color][3] = c_Blink2ShortOffTime2;
        s_BlinkStage[color] = 0;
        s_PrevTime[color] = 0;
        s_State[color] = true;
    } else if (blinkType == Blink50DC) {
        s_BlinkType[color] = Blink50DC;
        s_BlinkTime[color][0] = c_Blink50DCOnTime1;
        s_BlinkTime[color][1] = c_Blink50DCOffTime1;
        s_BlinkTime[color][2] = c_Blink50DCOnTime1;
        s_BlinkTime[color][3] = c_Blink50DCOffTime1;
        s_BlinkStage[color] = 0;
        s_PrevTime[color] = 0;
        s_State[color] = true;
    } else if (blinkType == Blink2ShortStop) {
        s_BlinkType[color] = Blink2ShortStop;
        s_BlinkTime[color][0] = c_Blink2ShortStopOnTime1;
        s_BlinkTime[color][1] = c_Blink2ShortStopOffTime1;
        s_BlinkTime[color][2] = c_Blink2ShortStopOnTime2;
        s_BlinkTime[color][3] = c_Blink2ShortStopOffTime2;
        s_BlinkStage[color] = 0;
        s_PrevTime[color] = 0;
        s_State[color] = true;
    }

    digitalWrite(c_nLEDPin[color], !s_State[color]);
}

void LEDs::update()
{
    unsigned long currentTime = millis();

    for (byte index = 0; index < c_LEDCount; index++) {
        if (s_BlinkType[index] != BlinkNone) {
            if (currentTime >= (s_PrevTime[index] + s_BlinkTime[index][s_BlinkStage[index]])) {
                s_State[index] = !s_State[index];
                s_BlinkStage[index]++;
                s_PrevTime[index] = currentTime;
            }

            if (s_BlinkStage[index] > 3) {
                if (s_BlinkType[index] == Blink2ShortStop) {
                    activate((LEDColor)index, false);
                }
                s_BlinkStage[index] = 0;
            }
        }

        digitalWrite(c_nLEDPin[index], !s_State[index]);
    }
}
