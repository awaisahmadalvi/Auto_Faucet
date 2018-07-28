/*
    avrdude.exe  -CC:\Users\awais\AppData\Local\Arduino15\packages\diy_attiny\hardware\avr\2017.10.29/avrdude.conf -v -c stk500v1 -p ATtiny13 -P COM17 -b9600 -U flash:w:Tap_Attiny13.ino_attiny13a_9600000L.hex
*/
/*
    Arduino proximity sensor
    by Awais Alvi 2018
*/
#include "Tap_Attiny85.h"

#define numReadings 3

class IR_Control
{
    // Constructor - creates a IR_Control
    // and initializes the member variables and state
  public:
    IR_Control() {};
    int getDist() {

      distRead = analogRead(snsrInPin);
      double t = (distRead - total) / 8;
      total = total + t;

      readIndex++;

      if (readIndex >= numReadings) {
        readIndex = 0;
        return total;
      }
      delay1(1);
      return -1;
    }
  private:
    int distRead;           // the readings from the analog input
    int readIndex = 0;      // the index of the current reading
    double total = 0;          // the running total

};

IR_Control IR_on, IR_off;


void setup()
{
#ifdef SERIAL_DEBUG
  Serial.begin(115200);
#endif
#ifdef SERIAL_DEBUG2
  Serial.begin(115200);
#endif
#ifdef LED
  pinMode(LED, OUTPUT);
#endif
  pinMode(irOutPin, OUTPUT);
  pinMode(mtrPin1, OUTPUT);
  pinMode(mtrPin2, OUTPUT);
}

void closeTAP() {
  digitalWrite(mtrPin1, LOW);
  digitalWrite(mtrPin2, HIGH);
#ifdef H_PWM
  digitalWrite(H_PWM, HIGH);
#endif
  delay1(pulseTime);
#ifdef H_PWM
  digitalWrite(H_PWM, LOW);
#endif
  digitalWrite(mtrPin2, LOW);

#ifdef SERIAL_DEBUG
  Serial.println("TAP Closed");
#endif

#ifdef LED
  digitalWrite(LED, HIGH);
  delay1(pulseTime);
  digitalWrite(LED, LOW);
#endif

  tapStatus = false;
  return;
}

void openTAP() {
  digitalWrite(mtrPin2, LOW);
  digitalWrite(mtrPin1, HIGH);
#ifdef H_PWM
  digitalWrite(H_PWM, HIGH);
#endif
  delay1(pulseTime);
#ifdef H_PWM
  digitalWrite(H_PWM, LOW);
#endif
  digitalWrite(mtrPin1, LOW);

  previousMillis = millis();

#ifdef SERIAL_DEBUG
  Serial.println("TAP Opened");
#endif

#ifdef LED
  digitalWrite(LED, HIGH);
  delay1(pulseTime);
  digitalWrite(LED, LOW);
#endif

  tapStatus = true;

  delay1(openTime);
  return;
}

void loop() {
  digitalWrite(irOutPin, HIGH);
  delay1(10);
  int distON = IR_on.getDist();
  digitalWrite(irOutPin, LOW);
  delay1(10);
  int distOFF = IR_off.getDist();
  if (distOFF >= 0 || distON >= 0) {
    int diff = distOFF - distON;
#ifdef SERIAL_DEBUG
    Serial.println(diff);
#endif

    if (diff >= IRthres) {
      if (!tapStatus && !tapConst)
      {
        if (thres) {
          thres = false;
          openTAP();
        }
        else
          thres = true;
      }
    }
    else if (diff < IRthres )//* 0.9)
    {
      if (tapStatus == true)
        closeTAP();
      if (tapConst == true)
        tapConst = false;
    }

    unsigned long currentMillis = millis();
    if (tapStatus && currentMillis - previousMillis >= WDT_count) {
#ifdef SERIAL_DEBUG
      Serial.println("WDT Clossed");
#endif
      closeTAP();
      tapConst = true;
    }
  }
  delay1(1);
}

