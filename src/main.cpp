#define IR_USE_AVR_TIMER1

#include <StandardCplusplus.h>
#include <Arduino.h>
#include <IRremote.h>
#include <vector>

unsigned long currentMillis;
unsigned long previousMillis = 0;

const int RECV_PIN = 11;
std::vector<int> code;
const int combination = 2749;

int doorPin = 2;
bool reedState;

bool opened = false;

// notes in the melody:
int alarmPin = 4;
int melody[]{530, 2000};
int currentFrequency = 0;
size_t melodySize = sizeof(melody) / sizeof(melody[0]);
unsigned int toneDuration = 500;

void setup()
{
  pinMode(doorPin, INPUT);

  // Serial.begin(9600);
  IrReceiver.begin(RECV_PIN);
}

void loop()
{
  currentMillis = millis();

  reedState = digitalRead(doorPin);

  // if (code.size() == 1)
  //   Serial.println(code[0]);
  // else if (code.size() == 2)
  //   Serial.println(code[0] * 10 + code[1]);
  // else if (code.size() == 3)
  //   Serial.println(code[0] * 100 + code[1] * 10 + code[2]);
  // else if (code.size() == 4)
  //   Serial.println(code[0] * 1000 + code[1] * 100 + code[2] * 10 + code[3]);

  // N.C. Reed Switch
  if (opened && currentMillis - previousMillis >= toneDuration)
  {
    previousMillis = currentMillis;
    tone(alarmPin, melody[currentFrequency], toneDuration);
    currentFrequency = currentFrequency == 0 ? 1 : 0;

    if (reedState  && IrReceiver.decode())
  {
    // Print Code in HEX
    switch (IrReceiver.decodedIRData.decodedRawData)
    {
    case 0xBA45FF00:
    {
      int codeNumber = code.size() == 4 ? code[0] * 1000 + code[1] * 100 + code[2] * 10 + code[3] : 0;
      if (code.size() == 4 && codeNumber == combination && reedState)
        opened = false;
      break;
    }
    case 0xBB44FF00:
      if (code.size() != 0)
        code.clear();
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
