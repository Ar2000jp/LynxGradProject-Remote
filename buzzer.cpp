#include "buzzer.h"

bool Buzzer::s_Initialized = false;

Buzzer::BuzzerTone Buzzer::s_BuzzerTone = Buzzer::ToneConstant;
unsigned long Buzzer::s_ToneTime[4] = {0, 0, 0, 0};
unsigned long Buzzer::s_PrevTime = 0;
byte Buzzer::s_ToneStage = 0;
bool Buzzer::s_State = false;

Buzzer::Buzzer()
{
    if (s_Initialized == false) {
        s_Initialized = true;
        pinMode(c_nBuzzerPin, OUTPUT);
        activate(false);
    }
}

Buzzer::~Buzzer()
{

}

void Buzzer::activate(bool state, Buzzer::BuzzerTone tone)
{
    if (state == false) {
        s_BuzzerTone = ToneConstant;
        s_State = false;
        digitalWrite(c_nBuzzerPin, !s_State);
    } else if (tone == ToneConstant) {
        s_BuzzerTone = ToneConstant;
        s_State = true;
        digitalWrite(c_nBuzzerPin, !s_State);
    } else if (tone == Tone2Short) {
        s_BuzzerTone = Tone2Short;
        s_ToneTime[0] = c_Tone2ShortOnTime1;
        s_ToneTime[1] = c_Tone2ShortOffTime1;
        s_ToneTime[2] = c_Tone2ShortOnTime2;
        s_ToneTime[3] = c_Tone2ShortOffTime2;
        s_ToneStage = 0;
        s_PrevTime = 0;
        s_State = true;
        digitalWrite(c_nBuzzerPin, !s_State);
    } else if (tone == Tone50DC) {
        s_BuzzerTone = Tone50DC;
        s_ToneTime[0] = c_Tone50DCOnTime1;
        s_ToneTime[1] = c_Tone50DCOffTime1;
        s_ToneTime[2] = c_Tone50DCOnTime1;
        s_ToneTime[3] = c_Tone50DCOffTime1;
        s_ToneStage = 0;
        s_PrevTime = 0;
        s_State = true;
        digitalWrite(c_nBuzzerPin, !s_State);
    } else if (tone == ToneSalute) {
        s_BuzzerTone = ToneSalute;
        s_ToneTime[0] = c_ToneSaluteOnTime1;
        s_ToneTime[1] = c_ToneSaluteOffTime1;
        s_ToneTime[2] = c_ToneSaluteOnTime2;
        s_ToneTime[3] = c_ToneSaluteOffTime2;
        s_ToneStage = 0;
        s_PrevTime = 0;
        s_State = true;
        digitalWrite(c_nBuzzerPin, !s_State);
    }
}

void Buzzer::salute()
{
    activate(true, ToneSalute);
}

void Buzzer::update()
{
    if (s_BuzzerTone == ToneConstant) {
        return;
    }

    unsigned long currentTime = millis();
    if (currentTime >= (s_PrevTime + s_ToneTime[s_ToneStage])) {
        s_State = !s_State;
        s_ToneStage++;
        s_PrevTime = currentTime;
    }

    if (s_ToneStage > 3) {
        if (s_BuzzerTone == ToneSalute) {
            activate(false);
        }
        s_ToneStage = 0;
    }

    digitalWrite(c_nBuzzerPin, !s_State);
}
