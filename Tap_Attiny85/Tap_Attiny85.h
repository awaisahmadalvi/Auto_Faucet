/*
    Arduino proximity sensor
    by Awais Alvi 2018
*/
#define tiny true
//#define SERIAL_DEBUG 1
//#define LED 13
#define H_PWM 0

#ifdef tiny
/*
    //Do some low level AVR pin writing to save instructions. 1010 of 1024. Close!
    #define SetPin(Bit)    PORTB |= (1 << Bit)
    #define ClearPin(Bit)    PORTB &= ~(1 << Bit)
*/
// wire your IR led to this pin. (chip pin 3);
#define irOutPin 1//3 - ATTINY85
// These pins go high when the sensor detects and obstacle. (chip pin 0/1)
#define mtrPin1 3//1 - ATTINY85
#define mtrPin2 4//0 - ATTINY85

// wire this pin to your phototrans vcc-->10K res-->pin4-->phototrans-->gnd (chip pin A1)
#define snsrInPin A1

#endif

#ifndef tiny

// wire your IR led to this pin. (chip pin 8);
#define irOutPin 8

// These pins go high when the sensor detects and obstacle. (chip pin 11/12)
#define mtrPin1 11
#define mtrPin2 12

// wire this pin to your phototrans vcc-->10K res-->pin4-->phototrans-->gnd (chip pin A0)
#define snsrInPin A0

#endif

#define IRthres 250
#define pulseTime 100

// constants won't change:
#define WDT_count 10 * 1000      // interval at which to Close TAP (milliseconds)

void delay1(int msec);


// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;       // will store last time TAP was OPENED

bool tapStatus = false, tapConst = false, thres = false;

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
