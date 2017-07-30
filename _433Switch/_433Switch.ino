/*
  Example for different sending methods
  
  https://github.com/sui77/rc-switch/
  
*/
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {

  Serial.begin(9600);
  Serial.println("setup:start");
  
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(D2);

  // Optional set pulse length.
  mySwitch.setPulseLength(514);
  
  // Optional set protocol (default is 1, will work for most outlets)
  mySwitch.setProtocol(5);
  
  // Optional set number of transmission repetitions.
  mySwitch.setRepeatTransmit(5);
  Serial.println("setup:end");
}

void loop() 
{
  /* Same switch as above, but using binary code */
  Serial.println("loop:start");
  mySwitch.send("000001000000010101010001");
  delay(2000);  
  mySwitch.send("000001000000010101010100");
  delay(2000);
  Serial.println("loop:end");
}
