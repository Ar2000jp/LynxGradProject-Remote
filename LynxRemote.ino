/*
 Team Lynx Graduation Project.
 Remote control code.
 */

// Pin definitions
const int nBuzzer = 12;
const int nRedLED = 10;
const int nGreenLED = 8;

const int spiMISO = 50;
const int spiMOSI = 51;
const int spiSCK = 52;
const int spiNRadioSelect = 53;

const int radioInterruptPin = 2;
const int radioInterrupt = 0;

const int keypadCol1 = 27;
const int keypadCol2 = 26;
const int keypadCol3 = 25;
const int keypadCol4 = 24;

const int keypadRow1 = 30;
const int keypadRow2 = 31;
const int keypadRow3 = 32;
const int keypadRow4 = 33;

// Flags
int debugMode = 0;

// Variables
char charBuf[8];

void activateBuzzer(bool state){
  digitalWrite(nBuzzer, !state);
}

void activateGreenLED(bool state){
  digitalWrite(nGreenLED, !state);
}

void activateRedLED(bool state){
  digitalWrite(nRedLED, !state);
}

void checkSerial(){
  if(Serial.available() > 0){
    if(Serial.read() == 'd')
    {
      // Debug mode. Print banner.
      Serial.println("Debug mode:");
      Serial.println("Command\tAction");
      Serial.println("b\tBuzz");
      Serial.println("r\tBlink red LED");
      Serial.println("g\tBlink green LED");
      Serial.println("q\tExit debug mode");

      debugMode = 1;

      while (debugMode == 1){
        // Wait for data from Serial
        while(Serial.available() < 1){
        }

        byte inChar = Serial.read();
        switch(inChar){
        case 'b':
          Serial.println("Buzzing buzzer.");
          activateBuzzer(true);
          delay(100);
          activateBuzzer(false);
          break;

        case 'r':
          Serial.println("Blinking Red LED");
          activateRedLED(true);
          delay(100);
          activateRedLED(false);
          break;

        case 'g':
          Serial.println("Blinking Green LED");
          activateGreenLED(true);
          delay(100);
          activateGreenLED(false);
          break;

        case 'q':
          Serial.println("Exiting debug mode.");
          debugMode = 0;
          break;

        default:
          Serial.println("Invalid command");
        }

        // Drain buffer before looping.
        drainSerialBuffer();
      }
    }
    else
    {
      // Clear buffer if 'd' is not received.
      drainSerialBuffer();
    }
  }
}

void drainSerialBuffer(){
  char dummyBuf[8];
  while(Serial.available() > 0){
    Serial.readBytes(dummyBuf, 8);
  }
}

void setup(){
  // Init Serial with PC
  Serial.begin(9600);
  Serial.setTimeout(100);

  // Get pins into the correct mode
  pinMode(nBuzzer, OUTPUT);
  digitalWrite(nBuzzer, HIGH);

  pinMode(nRedLED, OUTPUT);
  digitalWrite(nRedLED, HIGH);

  pinMode(nGreenLED, OUTPUT);
  digitalWrite(nGreenLED, HIGH);

  pinMode(spiMISO, INPUT);

  pinMode(spiMOSI, OUTPUT);
  digitalWrite(spiMOSI, LOW);

  pinMode(spiSCK, OUTPUT);
  digitalWrite(spiSCK, LOW);

  pinMode(spiNRadioSelect, OUTPUT);
  digitalWrite(spiNRadioSelect, LOW);

  pinMode(radioInterruptPin, INPUT);

  pinMode(keypadCol1, INPUT);
  pinMode(keypadCol2, INPUT);
  pinMode(keypadCol3, INPUT);
  pinMode(keypadCol4, INPUT);

  pinMode(keypadRow1, INPUT);
  pinMode(keypadRow2, INPUT);
  pinMode(keypadRow3, INPUT);
  pinMode(keypadRow4, INPUT);

  // Buzzer salute
  activateBuzzer(true);
  delay(10);
  activateBuzzer(false);
  delay(300);

  activateBuzzer(true);
  delay(10);
  activateBuzzer(false);

  Serial.println("Send 'd' to enter debug mode.");
}

void loop(){
  // Check for data on Serial
  checkSerial();
}


