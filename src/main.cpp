#include <Arduino.h>

unsigned long currentMillis;
unsigned long previousMillis = 0;

short int doorPin = 2;
bool reedState;

int resetPin = 3;
bool resetState;
bool opened = false;

// notes in the melody:
short int melody[]{530, 2000};
short int currentFrequency = 0;
size_t melodySize = sizeof(melody) / sizeof(melody[0]);
unsigned short int toneDuration = 500;

void setup()
{
  pinMode(doorPin, INPUT);
  pinMode(resetPin, INPUT);
  // Serial.begin(9600);
}

void loop()
{
  currentMillis = millis();

  reedState = digitalRead(doorPin);
  resetState = digitalRead(resetPin);
  // Serial.println("dfsdf");

  if (opened && resetState)
  {
    opened = false;
  }
  // N.C. Reed Switch
  if (opened && currentMillis - previousMillis >= toneDuration)
  {
    previousMillis = currentMillis;
    tone(4, melody[currentFrequency], toneDuration);
    currentFrequency = currentFrequency == 0 ? 1 : 0;
  }
  else if (!reedState)
  {
    opened = true;
  }
}
