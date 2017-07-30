/*
sniffer -
*/

#include <RCSwitch.h>
#include <IRremote.h>



//------------------------------------------------------------------------------
// Tell IRremote which Arduino pin is connected to the IR Receiver (TSOP4838)
//

RCSwitch mySwitch433 = RCSwitch();
RCSwitch mySwitch315 = RCSwitch();

int recvPin = 11;
IRrecv irrecv(recvPin);

char menuSelection='0';

// the setup function runs once when you press reset or power the board
void setup() 
{
  Serial.begin(9600);   // Status message will be sent to PC at 9600 baud
  Serial.println("Initilize modules...");
  
  irrecv.enableIRIn();
  //delay(500);
  //mySwitch433.enableReceive(0);  
  
  // Receiver on interrupt 0 => that is pin #2
  //delay(500);
//  mySwitch315.enableReceive(1);  // Receiver on interrupt 1 => that is pin #3

  
  // initialize digital pin 13 as an output.
  //pinMode(13, OUTPUT);
  printMenu();  
}

// the loop function runs over and over again forever
void loop() 
{
  
  if (Serial.available())
  {
    char ch = Serial.read();
    if (ch == '4')
    {
      Serial.println("Radio sniffer 433mhz selected...");
      menuSelection='4';
      Serial.println("Init 433mhz...");
      mySwitch315.disableReceive();
      mySwitch433.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
      Serial.println("Done");
      sniffRadio433();
    }
    if (ch == '3')
    {
      Serial.println("Radio sniffer 315mhz selected...");
      menuSelection='3';
      Serial.println("Init 315mhz...");
      mySwitch433.disableReceive();
      mySwitch315.enableReceive(1);  // Receiver on interrupt 0 => that is pin #2
      Serial.println("Done");
      sniffRadio315();
    }
    if (ch == 'i')
    {
      Serial.println("IR sniffer selected...");
      menuSelection='i';
      sniffIR();
   }
    if (ch == 'b')
    {
      Serial.println("Back to main menu...");
      printMenu();
    }
  }
  if (menuSelection=='i')
  {
      //Serial.println("wait for ir");
    sniffIR();
  }
  if (menuSelection=='3')
  {
      //Serial.println("wait for 315");
    sniffRadio315();
  }

  if (menuSelection=='4')
  {
      //Serial.println("wait for 433");
    sniffRadio433();
  }
  
}

//+=============================================================================
// Print main menu via serial
//
void printMenu()
{
  Serial.println("*************************");
  Serial.println("* IR and Radio sniiffer *");
  Serial.println("*************************");
  Serial.println("Please select option:");
  Serial.println("4 - radio sniffer for 433mhz");
  Serial.println("3 - radio sniffer for 315mhz");
  Serial.println("i - IR sniffer for IR remotes");
  Serial.println("b - back to this main menu");
}

//+=============================================================================
// Display IR code
//
void  ircode (decode_results *results)
{
  // Panasonic has an Address
  if (results->decode_type == PANASONIC) {
    Serial.print(results->address, HEX);
    Serial.print(":");
  }

  // Print Code
  Serial.print(results->value, HEX);
}

//+=============================================================================
// Display encoding type
//
void  encoding (decode_results *results)
{
  switch (results->decode_type) {
    default:
    case UNKNOWN:      Serial.print("UNKNOWN");       break ;
    case NEC:          Serial.print("NEC");           break ;
    case SONY:         Serial.print("SONY");          break ;
    case RC5:          Serial.print("RC5");           break ;
    case RC6:          Serial.print("RC6");           break ;
    case DISH:         Serial.print("DISH");          break ;
    case SHARP:        Serial.print("SHARP");         break ;
    case JVC:          Serial.print("JVC");           break ;
    case SANYO:        Serial.print("SANYO");         break ;
    case MITSUBISHI:   Serial.print("MITSUBISHI");    break ;
    case SAMSUNG:      Serial.print("SAMSUNG");       break ;
    case LG:           Serial.print("LG");            break ;
    case WHYNTER:      Serial.print("WHYNTER");       break ;
    case AIWA_RC_T501: Serial.print("AIWA_RC_T501");  break ;
    case PANASONIC:    Serial.print("PANASONIC");     break ;
    case DENON:        Serial.print("Denon");         break ;
  }
}

//+=============================================================================
// Dump out the decode_results structure.
//
void  dumpInfo (decode_results *results)
{
  // Check if the buffer overflowed
  if (results->overflow) {
    Serial.println("IR code too long. Edit IRremoteInt.h and increase RAWLEN");
    return;
  }

  // Show Encoding standard
  Serial.print("Encoding  : ");
  encoding(results);
  Serial.println("");

  // Show Code & length
  Serial.print("Code      : ");
  ircode(results);
  Serial.print(" (");
  Serial.print(results->bits, DEC);
  Serial.println(" bits)");
}

//+=============================================================================
// Dump out the decode_results structure.
//
void  dumpRaw (decode_results *results)
{
  // Print Raw data
  Serial.print("Timing[");
  Serial.print(results->rawlen-1, DEC);
  Serial.println("]: ");

  for (int i = 1;  i < results->rawlen;  i++) {
    unsigned long  x = results->rawbuf[i] * USECPERTICK;
    if (!(i & 1)) {  // even
      Serial.print("-");
      if (x < 1000)  Serial.print(" ") ;
      if (x < 100)   Serial.print(" ") ;
      Serial.print(x, DEC);
    } else {  // odd
      Serial.print("     ");
      Serial.print("+");
      if (x < 1000)  Serial.print(" ") ;
      if (x < 100)   Serial.print(" ") ;
      Serial.print(x, DEC);
      if (i < results->rawlen-1) Serial.print(", "); //',' not needed for last one
    }
    if (!(i % 8))  Serial.println("");
  }
  Serial.println("");                    // Newline
}

