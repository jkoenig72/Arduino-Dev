/*
 Example sketch for the PS4 Bluetooth library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <PS4BT.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>
#include <SoftReset.h>


uint8_t bta[6] = {0xE8,0x70,0x03,0x15,0x07,0x16};

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside
BTD Btd(&Usb, bta); // You have to create the Bluetooth Dongle instance like so

/* You can create the instance of the PS4BT class in two ways */
// This will start an inquiry and then pair with the PS4 controller - you only have to do this once
// You will need to hold down the PS and Share button at the same time, the PS4 controller will then start to blink rapidly indicating that it is in pairing mode
PS4BT PS4(&Btd, PAIR);

// controller vars

int speed = 800;
int up = 20;
int down = 20;
int max = 2000;
int min = 800;

boolean ready = false;

unsigned long previousTime;
unsigned long elapsedTime;

// functions & procedures




// After that you can simply create the instance like so and then press the PS button on the device
//PS4BT PS4(&Btd);

bool printAngle, printTouch;
uint8_t oldL2Value, oldR2Value;

void setup() {
  Serial.begin(115200);

#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif

  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); // Halt
  }
  Serial.print(F("\r\nPS4 Bluetooth Library Started\r\n"));

  previousTime = millis();
  
}
void loop() 
{
  Usb.Task();

  if (PS4.connected()) 
  {
    elapsedTime =  millis() - previousTime;
    if (Btd.connected!=false & (elapsedTime > 99 ))
    {  


      switch (PS4.getAnalogHat(LeftHatX)) {
      case 127:    // after reboot
        if (ready==false)
        {  
          ready = true;
          Serial.println("after reboot - BEEEP!");
          Serial.print("Speed:");
          Serial.println(speed);
        }
        else
        {
          /*Serial.println("after reboot - BEEEP!");
          Serial.print("Speed:");
          Serial.println(speed);*/
        }
        break;
      case 0:    // nothing
        Serial.println("nothing (speed slowly decrease)");
        if (speed>min)
          speed-=25;
        Serial.print("Speed:");
        Serial.println(speed);
        break;
      case 80:    // up
        Serial.println("up (speed increase)");
        if (speed<max)
          speed+=100;
        Serial.print("Speed:");
        Serial.println(speed);
        break;
      case 79:    // down
        Serial.println("down (spped downcrease)");
        if (speed>min)
          speed-=100;
        Serial.print("Speed:");
        Serial.println(speed);
        break;
      case 82:    // right
        Serial.println("right");
        break;
      case 81:    // left
        Serial.println("left (hold speed)");
        Serial.print("Speed:");
        Serial.println(speed);
        break;
      case 34:    // select
        Serial.println("select");
        break;
      case 39:    // start
        Serial.println("start");
        break;
      case 32:    // X
        Serial.println("X");
        break;
      case 30:    // A
        Serial.println("A");
        break;
      case 33:    // Y
        Serial.println("Y");
        break;
      case 31:    // B
        Serial.println("B");
        // soft_restart();
        break;
      }

  /*    Serial.print(previousTime);
      Serial.print(",");
      Serial.print(elapsedTime);
      Serial.print(":");
      Serial.print(" ControllerStatus: ");
      Serial.println(PS4.getAnalogHat(LeftHatX));
    */
      previousTime = millis();
      
      // check ranges
      if (speed > max)
        speed = max;
      if (speed < min)
        speed = min;  

      



      
    }
  }
}

