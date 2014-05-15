#ifndef MYKEYPAD_H
#define MYKEYPAD_H

#include <Keypad.h>

#include "globals.h"

class MyKeypad
{
public:
    MyKeypad();
    ~MyKeypad();

    char getKey();

private:
// Keypad pins
    static const byte c_keypadCol1Pin = 27;
    static const byte c_keypadCol2Pin = 26;
    static const byte c_keypadCol3Pin = 25;
    static const byte c_keypadCol4Pin = 24;

    static const byte c_keypadRow1Pin = 30;
    static const byte c_keypadRow2Pin = 31;
    static const byte c_keypadRow3Pin = 32;
    static const byte c_keypadRow4Pin = 33;

    // Keypad size
    static const byte c_keypadRows = 4;
    static const byte c_keypadCols = 4;

    static const char c_hexaKeys[c_keypadRows][c_keypadCols];

    static const byte c_rowPins[c_keypadRows];
    static const byte c_colPins[c_keypadCols];

    static char s_LastKey;
    static char s_Key;

    static Keypad s_Keypad;
};

#endif
