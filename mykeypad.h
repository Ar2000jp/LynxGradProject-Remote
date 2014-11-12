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
