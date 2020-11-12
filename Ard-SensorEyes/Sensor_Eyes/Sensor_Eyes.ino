/***********************************************
 * This is a little Halloween special
 * 
 * Use a PiR sensor to turn on an ultrasound.
 * Set the LED brightness based on the distance from
 * the ultrasound.
 ***********************************************/

#include <hcsr04.h>

#define TRIG_PIN 12
#define ECHO_PIN 11

const byte interruptPin = 2;
const byte signalPin = 13;
const byte ledPin = 3;

int distance = 0;
int pir_distance = 0;

HCSR04 hcsr04(TRIG_PIN, ECHO_PIN, 20, 4000);

void setup(){
    Serial.begin(9600);

    pinMode(signalPin, OUTPUT);
    pinMode(interruptPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(interruptPin), togglePiR, CHANGE);
}

void loop() {
    // Show the distance in mm from the ultrasound
    Serial.print("PiR Distance: ");
    Serial.println(pir_distance);

    // If the PiR is not triggered, turn off the eyes
    if(digitalRead(interruptPin) == LOW) {
      distance = 0;
    } else {
      distance = map(pir_distance, 0, 2000, 0, 255);
    }
    // Set an upper limit to the PWM output value
    if(distance > 255) distance=255;
    
    // Show the PWM value for the eyes
    Serial.print("LED Distance: ");
    Serial.println(distance);

    // Turn on the LEDs using an analog value
    analogWrite(ledPin, distance);

    // Grab a new measurement from the ultrasound
    pir_distance = hcsr04.distanceInMillimeters();
    delay(250);
}

// If the PiR is triggered, send a voltage to the ultrasound
void togglePiR() {
  digitalWrite(signalPin, digitalRead(interruptPin));
}
