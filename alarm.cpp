#include "alarm.h"

Alarm::AlarmLevel Alarm::s_AlarmLevel = Alarm::AlarmOff;

Alarm::Alarm()
{

}

Alarm::~Alarm()
{

}

void Alarm::setLevel(Alarm::AlarmLevel level)
{
    s_AlarmLevel = level;
    switch (level) {
    case 0:
        m_LEDs.activate(LEDs::LEDRed, false);
        m_Buzzer.activate(false);
        break;

    case 1:
        m_LEDs.activate(m_LEDs.LEDs::LEDRed, true, LEDs::Blink2Short);
        m_Buzzer.activate(false);
        break;

    case 2:
        m_LEDs.activate(m_LEDs.LEDs::LEDRed, true, LEDs::Blink50DC);
        m_Buzzer.activate(false);
        break;

    case 3:
        m_LEDs.activate(m_LEDs.LEDs::LEDRed, true, LEDs::Blink50DC);
        m_Buzzer.activate(true, Buzzer::Tone2Short);
        break;

    case 4:
        m_LEDs.activate(m_LEDs.LEDs::LEDRed, true, LEDs::Blink50DC);
        m_Buzzer.activate(true, Buzzer::Tone50DC);
        break;

    case 5:
        m_LEDs.activate(m_LEDs.LEDs::LEDRed, true, LEDs::Blink50DC);
        m_Buzzer.activate(true, Buzzer::ToneConstant);
        break;

    default:
        Serial.println("Invalid alarmLevel.");
    }
}

Alarm::AlarmLevel Alarm::getLevel()
{
    return s_AlarmLevel;
}

void Alarm::update()
{
    m_Buzzer.update();
    m_LEDs.update();
}
