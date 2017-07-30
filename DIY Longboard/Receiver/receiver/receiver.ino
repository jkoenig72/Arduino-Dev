#include <VirtualWire.h>
#include <ServoTimer2.h>

int IDNumber = 111;
ServoTimer2 ESC;; 

typedef struct sendData 
{
  int    id;
  int    data1;
  int    data2;
  
};

void setup()
{
    Serial.begin(9600);	// Debugging only
    Serial.println("Setup - Start");

    ESC.attach(9); 
    ESC.write(800);
    delay(10000);

    // Initialise the IO and ISR
    vw_setup(2000);	 // Bits per sec
    vw_rx_start();       // Start the receiver PLL running
    
    Serial.println("Setup - End");
}

void loop()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  struct sendData receivedData;
  uint8_t rcvdSize = sizeof(receivedData);//Incoming data size 
  
  vw_wait_rx();// Start to Receive data now 

  if (vw_get_message((uint8_t *)&receivedData, &rcvdSize)) // check to see if anything has been received
  {
    Serial.print(rcvdSize, DEC);
    Serial.println(" bytes received!");

    Serial.print("ID:");
    Serial.println(receivedData.id);

    Serial.print("Data1:");
    Serial.println(receivedData.data1);
    Serial.print("Data2:");
    Serial.println(receivedData.data2);    

    ESC.write(receivedData.data1);
  }

 
  delay(50);
}
