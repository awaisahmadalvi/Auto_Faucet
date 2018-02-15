/*
    avrdude.exe  -CC:\Users\awais\AppData\Local\Arduino15\packages\diy_attiny\hardware\avr\2017.10.29/avrdude.conf -v -c stk500v1 -p ATtiny13 -P COM17 -b9600 -U flash:w:Tap_Attiny13.ino_attiny13a_9600000L.hex
*/
/*
    Arduino proximity sensor
    by Awais Alvi 2018
*/
#include "Tap_Attiny13.h"

void setup()
{
#ifdef SERIAL_DEBUG
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
    digitalWrite(4, HIGH);
    delay(pulseTime);
    digitalWrite(4, LOW);
    digitalWrite(mtrPin2, LOW);
    
#ifdef SERIAL_DEBUG
    Serial.println("TAP Closed");
#endif

#ifdef LED
    digitalWrite(LED, HIGH);
    delay(pulseTime);
    digitalWrite(LED, LOW);
#endif

    tapStatus = false;
    return;
}

void openTAP() {
    digitalWrite(mtrPin2, LOW);
    digitalWrite(mtrPin1, HIGH);
    digitalWrite(4, HIGH);
    delay(pulseTime);
    digitalWrite(4, LOW);
    digitalWrite(mtrPin1, LOW);
    
    previousMillis = millis();

#ifdef SERIAL_DEBUG
    Serial.println("TAP Opened");
#endif

#ifdef LED
    digitalWrite(LED, HIGH);
    delay(pulseTime);
    digitalWrite(LED, LOW);
#endif

    tapStatus = true;

    return;
}

void loop() {
    digitalWrite(irOutPin, HIGH);
    delay(10);
    LedOnstate();
    digitalWrite(irOutPin, LOW);
    delay(10);
    LedOffstate();
    if (readIndex >= numReadings - 1 || readIndex2 >= numReadings - 1) {
        int diff = total2 - total;
#ifdef SERIAL_DEBUG
        Serial.println(diff);
#endif

        if (diff >= IRthres && !tapStatus && !tapConst)
        {
            if (thres) {
                thres = false;
                openTAP();
            }
            else
                thres = true;
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
    delay(1);
}

void LedOnstate() {
    total = total - readings[readIndex];
    readings[readIndex] = analogRead(snsrInPin);
    total = total + readings[readIndex];
    readIndex = readIndex + 1;

    if (readIndex >= numReadings) {
        readIndex = 0;
        average = total / numReadings;
    }
    delay(1);
}

void LedOffstate() {
    total2 = total2 - readings2[readIndex2];
    readings2[readIndex2] = analogRead(snsrInPin);
    total2 = total2 + readings2[readIndex2];
    readIndex2 = readIndex2 + 1;

    if (readIndex2 >= numReadings) {
        readIndex2 = 0;
        average2 = total2 / numReadings;
    }
    delay(1);
}

void delay1(int msec)
{
    //delay(1) is too costly in memory. Why is this so much cheaper? :D
    long start = millis();
    while (true)
    {
        if (millis() - start > msec) {
            break;
        }
    }
}
