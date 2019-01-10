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
        int getDist() {
            total = total + ((analogRead(snsrInPin) - total) >> 3);
            return total;
        }
    private:
        int total = 0;          // the running total
};

IR_Control IR_on, IR_off;

int HOLDTIME = 3 * 1000;        // interval to HOLD TAP (milliseconds)

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
    prevMillis = 0;
}


void closeTAP() {
    digitalWrite(mtrPin1, LOW);
    digitalWrite(mtrPin2, HIGH);
#ifdef H_PWM
    digitalWrite(H_PWM, HIGH);
#endif
    delay(pulseTime);
#ifdef H_PWM
    digitalWrite(H_PWM, LOW);
#endif
    digitalWrite(mtrPin2, LOW);

#ifdef SERIAL_DEBUG
    Serial.println("TAP Closed");
#endif

#ifdef LED
    digitalWrite(LED, HIGH);
    delay(pulseTime);
    digitalWrite(LED, LOW);
#endif

    tapOpen = false;
    return;
}

void openTAP() {
    digitalWrite(mtrPin2, LOW);
    digitalWrite(mtrPin1, HIGH);
#ifdef H_PWM
    digitalWrite(H_PWM, HIGH);
#endif
    delay(pulseTime);
#ifdef H_PWM
    digitalWrite(H_PWM, LOW);
#endif
    digitalWrite(mtrPin1, LOW);

    prevMillis = millis();
    delay(HOLDTIME);

#ifdef SERIAL_DEBUG
    Serial.println("TAP Opened");
#endif

#ifdef LED
    digitalWrite(LED, HIGH);
    delay(pulseTime);
    digitalWrite(LED, LOW);
#endif

    tapOpen = true;

    return;
}


void loop() {

    digitalWrite(irOutPin, HIGH);
    delay(10);
    int distON = analogRead(snsrInPin);//IR_on.getDist();//analogRead(snsrInPin);
    delay(50);

#ifdef SERIAL_DEBUG_ONOFF
    Serial.print("HIGH\t");
    Serial.print(distON);
#endif

    digitalWrite(irOutPin, LOW);
    delay(10);
    int distOFF = analogRead(snsrInPin);//IR_off.getDist();//analogRead(snsrInPin);
    delay(50);

#ifdef SERIAL_DEBUG_ONOFF
    Serial.print("\t,LOW\t");
    Serial.println(distOFF);
#endif

    if (distOFF >= 0 || distON >= 0) {
        int diff = distOFF - distON;

#ifdef SERIAL_DEBUG_DIFF_THRESH
        Serial.print(diff);
#endif

        IRthres = IRthres + ((diff - IRthres) >> 3);
#ifdef SERIAL_DEBUG_DIFF_THRESH
        Serial.print("\t");
        Serial.println(IRthres);
#endif


        if (millis() - prevMillis >= HOLDTIME) {
            HOLDTIME = 2 * 1000;
            // tapOpen is stating if tap is open by threshold or not
            // tapWDT is stating that threshold is already crossed and WDT has turned off the tap
            if (diff >= IRthres + 80  && !tapOpen && !tapWDT) {
                openTAP();
            }
            else if (diff <= IRthres)
            {
                if (tapOpen == true)  // check if tap is already opened
                    closeTAP();
                if (tapWDT == true)   // preventing reopening right after WDT trigger
                    tapWDT = false;
            }
            if (tapOpen && millis() - prevMillis >= WDT_count) {
#ifdef SERIAL_DEBUG
                Serial.println("WDT Clossed");
#endif
                closeTAP();
                tapWDT = true;
            }
        }
    }
    delay(1);
}

