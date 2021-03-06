// Extracted from GreyGnome and Ben Ripley:
// http://www.benripley.com/diy/arduino/three-ways-to-read-a-pwm-signal-with-arduino/
// EnableInterrupt Simple example sketch. Demonstrates operation on a single pin of your choice.
// See https://github.com/GreyGnome/EnableInterrupt and the README.md for more information.

#include <EnableInterrupt.h>
#include "TimerOne.h"

// Modify this at your leisure. Refer to https://github.com/GreyGnome/EnableInterrupt/wiki/Usage#Summary
#define ARDUINOPIN 7    // Read in Futaba PCM value

#define PWM2 9         // and convert and output to PWM2 pin
#define INA2 10
#define INB2 11

#define PWM1 6
#define INA1 12
#define INB1 13


//volatile uint16_t interruptCount=0;  // The count will go back to 0 after hitting 65535.
volatile uint16_t pwm_value = 0;        // ON time
volatile uint16_t prev_time = 0;
volatile uint32_t pwm_convert = 0;      // Duty cycle % equivalent
volatile uint16_t bias_chan3 = 940;     // bias from PCM1024 receiver for Channel 3 

//void interruptFunction() {
//  interruptCount++;
//}

void rising() {
  enableInterrupt(ARDUINOPIN, falling, FALLING);
  prev_time = micros();
}

void falling() {
  enableInterrupt(ARDUINOPIN, rising, RISING);
  pwm_value = micros() - prev_time;
  //Serial.println(pwm_value);
  pwm_convert = pwm_value - bias_chan3;
  if (pwm_convert < 0) {
    pwm_convert = 0;
  }
  pwm_convert = (pwm_convert * 255) / 1152;
  if (pwm_convert > 1030) pwm_convert = 0;
  analogWrite(PWM2,pwm_convert);
  analogWrite(PWM1,pwm_convert);
  //Serial.println(pwm_convert);
}

void setup() {
  Serial.begin(115200);
  
  pinMode(INA1, OUTPUT);
  pinMode(INB1, OUTPUT);
  pinMode(PWM1, OUTPUT);
  
  pinMode(INA2, OUTPUT);
  pinMode(INB2, OUTPUT);
  pinMode(PWM2, OUTPUT);

  digitalWrite(INA1, HIGH);
  digitalWrite(INB1, LOW);

  digitalWrite(INA2, HIGH);
  digitalWrite(INB2, LOW);
  
#ifdef MIGHTY1284
  DDRA = 0x0;    DDRB = 0x0;   DDRC = 0x0;   DDRD = 0x0; // set all pins as inputs
  PORTA = 0xFF; PORTB = 0xFF; PORTC = 0xFF; PORTD = 0xFF; // turn on all pullup resistors.
#else
  pinMode(ARDUINOPIN, INPUT_PULLUP);  // See http://arduino.cc/en/Tutorial/DigitalPins
#endif
  enableInterrupt(ARDUINOPIN, rising, RISING);
  //enableInterrupt(ARDUINOPIN, interruptFunction, CHANGE);
}

// In the loop we just display interruptCount. The value is updated by the interrupt routine.
void loop() {
  //Serial.println("---------------------------------------");
  delay(1000);
  //Serial.print("Pin was interrupted: ");
  //Serial.print(interruptCount, DEC);
  //Serial.println(" times so far.");
}

//////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// GORY DETAILS //////////////////////////////////////
// This has only been tested on an Arduino Duemilanove and Mega ADK.
// It is designed to work with the Arduino Duemilanove/Uno, Arduino Mega2560/ADK, the Arduino
// Leonardo, and the Arduino Due. Please let me know how you fare on the Leonardo or Due.

// To use:

// 1. You must be using a fairly recent version of the Arduino IDE software on your PC/Mac,
// that is, version 1.0.1 or later. Check Help->About Arduino in the IDE.

// 2. Wire a simple switch to any Analog or Digital pin (known as ARDUINOPIN, defined below)
// that supports interrupts. See https://github.com/GreyGnome/EnableInterrupt/wiki/Usage#Summary
// Attach the other end to a GND pin. A "single pole single throw momentary contact normally
// open" // pushbutton switch is best for the most interrupting fun.
// See https://www.sparkfun.com/products/97 and https://octopart.com/b3f-1000-omron-3117

// 3. When pressed, the switch will connect the pin to ground ("low", or "0") voltage, and interrupt the
// processor. See http://arduino.cc/en/Tutorial/DigitalPins

// 4. The interrupt is serviced immediately, and the ISR (Interrupt SubRoutine) modifies the value of
// the global variable interruptCount. Open Tools->Serial Monitor in the IDE to see the results of your
// interrupts.

// 5. Peruse the Examples directory for more elaborate examples.

// 6. Create your own sketch using the EnableInterrupt library!

