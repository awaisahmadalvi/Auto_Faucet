/*
    Arduino proximity sensor
    by Awais Alvi 2018
*/

#define tiny true
//#define SERIAL_DEBUG 1
//#define LED 9

#ifdef tiny

//Do some low level AVR pin writing to save instructions. 1010 of 1024. Close!
#define SetPin(Bit)    PORTB |= (1 << Bit)
#define ClearPin(Bit)    PORTB &= ~(1 << Bit)

// wire your IR led to this pin. (chip pin 3);
#define irOutPin 3
// These pins go high when the sensor detects and obstacle. (chip pin 0/1)
#define mtrPin1 1
#define mtrPin2 0

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

#define IRthres 280
#define pulseTime 300

void delay1(int msec);


const int numReadings = 5;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

int readings2[numReadings];      // the readings from the analog input
int readIndex2 = 0;              // the index of the current reading
int total2 = 0;                  // the running total
int average2 = 0;                // the average

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;       // will store last time TAP was OPENED

// constants won't change:
const long WDT_count = 5 * 1000;         // interval at which to Close TAP (milliseconds)

bool tapStatus = false, tapConst = false, thres = false;
