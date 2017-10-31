#include <LiquidCrystal.h>
#include <TimeLib.h>
#include <TimeLib.h>
#include <TimeAlarms.h>

AlarmId id;

// encoder
const int encPinA = 6;
const int encPinB = 7;
int encValue = 0;      // this is the starting value
int encOldValue = 0;    //
const int encRight = 1;            // don't change these values
const int encLeft = 2;             //
const int encNoChange = 3;         //
int encValueA = 0;                 //
int encValueB = 0;                 //
int encDirection = encNoChange;    //
const int encMinValue = 0;      // change these if you wish
const int encMaxValue = 59;    //
const int encStep = 1;          //

// Buttons
const int bttnOne = 13;
const int bttnTwo = 8;
const int bttnThree = 10;

// leds
const int ledOne = A2;
const int ledTwo = A3;
const int ledThree = A4;

// Sensor
const int lightSensor = A1;

// Buzzer
const int buzzerPin = 1;

// LCD screen
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int STATE = 0;
unsigned long timer = 0;
int goal = 15;
bool LedsOn = false;
int correctAttempts = 0;

void setup() {

  pinMode(encPinA, INPUT_PULLUP);    // active LOW logic
  pinMode(encPinB, INPUT_PULLUP);    // active LOW logic
  pinMode(bttnOne, INPUT);
  pinMode(bttnTwo, INPUT);
  pinMode(bttnThree, INPUT);
  pinMode(ledOne, OUTPUT);
  pinMode(ledTwo, OUTPUT);
  pinMode(ledThree, OUTPUT);
  pinMode(lightSensor, INPUT);
  pinMode(buzzerPin, OUTPUT);
  lcd.begin(16, 2);
  analogWrite(A0, 0);
  setTime(11, 28, 0, 10, 30, 17); // set time to Saturday 8:29:00am Jan 1 2011
  //  encValue = minute();
  delay(2000);

}

void loop() {

  switch (STATE) {
    case 0: // display hours
      digitalClockDisplay();
      break;
    case 1: // setting alarm
      displayAlarmSetting(encValue);
      break;
    case 2: // play alarm
      playAlarm();
      break;
    default:
      break;
  }
  if (STATE != 2) {
    encProcess();                    // check for encoder rotation
    if (encValue != encOldValue) {   // encoder rotation actions
      encOldValue = encValue;
      timer = millis();
      STATE = 1;
    }
  }
  Alarm.delay(0);

}

void digitalClockDisplay() {
  String Time;
  if (minute() < 10) {
    Time = String(hour()) + ":0" + String(minute());
  }
  else {
    Time = String(hour()) + ":" + String(minute());
  }
  lcd.setCursor(0, 0);
  lcd.print(Time);
}

void displayAlarmSetting(int value) {
  String Time;
  if (value < 10) {
    Time = String(hour()) + ":0" + String(value);
  }
  else {
    Time = String(hour()) + ":" + String(value);
  }
  lcd.setCursor(0, 0);
  lcd.print(Time);


  if (digitalRead(bttnTwo) == 1) {
    int diff = (value - minute()) * 60;
    Alarm.timerOnce(diff, OnceOnly);            // called once after x seconds
    lcd.setCursor(0, 0);
    lcd.print("Alarm on");
    STATE = 0;
    delay(1000);
    lcd.clear();
    timer = millis();
  }

  if (millis() - timer > 4000) STATE = 0;
}

void OnceOnly() {
  lcd.setCursor(0, 0);
  lcd.print("Alarm!!!");
  STATE = 2;
}
void playAlarm() {

  tone(1, 4000, 200);
  delay(200);
  noTone(1);
  delay(200);
  tone(1, 4000, 400);
  delay(400);
  noTone(1);



  int randNumber = random(2, 4);
  turnOnLed(randNumber);
  LedsOn = true;
  timer = millis();

  while (LedsOn && millis() - timer < 1300) {
    if (checkButtons(randNumber) == true) {
      turnOffLed(randNumber);
      LedsOn = false;
      delay(200);
      timer = millis();
      correctAttempts += 1;
      if (correctAttempts == goal) {
        alarmOff();
        correctAttempts = 0;
        STATE = 0;
        goal += 1;
      }
    }
    else if (analogRead(lightSensor) > 100) {
      alarmOff();
      correctAttempts = 0;
      STATE = 0;
      goal += 1;
    }
  }
  if (millis() - timer > 1290) {
    error();
    delay(200);
    lcd.clear();
  }

}

void turnOnLed(int randNumber) {
  switch (randNumber) {
    case 1:
      analogWrite(ledOne, 180);

      break;
    case 2:
      analogWrite(ledTwo, 180);

      break;
    case 3:
      analogWrite(ledThree, 180);

    default:
      break;
  }
}
void turnOffLed(int randNumber) {
  switch (randNumber) {
    case 1:
      analogWrite(ledOne, 0);
      break;
    case 2:
      analogWrite(ledTwo, 0);
      break;
    case 3:
      analogWrite(ledThree, 0);
    default:
      break;
  }
}

bool checkButtons(int randNumber) {

  int one = digitalRead(bttnOne);
  int two = digitalRead(bttnTwo);
  int three = digitalRead(bttnThree);

  if (two == 1 && randNumber == 2) {
    return true;
  }
  else  if (three == 1 && randNumber == 3) {
    return true;
  }
  else if (two == 1 && randNumber != 2) {
    error();
    return false;
  }
  else if (three == 1 && randNumber != 3) {
    error();
    return false;
  }
  else {
    return false;
  }
  delay(500);
}

void error() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ERROR");
  turnOffLed(1);
  turnOffLed(2);
  turnOffLed(3);
  LedsOn = false;
  correctAttempts = 0;
}

void alarmOff() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("OFF");
  delay(2000);

}
