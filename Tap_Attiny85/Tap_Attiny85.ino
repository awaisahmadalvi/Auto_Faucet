/*
    avrdude.exe  -CC:\Users\awais\AppData\Local\Arduino15\packages\diy_attiny\hardware\avr\2017.10.29/avrdude.conf -v -c stk500v1 -p ATtiny13 -P COM17 -b9600 -U flash:w:Tap_Attiny13.ino_attiny13a_9600000L.hex
*/
/*
    Arduino proximity sensor
    by Awais Alvi 2018
*/
#include "Tap_Attiny85.h"

class IR_Control
{
        // Constructor - creates a IR_Control
        // and initializes the member variables and state
    public:
        IR_Control() {};
        double getDist() {
            total = total + (analogRead(snsrInPin) - total) / 8;
            return total;
        }
    private:
        double total = 0;          // the running total
};

IR_Control IR_on, IR_off;

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

    prevMillis = millis();

    delay1(2000);
#ifdef SERIAL_DEBUG
    Serial.println("TAP Opened");
#endif

#ifdef LED
    digitalWrite(LED, HIGH);
    delay1(pulseTime);
    digitalWrite(LED, LOW);
#endif

    tapStatus = true;

    return;
}

void loop() {
    digitalWrite(irOutPin, HIGH);
    delay1(10);
    double distON = IR_on.getDist();
    digitalWrite(irOutPin, LOW);
    delay1(10);
    double distOFF = IR_off.getDist();;
    if (distOFF >= 0 || distON >= 0) {
        double diff = distOFF - distON;
#ifdef SERIAL_DEBUG
        Serial.print(diff);
#endif

        IRthres = IRthres + (diff - IRthres) / 32;
#ifdef SERIAL_DEBUG
        Serial.print("\t");
        Serial.println(IRthres);
#endif

        //if (millis() > 2000) {
        // tapStatus is stating if tap is open by threshold or not
        // tapConst is stating that threshold is already crossed and WDT has turned off the tap
        if (diff >= IRthres + 20 && !tapStatus && !tapConst)
        {
            openTAP();
        }
        else if (diff < IRthres - 5)
        {
            if (tapStatus == true)  // check if tap is already opened
                closeTAP();
            if (tapConst == true)   // preventing reopening right after WDT trigger
                tapConst = false;
        }
        
        unsigned long currentMillis = millis();
        if (tapStatus && currentMillis - prevMillis >= WDT_count) {
#ifdef SERIAL_DEBUG
            Serial.println("WDT Clossed");
#endif
            closeTAP();
            tapConst = true;
        }
        //}
    }
    delay1(1);
}
