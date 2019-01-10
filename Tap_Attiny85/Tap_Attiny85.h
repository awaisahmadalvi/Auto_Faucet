/*
    Arduino proximity sensor
    by Awais Alvi 2018
*/
#define tiny true
//#define tiny85 true
//#define arduino true
//#define nano true
//#define SERIAL_DEBUG 1
//#define SERIAL_DEBUG_ONOFF 1
//#define SERIAL_DEBUG_DIFF_THRESH 1
//#define LED 13

#ifdef tiny

#define irOutPin 1
#define mtrPin1 4
#define mtrPin2 3
#define snsrInPin A1
#define H_PWM 0

#endif

#ifdef tiny85 

#define irOutPin 1
#define mtrPin1 4
#define mtrPin2 3
#define snsrInPin A1    /* TO-DO */
#define H_PWM 0         /* TO-DO */

#endif

#ifdef nano

#define irOutPin 6
#define mtrPin1 3
#define mtrPin2 2
#define snsrInPin A0
#define H_PWM 5

#endif

#ifdef arduino

#define irOutPin 6
#define mtrPin1 11
#define mtrPin2 12
#define snsrInPin A0
//#define H_PWM 5     /* TO-DO */

#endif

int IRthres;
#define pulseTime 100

// constants won't change:
#define WDT_count 5 * 1000          

void delay1(int msec);

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long prevMillis = 0;       // will store last time TAP was OPENED

bool tapOpen = false, tapWDT = false, thres = false;

