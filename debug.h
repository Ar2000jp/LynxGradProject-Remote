#ifndef DEBUG_H
#define DEBUG_H

#include "leds.h"
#include "buzzer.h"
#include "alarm.h"
#include "mykeypad.h"
#include "myserial.h"

class Debug
{
public:
    Debug();
    ~Debug();

    void start();
    void printHelp();

private:
    static bool s_DebugMode;

    static LEDs s_LEDs;
    static Buzzer s_Buzzer;
    static Alarm s_Alarm;
    static MyKeypad s_Keypad;
};

#endif
