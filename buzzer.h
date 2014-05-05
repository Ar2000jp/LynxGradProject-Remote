#ifndef BUZZER_H
#define BUZZER_H

#include "Arduino.h"

class Buzzer
{
public:
    enum BuzzerTone {ToneConstant = 0, Tone2Short, Tone50DC, ToneSalute};

    Buzzer();
    ~Buzzer();

    void activate(bool state, BuzzerTone tone = ToneConstant);
    void salute();
    void update();

private:
    static bool s_Initialized;

    static BuzzerTone s_BuzzerTone;
    static unsigned long s_ToneTime[4];
    static unsigned long s_PrevTime;
    static bool s_State;
    static byte s_ToneStage;

    static const unsigned int c_ToneSaluteOnTime1 = 10;
    static const unsigned int c_ToneSaluteOffTime1 = 300;
    static const unsigned int c_ToneSaluteOnTime2 = 10;
    static const unsigned int c_ToneSaluteOffTime2 = 680;

    static const unsigned int c_Tone2ShortOnTime1 = 100;
    static const unsigned int c_Tone2ShortOffTime1 = 100;
    static const unsigned int c_Tone2ShortOnTime2 = 100;
    static const unsigned int c_Tone2ShortOffTime2 = 700;

    static const unsigned int c_Tone50DCOnTime1 = 300;
    static const unsigned int c_Tone50DCOffTime1 = 300;

    static const byte c_nBuzzerPin = 12;
};

#endif
