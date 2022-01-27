#include "cred.h"
#include <ESP8266WiFi.h>        
#include <BlynkSimpleEsp8266.h>
#include <AceButton.h>
#include <LedControl.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <TimeLib.h>

LedControl lc = LedControl(D4, D2, D3, 1); //DIN,CLK,CS,display

byte last_second, second_, minute_, hour_, day_;
int Digit7, Digit6, Digit5, Digit4, Digit3, Digit2, Digit1, Digit0;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", (6*60*60), 60000);

using namespace ace_button;

bool fetch_blynk_state = true;

#define RelayPin1 D5
#define RelayPin2 D6
#define RelayPin3 D7
#define RelayPin4 D8

#define SwitchPin1 10
#define SwitchPin2 1
#define SwitchPin3 3
#define SwitchPin4 D1

#define wifiLed D0

#define VPIN_BUTTON_1    V4
#define VPIN_BUTTON_2    V5
#define VPIN_BUTTON_3    V6 
#define VPIN_BUTTON_4    V7

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

void handleEvent1(AceButton*, uint8_t, uint8_t);
void handleEvent2(AceButton*, uint8_t, uint8_t);
void handleEvent3(AceButton*, uint8_t, uint8_t);
void handleEvent4(AceButton*, uint8_t, uint8_t);

BlynkTimer timer;

BLYNK_WRITE(VPIN_BUTTON_1) {
  toggleState_1 = param.asInt();
  digitalWrite(RelayPin1, !toggleState_1);
}

BLYNK_WRITE(VPIN_BUTTON_2) {
  toggleState_2 = param.asInt();
  digitalWrite(RelayPin2, !toggleState_2);
}

BLYNK_WRITE(VPIN_BUTTON_3) {
  toggleState_3 = param.asInt();
  digitalWrite(RelayPin3, !toggleState_3);
}

BLYNK_WRITE(VPIN_BUTTON_4) {
  toggleState_4 = param.asInt();
  digitalWrite(RelayPin4, !toggleState_4);
}

void checkBlynkStatus() { 

  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    wifiFlag = 1;
    Serial.println("Blynk Not Connected");
    digitalWrite(wifiLed, HIGH);
  }
  if (isconnected == true) {
    wifiFlag = 0;
    if (!fetch_blynk_state){
    Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
    Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
    Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
    Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
    }
    digitalWrite(wifiLed, LOW);
    Serial.println("Blynk Connected");
  }
}

BLYNK_CONNECTED() {
  // Request the latest state from the server
  if (fetch_blynk_state){
    Blynk.syncVirtual(VPIN_BUTTON_1);
    Blynk.syncVirtual(VPIN_BUTTON_2);
    Blynk.syncVirtual(VPIN_BUTTON_3);
    Blynk.syncVirtual(VPIN_BUTTON_4);
  }
}

void setup()
{
  Serial.begin(115200);

  lc.shutdown(0,false);
  lc.setIntensity(0,15);
  lc.clearDisplay(0);

  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);

  pinMode(wifiLed, OUTPUT);

  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  pinMode(SwitchPin3, INPUT_PULLUP);
  pinMode(SwitchPin4, INPUT_PULLUP);

  digitalWrite(RelayPin1, !toggleState_1);
  digitalWrite(RelayPin2, !toggleState_2);
  digitalWrite(RelayPin3, !toggleState_3);
  digitalWrite(RelayPin4, !toggleState_4);

  digitalWrite(wifiLed, HIGH);

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
  
  if (!fetch_blynk_state){
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


  timeClient.update();
  unsigned long unix_epoch = timeClient.getEpochTime();
    second_ = second(unix_epoch);
  if (last_second != second_) {


    minute_ = minute(unix_epoch);
    hour_   = hour(unix_epoch);
    day_    = day(unix_epoch);

    Digit0 = second_ % 10;
    Digit1 = second_ / 10;
    Digit2  = minute_ % 10;
    Digit3  = minute_ / 10;
    Digit4  = hour_   % 10;
    Digit5  = hour_   / 10;
    Digit6  = day_   %  10;
    Digit7  = day_   /10;
    
  Serial.println(day_);
  Serial.println(hour_);
  Serial.println(minute_);
  Serial.println(second_);
  
  last_second = second_;

  lc.setDigit(0,7,(byte)Digit7,false);
  lc.setDigit(0,6,(byte)Digit6,true);
  lc.setDigit(0,5,(byte)Digit5,false);
  lc.setDigit(0,4,(byte)Digit4,true);
  lc.setDigit(0,3,(byte)Digit3,false);
  lc.setDigit(0,2,(byte)Digit2,true);
  lc.setDigit(0,1,(byte)Digit1,false);
  lc.setDigit(0,0,(byte)Digit0,false);  
  }

  button1.check();
  button2.check();
  button3.check();
  button4.check();
}

void button1Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT1");
  switch (eventType) {
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      digitalWrite(RelayPin1, toggleState_1);
      toggleState_1 = !toggleState_1;
      Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
      break;
  }
}
void button2Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT2");
  switch (eventType) {
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      digitalWrite(RelayPin2, toggleState_2);
      toggleState_2 = !toggleState_2;
      Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
      break;
  }
}
void button3Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT3");
  switch (eventType) {
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      digitalWrite(RelayPin3, toggleState_3);
      toggleState_3 = !toggleState_3;
      Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
      break;
  }
}
void button4Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT4");
  switch (eventType) {
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      digitalWrite(RelayPin4, toggleState_4);
      toggleState_4 = !toggleState_4;
      Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
      break;
  }
}
