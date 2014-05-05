#include "mykeypad.h"

bool MyKeypad::s_Initialized = false;

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
    if (s_Initialized == false) {
        s_Initialized = true;
        //Keypad s_Keypad = Keypad(makeKeymap(c_hexaKeys), (byte*)c_rowPins, (byte*)c_colPins, c_keypadRows, c_keypadCols);
    }
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
        Serial.print("Key: ");
        Serial.print(s_Key);
        return s_Key;
    }

    return 0;
}
