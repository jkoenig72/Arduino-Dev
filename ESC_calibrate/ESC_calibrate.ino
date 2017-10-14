/*

ESC Calibrate

ESC data pin attached to Digital Pin 10 on the arduino.
A potentiometer/a Joystick input attached to Analog Pin A0 on the Arduino.

*/


#include<Servo.h>           //Include The servo library for the functions used

Servo esc;                  //Declare the ESC as a Servo Object
int input;

void setup() 
{
  esc.attach(10);           //Attach the ESC to Digital Pin 10
  Serial.begin(9600);      //Begin Reading/Open Serial Monitor
}

void loop() 
{ 
  input=analogRead(A0);                       //Value of input is analog input on pin A0
  Serial.print("Original Input Value:");      //Serial print the original input value
  Serial.print(input);
  Serial.print(" | ");
  delay(1);
  input=map(analogRead(A0), 0, 1023, 800, 2000); //Map the input values from the joystick on analog pin 0 to  correspond to max and min values for the servo output: 2000 and 800
  Serial.print("Mapped Input Value:");
  Serial.print(input);                        //Serial print the mapped input value
  Serial.print("\n");
  delay(1);                                   //Delay determines your resolution/smoothness and responsiveness of Motor to the movement of Potwntiometer/Joystick.
  esc.writeMicroseconds(input);                           //The state of ESC will be the same as the state/position of Joystick
}