//+=============================================================================
// Dump out the decode_results structure.
//
void  dumpCode (decode_results *results)
{
  // Start declaration
  Serial.print("unsigned int  ");          // variable type
  Serial.print("rawData[");                // array name
  Serial.print(results->rawlen - 1, DEC);  // array size
  Serial.print("] = {");                   // Start declaration

  // Dump data
  for (int i = 1;  i < results->rawlen;  i++) {
    Serial.print(results->rawbuf[i] * USECPERTICK, DEC);
    if ( i < results->rawlen-1 ) Serial.print(","); // ',' not needed on last one
    if (!(i & 1))  Serial.print(" ");
  }

  // End declaration
  Serial.print("};");  // 

  // Comment
  Serial.print("  // ");
  encoding(results);
  Serial.print(" ");
  ircode(results);

  // Newline
  Serial.println("");

  // Now dump "known" codes
  if (results->decode_type != UNKNOWN) {

    // Some protocols have an address
    if (results->decode_type == PANASONIC) {
      Serial.print("unsigned int  addr = 0x");
      Serial.print(results->address, HEX);
      Serial.println(";");
    }

    // All protocols have data
    Serial.print("unsigned int  data = 0x");
    Serial.print(results->value, HEX);
    Serial.println(";");
  }
}

//+=============================================================================
// The repeating section of the code
//
void sniffIR()
{  
  decode_results  results;        // Somewhere to store the results
  if (irrecv.decode(&results)) 
  {  // Grab an IR code
    dumpInfo(&results);           // Output the results
    dumpRaw(&results);            // Output the results in RAW format
    dumpCode(&results);           // Output the results as source code
    Serial.println("");           // Blank line between entries
    irrecv.resume();              // Prepare for the next value
  }
  if (Serial.available())
  {
    char ch = Serial.read();
    if (ch == 'b')
    {
      Serial.println("Trigger - Back to main menu...");
      menuSelection='b';
      printMenu();
    }
  }
  //Serial.println("sniffIR - end");
}

//+=============================================================================
// The repeating section of the code
//
void sniffRadio433()
{
  if (mySwitch433.available()) 
  {
    output(mySwitch433.getReceivedValue(), mySwitch433.getReceivedBitlength(), mySwitch433.getReceivedDelay(), mySwitch433.getReceivedRawdata(),mySwitch433.getReceivedProtocol());
    mySwitch433.resetAvailable();
  }
  if (Serial.available())
  {
    char ch = Serial.read();
    if (ch == 'b')
    {
      Serial.println("Trigger - Back to main menu...");
      menuSelection='b';
    }
  }
}

void sniffRadio315()
{
  if (mySwitch315.available())
  {
    output(mySwitch315.getReceivedValue(), mySwitch315.getReceivedBitlength(), mySwitch315.getReceivedDelay(), mySwitch315.getReceivedRawdata(),mySwitch315.getReceivedProtocol());
    mySwitch315.resetAvailable();
  }

  if (Serial.available())
  {
    char ch = Serial.read();
    if (ch == 'b')
    {
      Serial.println("Trigger - Back to main menu...");
      menuSelection='b';
    }
  }
}

void output(unsigned long decimal, unsigned int length, unsigned int delay, unsigned int* raw, unsigned int protocol) {

  if (decimal == 0) {
    Serial.print("Unknown encoding.");
  } else {
    char* b = dec2binWzerofill(decimal, length);
    Serial.print("Decimal: ");
    Serial.print(decimal);
    Serial.print(" (");
    Serial.print( length );
    Serial.print("Bit) Binary: ");
    Serial.print( b );
    Serial.print(" Tri-State: ");
    Serial.print( bin2tristate( b) );
    Serial.print(" PulseLength: ");
    Serial.print(delay);
    Serial.print(" microseconds");
    Serial.print(" Protocol: ");
    Serial.println(protocol);
  }
  
  Serial.print("Raw data: ");
  for (int i=0; i<= length*2; i++) {
    Serial.print(raw[i]);
    Serial.print(",");
  }
  Serial.println();
  Serial.println();
}


static char* bin2tristate(char* bin) {
  char returnValue[50];
  int pos = 0;
  int pos2 = 0;
  while (bin[pos]!='\0' && bin[pos+1]!='\0') {
    if (bin[pos]=='0' && bin[pos+1]=='0') {
      returnValue[pos2] = '0';
    } else if (bin[pos]=='1' && bin[pos+1]=='1') {
      returnValue[pos2] = '1';
    } else if (bin[pos]=='0' && bin[pos+1]=='1') {
      returnValue[pos2] = 'F';
    } else {
      return "not applicable";
    }
    pos = pos+2;
    pos2++;
  }
  returnValue[pos2] = '\0';
  return returnValue;
}

static char * dec2binWzerofill(unsigned long Dec, unsigned int bitLength){
  static char bin[64]; 
  unsigned int i=0;

  while (Dec > 0) {
    bin[32+i++] = (Dec & 1 > 0) ? '1' : '0';
    Dec = Dec >> 1;
  }

  for (unsigned int j = 0; j< bitLength; j++) {
    if (j >= bitLength - i) {
      bin[j] = bin[ 31 + i - (j - (bitLength - i)) ];
    }else {
      bin[j] = '0';
    }
  }
  bin[bitLength] = '\0';
  
  return bin;
}



