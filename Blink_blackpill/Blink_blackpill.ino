/*
 Blink: Turns on the built-in LED on for one second, then off for one second, repeatedly.
 Arduino 1.6.0rc1
   Sketch uses 11,900 bytes (11%) of program storage space. Maximum is 108,000 bytes.
   Global variables use 2,592 bytes of dynamic memory.
 Ported to Maple from the Arduino example 27 May 2011 By Marti Bolivar
*/

void setup() {
    // Set up the built-in LED pin as an output:
    pinMode(6, OUTPUT);
    //pinMode(PC13 , OUTPUT);
}

void loop() {
    digitalWrite(6,!digitalRead(6));// Turn the LED from off to on, or on to off
    //digitalWrite(PC13,!digitalRead(PC13));// Turn the LED from off to on, or on to off
    delay(25);          // Wait for 1 second (1000 milliseconds)
}
