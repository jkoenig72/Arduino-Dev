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
#include<Servo.h> 

#define DEBUGOUT

// Controller BT address
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
Servo esc; 
int PIN_ESC = 7;
int PIN_PIEZO = 6;


boolean ready = false;

unsigned long previousTime;
unsigned long elapsedTime;

// functions & procedures





bool printAngle, printTouch;
uint8_t oldL2Value, oldR2Value;

void setup()
{
  
  esc.attach(PIN_ESC); 
  Serial.begin(115200);

#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif

  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); // Halt
  }
  Serial.print(F("\r\nPS4 Bluetooth Library Started\r\n"));

  esc.writeMicroseconds(800);
  delay(5000);   

  tone(PIN_PIEZO, 1000, 1500);
  delay(1500);

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
#ifdef DEBUGOUT          
          Serial.println("after reboot - BEEEP!");
          Serial.print("Speed:");
          Serial.println(speed);
#endif          
          tone(PIN_PIEZO, 1000, 500);
          delay(1000);
          tone(PIN_PIEZO, 1000, 500);
          delay(1000);
          tone(PIN_PIEZO, 1000, 500);
          delay(1000);
          
        }
        else
        {
          // not used
        }
        break;
      case 0:    // nothing
#ifdef DEBUGOUT          
        Serial.println("nothing (speed slowly decrease)");
#endif          
        if (speed>min)
          speed-=25;
#ifdef DEBUGOUT          
        Serial.print("Speed:");
        Serial.println(speed);
#endif          
        break;
      case 80:    // up
        //Serial.println("up (speed increase)");
        if (speed<max)
          speed+=25;
#ifdef DEBUGOUT          
        Serial.print("Speed:");
        Serial.println(speed);
#endif          
        break;
      case 79:    // down
        //Serial.println("down (spped downcrease)");
        if (speed>min)
          speed-=50;
#ifdef DEBUGOUT          
        Serial.print("Speed:");
        Serial.println(speed);
#endif          
        break;
      case 82:    // right
#ifdef DEBUGOUT          
        Serial.println("right");
#endif          
        break;
      case 81:    // left
        //Serial.println("left (hold speed)");
#ifdef DEBUGOUT          
        Serial.print("Speed:");
        Serial.println(speed);
#endif          
        break;
      case 34:    // select
#ifdef DEBUGOUT          
        Serial.println("select");
#endif          
        break;
      case 39:    // start
#ifdef DEBUGOUT          
        Serial.println("start");
#endif          
        break;
      case 32:    // X
#ifdef DEBUGOUT          
        Serial.println("X");
#endif          
        break;
      case 30:    // A
#ifdef DEBUGOUT          
        Serial.println("A");
#endif          
        break;
      case 33:    // Y
#ifdef DEBUGOUT          
        Serial.println("Y");
#endif          
        break;
      case 31:    // B
#ifdef DEBUGOUT          
        Serial.println("B");
#endif          
        break;
      }
      
      // check ranges
      if (speed > max)
        speed = max;
      if (speed < min)
        speed = min; 

      delay(10);  
      esc.writeMicroseconds(speed);
      delay(10);
      /*  
      Serial.print(previousTime);
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
    if (Btd.connected==false) // lost buetooth connection?
    {
#ifdef DEBUGOUT          
        Serial.println("Connection lost - decrease speed until halt");
#endif          

       if (speed>min)
          speed-=50;  
      if (speed < min)
        speed = min; 

      delay(10);  
      esc.writeMicroseconds(speed);
      delay(10);   
    }

  }
}

