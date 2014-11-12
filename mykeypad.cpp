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

#include "mykeypad.h"

const char MyKeypad::c_hexaKeys[c_keypadRows][c_keypadCols] = {
    {'0', '1', '2', '3'},
    {'4', '5', '6', '7'},
    {'8', '9', 'A', 'B'},
    {'C', 'D', 'E', 'F'}
};

const byte MyKeypad::c_rowPins[c_keypadRows] = {
    c_keypadRow1Pin, c_keypadRow2Pin, c_keypadRow3Pin, c_keypadRow4Pin
};
const byte MyKeypad::c_colPins[c_keypadCols] = {
    c_keypadCol1Pin, c_keypadCol2Pin, c_keypadCol3Pin, c_keypadCol4Pin
};

char MyKeypad::s_LastKey = 0;
char MyKeypad::s_Key = 0;

Keypad MyKeypad::s_Keypad = Keypad(makeKeymap(MyKeypad::c_hexaKeys), (byte*)MyKeypad::c_rowPins, (byte*)MyKeypad::c_colPins, MyKeypad::c_keypadRows, MyKeypad::c_keypadCols);

MyKeypad::MyKeypad()
{

}

MyKeypad::~MyKeypad()
{

}

char MyKeypad::getKey()
{
    s_LastKey = s_Key;
    s_Key = s_Keypad.getKey();
    if (s_Key == NO_KEY) {
        return 0;
    }

    if (s_Key != s_LastKey) {
        return s_Key;
    }

    return 0;
}
