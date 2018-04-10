/*
    avrdude.exe  -CC:\Users\awais\AppData\Local\Arduino15\packages\diy_attiny\hardware\avr\2017.10.29/avrdude.conf -v -c stk500v1 -p ATtiny13 -P COM17 -b9600 -U flash:w:Tap_Attiny13.ino_attiny13a_9600000L.hex
*/
/*
    Arduino proximity sensor
    by Awais Alvi 2018
*/
#include "Tap_btn_Attiny13A.h"

void setup()
{
    pinMode(mtrPin1, OUTPUT);
    pinMode(mtrPin2, OUTPUT);
    pinMode(snsrInPin, INPUT_PULLUP);
}

void closeTAP() {
    digitalWrite(mtrPin1, LOW);
    digitalWrite(mtrPin2, HIGH);
    digitalWrite(H_PWM, HIGH);
    delay(pulseTime);
    digitalWrite(H_PWM, LOW);
    digitalWrite(mtrPin2, LOW);
    tapStatus = false;
    return;
}

void openTAP() {
    digitalWrite(mtrPin2, LOW);
    digitalWrite(mtrPin1, HIGH);
    digitalWrite(H_PWM, HIGH);
    delay(pulseTime);
    digitalWrite(H_PWM, LOW);
    digitalWrite(mtrPin1, LOW);
    previousMillis = millis();
    tapStatus = true;

    return;
}

void loop() {

    int sensorVal = digitalRead(snsrInPin);
    if (sensorVal == LOW) {
        if (!tapStatus)
            openTAP();
    } else {
        unsigned long currentMillis = millis();
        if (tapStatus && currentMillis - previousMillis >= WDT_count) {
            closeTAP();
        }
    }
    delay(1);
}

