/*#include <IRremote.h>

const int RECV_PIN = 11;

void setup()
{
  Serial.begin(9600);
  // Enable the IR Receiver
  IrReceiver.begin(RECV_PIN, DISABLE_LED_FEEDBACK);
}

void loop()
{
  if (IrReceiver.decode())
  {
    // Print Code in HEX
    switch (IrReceiver.decodedIRData.decodedRawData)
    {
    case 0xBA45FF00:
      Serial.println("POWER");
      break;
    case 0xB847FF00:
      Serial.println("FUNC/STOP");
      break;
    case 0xB946FF00:
      Serial.println("VOL+");
      break;
    case 0xBB44FF00:
      Serial.println("FAST BACK");
      break;
    case 0xBF40FF00:
      Serial.println("PAUSE");
      break;
    case 0xBC43FF00:
      Serial.println("FAST FORWARD");
      break;
    case 0xF807FF00:
      Serial.println("DOWN");
      break;
    case 0xEA15FF00:
      Serial.println("VOL-");
      break;
    case 0xF609FF00:
      Serial.println("UP");
      break;
    case 0xE619FF00:
      Serial.println("EQ");
      break;
    case 0xF20DFF00:
      Serial.println("ST/REPT");
      break;
    case 0xE916FF00:
      Serial.println("0");
      break;
    case 0xF30CFF00:
      Serial.println("1");
      break;
    case 0xE718FF00:
      Serial.println("2");
      break;
    case 0xA15EFF00:
      Serial.println("3");
      break;
    case 0xF708FF00:
      Serial.println("4");
      break;
    case 0xE31CFF00:
      Serial.println("5");
      break;
    case 0xA55AFF00:
      Serial.println("6");
      break;
    case 0xBD42FF00:
      Serial.println("7");
      break;
    case 0xAD52FF00:
      Serial.println("8");
      break;
    case 0xB54AFF00:
      Serial.println("9");
      break;
    default:
      Serial.println("other button");
    }
    IrReceiver.resume();
  }
}*/
#define IR_USE_AVR_TIMER1

#include <Arduino.h>
#include <IRremote.h>

unsigned long currentMillis;
unsigned long previousMillis = 0;

int doorPin = 2;
const int RECV_PIN = 11;
bool reedState;

int resetPin = 3;
bool resetState;
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
  pinMode(resetPin, INPUT);

  Serial.begin(9600);
  IrReceiver.begin(RECV_PIN);
}

void loop()
{
  currentMillis = millis();

  reedState = digitalRead(doorPin);
  resetState = digitalRead(resetPin);

  // N.C. Reed Switch
  if (opened && resetState && reedState)
  {
    opened = false;
  }
  if (opened && IrReceiver.decode())
  {
    // Print Code in HEX
    switch (IrReceiver.decodedIRData.decodedRawData)
    {
    case 0xBA45FF00:
      Serial.println("POWER");
      break;
    case 0xB847FF00:
      Serial.println("FUNC/STOP");
      break;
    case 0xB946FF00:
      Serial.println("VOL+");
      break;
    case 0xBB44FF00:
      Serial.println("FAST BACK");
      break;
    case 0xBF40FF00:
      Serial.println("PAUSE");
      break;
    case 0xBC43FF00:
      Serial.println("FAST FORWARD");
      break;
    case 0xF807FF00:
      Serial.println("DOWN");
      break;
    case 0xEA15FF00:
      Serial.println("VOL-");
      break;
    case 0xF609FF00:
      Serial.println("UP");
      break;
    case 0xE619FF00:
      Serial.println("EQ");
      break;
    case 0xF20DFF00:
      Serial.println("ST/REPT");
      break;
    case 0xE916FF00:
      Serial.println("0");
      break;
    case 0xF30CFF00:
      Serial.println("1");
      break;
    case 0xE718FF00:
      Serial.println("2");
      break;
    case 0xA15EFF00:
      Serial.println("3");
      break;
    case 0xF708FF00:
      Serial.println("4");
      break;
    case 0xE31CFF00:
      Serial.println("5");
      opened = false;
      break;
    case 0xA55AFF00:
      Serial.println("6");
      break;
    case 0xBD42FF00:
      Serial.println("7");
      break;
    case 0xAD52FF00:
      Serial.println("8");
      break;
    case 0xB54AFF00:
      Serial.println("9");
      break;
    default:
      Serial.println("error");
    }
    IrReceiver.resume();
  }

  if (opened && currentMillis - previousMillis >= toneDuration)
  {
    previousMillis = currentMillis;
    tone(alarmPin, melody[currentFrequency], toneDuration);
    currentFrequency = currentFrequency == 0 ? 1 : 0;
  }
  else if (!reedState)
  {
    opened = true;
  }
}
