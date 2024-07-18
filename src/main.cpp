#define IR_USE_AVR_TIMER1

#include <StandardCplusplus.h>
#include <Arduino.h>
#include <IRremote.h>
#include <vector>
#include <LiquidCrystal.h>

unsigned long currentMillis;
unsigned long previousMillis = 0;
unsigned long previousMillisLCD = 0;

const int RECV_PIN = 9;
std::vector<int> code;
const int combination = 2749;

int doorPin = 8;
bool reedState;

bool opened = false;

// notes in the melody:
int alarmPin = 10;
int melody[]{530, 2000};
int currentFrequency = 0;
size_t melodySize = sizeof(melody) / sizeof(melody[0]);
unsigned int toneDuration = 500;

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
int refreshSpan = 100;

int VCC_ControlPin = 11;
int GND_ControlPin = 12;

void printCode(int currentMillis)
{
  if (currentMillis - previousMillisLCD >= refreshSpan)
  {
    lcd.setCursor(0, 0);
    previousMillisLCD = currentMillis;
    if (code.size() == 1)
      lcd.print(code[0]);
    else if (code.size() == 2)
      lcd.print(code[0] * 10 + code[1]);
    else if (code.size() == 3)
      lcd.print(code[0] * 100 + code[1] * 10 + code[2]);
    else if (code.size() == 4)
      lcd.print(code[0] * 1000 + code[1] * 100 + code[2] * 10 + code[3]);
  }
}

void setup()
{
  pinMode(doorPin, INPUT);

  IrReceiver.begin(RECV_PIN);

  lcd.begin(0, 0);
  digitalWrite(VCC_ControlPin, LOW);
  digitalWrite(GND_ControlPin, LOW);
}

void loop()
{
  currentMillis = millis();

  reedState = digitalRead(doorPin);

  printCode(currentMillis);

  // N.C. Reed Switch
  if (opened)
  {
    // Ton abspielen
    if (currentMillis - previousMillis >= toneDuration)
    {
      previousMillis = currentMillis;
      tone(alarmPin, melody[currentFrequency], toneDuration);
      currentFrequency = currentFrequency == 0 ? 1 : 0;
    }

    // IR-Signal empfangen
    if (reedState && IrReceiver.decode())
    {
      if (!digitalRead(GND_ControlPin))
      {
        digitalWrite(VCC_ControlPin, HIGH);
        digitalWrite(GND_ControlPin, HIGH);
      }
      // Print Code in HEX
      switch (IrReceiver.decodedIRData.decodedRawData)
      {
      case 0xBA45FF00:
      {
        int codeNumber = code.size() == 4 ? code[0] * 1000 + code[1] * 100 + code[2] * 10 + code[3] : 0;
        if (code.size() == 4 && codeNumber == combination && reedState)
        {
          opened = false;
          digitalWrite(VCC_ControlPin, LOW);
          digitalWrite(GND_ControlPin, LOW);

          if (toneDuration != 500)
          {
            toneDuration = 500;
            melody[0] -= 300;
            melody[1] -= 500;
          }
        }

        else if (code.size() == 4 && codeNumber != combination)
        {
          toneDuration = 250;
          melody[0] += 300;
          melody[1] += 500;
          code.clear();
        }
        break;
      }
      case 0xBB44FF00:
        if (code.size() != 0)
          code.clear();
        lcd.clear();
        break;
      case 0xE916FF00:
        if (code.size() != 4)
          code.push_back(0);
        break;
      case 0xF30CFF00:
        if (code.size() != 4)
          code.push_back(1);
        break;
      case 0xE718FF00:
        if (code.size() != 4)
          code.push_back(2);
        break;
      case 0xA15EFF00:
        if (code.size() != 4)
          code.push_back(3);
        break;
      case 0xF708FF00:
        if (code.size() != 4)
          code.push_back(4);
        break;
      case 0xE31CFF00:
        if (code.size() != 4)
          code.push_back(5);
        break;
      case 0xA55AFF00:
        if (code.size() != 4)
          code.push_back(6);
        break;
      case 0xBD42FF00:
        if (code.size() != 4)
          code.push_back(7);
        break;
      case 0xAD52FF00:
        if (code.size() != 4)
          code.push_back(8);
        break;
      case 0xB54AFF00:
        if (code.size() != 4)
          code.push_back(9);
        break;
      }
      IrReceiver.resume();
    }
  }
  else if (!reedState)
  {
    opened = true;
  }
}
