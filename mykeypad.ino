#include <Keypad.h>

// Keypad pins
const byte c_keypadCol1Pin = 27;
const byte c_keypadCol2Pin = 26;
const byte c_keypadCol3Pin = 25;
const byte c_keypadCol4Pin = 24;

const byte c_keypadRow1Pin = 30;
const byte c_keypadRow2Pin = 31;
const byte c_keypadRow3Pin = 32;
const byte c_keypadRow4Pin = 33;

// Keypad size
const byte c_keypadRows = 4;
const byte c_keypadCols = 4;

const char c_hexaKeys[c_keypadRows][c_keypadCols] = {
    {'0', '1', '2', '3'},
    {'4', '5', '6', '7'},
    {'8', '9', 'A', 'B'},
    {'C', 'D', 'E', 'F'}
};

const byte c_rowPins[c_keypadRows] = {
    c_keypadRow1Pin, c_keypadRow2Pin, c_keypadRow3Pin, c_keypadRow4Pin
};
const byte c_colPins[c_keypadCols] = {
    c_keypadCol1Pin, c_keypadCol2Pin, c_keypadCol3Pin, c_keypadCol4Pin
};

Keypad G_Keypad = Keypad(makeKeymap(c_hexaKeys), (byte*)c_rowPins, (byte*)c_colPins, c_keypadRows, c_keypadCols);

char lastKey = 0;

char readKeypad()
{
    lastKey = G_Key;
    G_Key = G_Keypad.getKey();
    if (G_Key == NO_KEY) {
        return 0;
    }
    if (G_Key != lastKey) {
        Serial.print("Key: ");
        Serial.print(G_Key);
        return G_Key;
    }

    return 0;
}
