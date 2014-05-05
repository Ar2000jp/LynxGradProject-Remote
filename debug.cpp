#include "debug.h"

bool Debug::s_DebugMode = false;

LEDs Debug::s_LEDs;
Buzzer Debug::s_Buzzer;
Alarm Debug::s_Alarm;
MyKeypad Debug::s_Keypad;

Debug::Debug()
{

}

Debug::~Debug()
{

}

void Debug::start()
{
    byte level = 0;
    char readChar = 0;
    s_DebugMode = true;

    // Print help
    printHelp();

    while (s_DebugMode == true) {
        readChar = 0;
        level = 0;

        // Process data from Serial
        if (Serial.available() < 1) {
            continue;
        }

        byte inChar = Serial.read();
        switch (inChar) {
        case 'h':
            printHelp();
            break;

        case 'a':
            level = Serial.parseInt();
            s_Alarm.setLevel((Alarm::AlarmLevel)level);
            Serial.print("Setting Alarm to level ");
            Serial.println(level);
            break;

        case 'b':
            level = Serial.parseInt();
            s_Buzzer.setTone((Buzzer::BuzzerTone)level);
            Serial.print("Setting Buzzer to tone ");
            Serial.println(level);
            break;

        case 'r':
            level = Serial.parseInt();
            s_LEDs.setPattern(LEDs::LEDRed, (LEDs::BlinkType)level);
            Serial.println("Setting Red LED to pattern ");
            Serial.println(level);
            break;

        case 'g':
            level = Serial.parseInt();
            s_LEDs.setPattern(LEDs::LEDGreen, (LEDs::BlinkType)level);
            Serial.println("Setting Green LED to pattern ");
            Serial.println(level);
            break;

        case 'k':
            Serial.println("Reading Keypad. Press any key on Keypad.");
            while (readChar == 0) {
                readChar = s_Keypad.getKey();
            }
            Serial.print("Key pressed is: ");
            Serial.println(readChar);
            break;

        case 'q':
            Serial.println("Exiting debug mode.");
            s_DebugMode = false;
            break;

        default:
            Serial.println("Invalid command");
        }

        // Drain buffer before looping.
        drainSerial();
    }
}

void Debug::printHelp()
{
    Serial.println("Debug mode:");
    Serial.println("Command\tAction");
    Serial.println("h\tPrint help");
    Serial.println("a#\tSet Alarm to level #");
    Serial.println("b#\tBuzz buzzer using level #");
    Serial.println("g#\tBlink Green LED using level #");
    Serial.println("r#\tBlink Red LED using level #");
    Serial.println("k\tRead Keypad");
    Serial.println("q\tExit debug mode");
}
