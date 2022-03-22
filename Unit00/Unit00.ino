#include "cred.h" //Don't be a tosser
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <AceButton.h>

using namespace ace_button;
bool fetch_blynk_state = true;

#define RelayPin1 D5
#define RelayPin2 D6
#define RelayPin3 D7
#define RelayPin4 D8

#define SwitchPin1 10
#define SwitchPin2 1
#define SwitchPin3 3
#define SwitchPin4 D3

#define uplinkLED D0
// uplink status indicator LED
#define opsLED D4
// operations status indicator LED
#define VPIN_BUTTON_1 V1
#define VPIN_BUTTON_2 V2
#define VPIN_BUTTON_3 V3
#define VPIN_BUTTON_4 V4

bool toggleState_1 = LOW;
bool toggleState_2 = LOW;
bool toggleState_3 = LOW;
bool toggleState_4 = LOW;

int wifiFlag = 0;

char auth[] = BLYNK_AUTH_TOKEN;

ButtonConfig config1;
AceButton button1(&config1);
ButtonConfig config2;
AceButton button2(&config2);
ButtonConfig config3;
AceButton button3(&config3);
ButtonConfig config4;
AceButton button4(&config4);

void handleEvent1(AceButton *, uint8_t, uint8_t);
void handleEvent2(AceButton *, uint8_t, uint8_t);
void handleEvent3(AceButton *, uint8_t, uint8_t);
void handleEvent4(AceButton *, uint8_t, uint8_t);

BlynkTimer timer;

BLYNK_WRITE(VPIN_BUTTON_1)
{
  toggleState_1 = param.asInt();
  digitalWrite(RelayPin1, !toggleState_1);
}

BLYNK_WRITE(VPIN_BUTTON_2)
{
  toggleState_2 = param.asInt();
  digitalWrite(RelayPin2, !toggleState_2);
}

BLYNK_WRITE(VPIN_BUTTON_3)
{
  toggleState_3 = param.asInt();
  digitalWrite(RelayPin3, !toggleState_3);
}

BLYNK_WRITE(VPIN_BUTTON_4)
{
  toggleState_4 = param.asInt();
  digitalWrite(RelayPin4, !toggleState_4);
}

void checkBlynkStatus()
{

  bool isconnected = Blynk.connected();
  if (isconnected == false)
  {
    wifiFlag = 1;
    Serial.println("Blynk Not Connected");
    digitalWrite(uplinkLED, HIGH);
    digitalWrite(opsLED, LOW);
  }
  if (isconnected == true)
  {
    wifiFlag = 0;
    if (!fetch_blynk_state)
    {
      Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
      Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
      Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
      Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
    }
    digitalWrite(uplinkLED, LOW);
    digitalWrite(opsLED, HIGH);
    Serial.println("Blynk Connected");
  }
}

BLYNK_CONNECTED()
{
  // Request the latest state from the server
  if (fetch_blynk_state)
  {
    Blynk.syncVirtual(VPIN_BUTTON_1);
    Blynk.syncVirtual(VPIN_BUTTON_2);
    Blynk.syncVirtual(VPIN_BUTTON_3);
    Blynk.syncVirtual(VPIN_BUTTON_4);
  }
}

void setup()
{
  Serial.begin(115200);

  pinMode(opsLED, OUTPUT);
  pinMode(uplinkLED, OUTPUT);

  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);

  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  pinMode(SwitchPin3, INPUT_PULLUP);
  pinMode(SwitchPin4, INPUT_PULLUP);

  digitalWrite(RelayPin1, !toggleState_1);
  digitalWrite(RelayPin2, !toggleState_2);
  digitalWrite(RelayPin3, !toggleState_3);
  digitalWrite(RelayPin4, !toggleState_4);

  digitalWrite(uplinkLED, HIGH);

  config1.setEventHandler(button1Handler);
  config2.setEventHandler(button2Handler);
  config3.setEventHandler(button3Handler);
  config4.setEventHandler(button4Handler);

  button1.init(SwitchPin1);
  button2.init(SwitchPin2);
  button3.init(SwitchPin3);
  button4.init(SwitchPin4);

  WiFi.begin(ssid, pass);
  timer.setInterval(2000L, checkBlynkStatus);
  Blynk.config(auth);
  delay(1000);

  if (!fetch_blynk_state)
  {
    Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
    Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
    Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
    Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
  }
}

void loop()
{
  Blynk.run();
  timer.run();

  button1.check();
  button2.check();
  button3.check();
  button4.check();
}

void button1Handler(AceButton *button, uint8_t eventType, uint8_t buttonState)
{
  Serial.println("EVENT1");
  switch (eventType)
  {
  case AceButton::kEventReleased:
    Serial.println("kEventReleased");
    digitalWrite(RelayPin1, toggleState_1);
    toggleState_1 = !toggleState_1;
    Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
    break;
  }
}
void button2Handler(AceButton *button, uint8_t eventType, uint8_t buttonState)
{
  Serial.println("EVENT2");
  switch (eventType)
  {
  case AceButton::kEventReleased:
    Serial.println("kEventReleased");
    digitalWrite(RelayPin2, toggleState_2);
    toggleState_2 = !toggleState_2;
    Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
    break;
  }
}
void button3Handler(AceButton *button, uint8_t eventType, uint8_t buttonState)
{
  Serial.println("EVENT3");
  switch (eventType)
  {
  case AceButton::kEventReleased:
    Serial.println("kEventReleased");
    digitalWrite(RelayPin3, toggleState_3);
    toggleState_3 = !toggleState_3;
    Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
    break;
  }
}
void button4Handler(AceButton *button, uint8_t eventType, uint8_t buttonState)
{
  Serial.println("EVENT4");
  switch (eventType)
  {
  case AceButton::kEventReleased:
    Serial.println("kEventReleased");
    digitalWrite(RelayPin4, toggleState_4);
    toggleState_4 = !toggleState_4;
    Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
    break;
  }
}
