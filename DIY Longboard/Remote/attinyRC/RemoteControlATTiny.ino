#include <VirtualWire.h>

int button1=4;
int button2=5;
int potentiometer=2;
int tx_pin = 4; 
int curval = 790;
int IDNumber = 111;

int escval=790; 
int escoldval=790;
int r1, r2, potval;

typedef struct sendData 
{
  int    id;
  int    data1;
  int    data2;
};

// the setup routine runs once when you press reset:
void setup() {
 // Serial.begin(9600);
 // Serial.println("setup - start");
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(potentiometer, INPUT);
  vw_set_tx_pin(tx_pin);
  vw_setup(2000);   // Bits per sec
 // Serial.println("setup - end");
}

// the loop routine runs over and over again forever:
void loop() 
{  
  struct sendData payload;
  r1 = digitalRead(button1);  
  r2 = digitalRead(button2);
  potval=analogRead(potentiometer);
  escval = map(potval, 500, 1000, 800, 2000);

  
  if (r2==HIGH)// if r2 is pressed - hold speed
  {
    curval = curval;
  }
  else
  {
    if (r1==HIGH) // if button 1 ist pressed and only then increase speed
    {
      if (escval>curval)
        curval=curval+10;
      else
        curval=curval-10;  
    }
    else // decrease if button 1 is not pressed
    {
      if (curval >= 800)
        curval=curval-10;  
    }
  }  
  

  /*Serial.print("Status Button1: ");
  Serial.println(r1);
  Serial.print("Status Button2: ");
  Serial.println(r2);
  */
  //Serial.print("Value Potentiometer: ");
  //Serial.println(potval);
  // int val = map(potval, 500, 1000, 800, 2000);
  //Serial.print("Speed for ESC: ");
  //Serial.println(escval);
  
  //Serial.print("Speed for ESC (throttled): ");
  //Serial.println(curval);


  payload.data1 = r1;
  payload.data2 = r2;
  payload.id = IDNumber;
  
  

 // Serial.println("start send");
  vw_send((uint8_t *)&payload, sizeof(payload));
  vw_wait_tx();  // Wait for message to finish
  //Serial.println("end send");

  escoldval=escval;
  delay(50);

  
  
}
