/*
  Example for different sending methods
  
  https://github.com/sui77/rc-switch/
  
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

String s1on="110001111001100001000000";
String s1off="110001001101111100100000";

String s2on="110000001110000100110100";
String s2off="110001011000101100010100";

String s3on="110000001110000100111100";
String s3off="110001011000101100011100";

String s4on="110001011000101100010010";
String s4off="110000001110000100110010";

String s5on="000000000001000101010001";
String s5off="000000000001000101010100";

String s6on="000000000001010001010001";
String s6off="000000000001010001010100";

String s7on="000000000000010101010001";
String s7off="000000000000010101010100";

String s8on="000001000000010101010001";
String s8off="000001000000010101010100";

String s9on="000001000001000101010001";
String s9off="000001000001000101010100";

String s10on="000001000001010001010001";
String s10off="000001000001010001010100";




void setup() {

  Serial.begin(9600);
  
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(3);

  // Optional set pulse length.
  mySwitch.setPulseLength(514);
  
  // Optional set protocol (default is 1, will work for most outlets)
  mySwitch.setProtocol(5);
  
  // Optional set number of transmission repetitions.
  // mySwitch.setRepeatTransmit(15);
  
}

void loop() {

  /* See Example: TypeA_WithDIPSwitches */
  /*mySwitch.switchOn("11111", "00010");
  delay(1000);
  mySwitch.switchOn("11111", "00010");
  delay(1000);
*/
  /* Same switch as above, but using decimal code */
  /*mySwitch.send(5393, 24);
  delay(1000);  
  mySwitch.send(5396, 24);
  delay(1000);  
*/
  /* Same switch as above, but using binary code */
  mySwitch.send("110010010011101010110000");
  delay(1000);  
  mySwitch.send("110011101010010010010000");
  delay(1000);

  /* Same switch as above, but tri-state code */ 
  /*mySwitch.sendTriState("00000FFF0F0F");
  delay(1000);  
  mySwitch.sendTriState("00000FFF0FF0");
  delay(1000);
*/
//  delay(5000);
}
