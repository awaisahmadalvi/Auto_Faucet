/*
    Arduino proximity sensor
    by Awais Alvi 2018
*/

#define H_PWM   0
#define mtrPin1 3
#define mtrPin2 4
#define snsrInPin 2

#define pulseTime 100

// constants won't change:
#define WDT_count 2 * 1000      // interval at which to Close TAP (milliseconds)

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;       // will store last time TAP was OPENED

bool tapStatus = false, tapConst = false, thres = false;
