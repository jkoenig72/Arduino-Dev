// Enable debug prints to serial monitor
#define MY_DEBUG
#define MY_RADIO_NRF24
#define MY_NODE_ID 9  

#include <MySensors.h>
#include <Wire.h> // must be included here so that Arduino library object file references work
//#include <Sleep_n0m1.h>
#include <RtcDS3231.h>
RtcDS3231<TwoWire> Rtc(Wire);

//Sleep sleep2;


// Setting up format for reading 3 soil sensors
#define NUM_READS 10    // Number of sensor reads for filtering
#define CHILD_ID 0
#define countof(a) (sizeof(a) / sizeof(a[0]))
#define RtcSquareWavePin 2 
#define RtcSquareWaveInterrupt 0
#define pumpDuration 4000

//#define intPin 2

MyMessage msg(CHILD_ID, V_LEVEL);
unsigned long SLEEP_TIME = 30000; // Sleep time between reads (in milliseconds)

long buffer[NUM_READS];
int sensorPin = 0;    // select the input pin for the moist sensor, analog
int pumpPin = 7;

volatile uint16_t interuptCount = 0;
volatile bool interuptFlag = false;

void InteruptServiceRoutine()
{
    // since this interupted any other running code,
    // don't do anything that takes long and especially avoid
    // any communications calls within this routine
    interuptCount++;
    interuptFlag = true;
}

void setup()
{
   Serial.begin(115200);

   pinMode(pumpPin, OUTPUT);

  // set the interupt pin to input mode
    pinMode(RtcSquareWavePin, INPUT);

    //--------RTC SETUP ------------
    Rtc.Begin();
    // if you are using ESP-01 then uncomment the line below to reset the pins to
    // the available pins for SDA, SCL
    // Wire.begin(0, 2); // due to limited pins, use pin 0 and 2 for SDA, SCL

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

    if (!Rtc.IsDateTimeValid()) 
    {
        Serial.println("RTC lost confidence in the DateTime!");
        Rtc.SetDateTime(compiled);
    }

    if (!Rtc.GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled) 
    {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    }
    
    Rtc.Enable32kHzPin(false);
    Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmBoth); 

    // Alarm 1 set to trigger every day when 
    // the hours, minutes, and seconds match
    RtcDateTime alarmTime = now; // into the future
    DS3231AlarmOne alarm1(
            0,
            0,
            0, 
            0,
            DS3231AlarmOneControl_MinutesSecondsMatch );
    Rtc.SetAlarmOne(alarm1);

    // Alarm 2 set to trigger at the top of the minute
    DS3231AlarmTwo alarm2(
            0,
            0,
            0, 
            DS3231AlarmTwoControl_MinutesMatch);
    Rtc.SetAlarmTwo(alarm2);

    // throw away any old alarm state before we ran
    Rtc.LatchAlarmsTriggeredFlags();

    // setup external interupt 
    attachInterrupt(RtcSquareWaveInterrupt, InteruptServiceRoutine, FALLING);

}

void presentation()
{
	sendSketchInfo("Soil Moisture Sensor Reverse Polarity", "1.0");
	present(CHILD_ID, S_MOISTURE);
}

void loop()
{

  long ret = readValues();
	//send back the values
  send(msg.set((long int)ret));
	//delay until next measurement (msec)
  sleep(SLEEP_TIME);

    if (!Rtc.IsDateTimeValid()) 
    {
        Serial.println("RTC lost confidence in the DateTime!");
    }

    RtcDateTime now = Rtc.GetDateTime();

    printDateTime(now);
    Serial.println();

    // we only want to show time every 10 seconds
    // but we want to show responce to the interupt firing
    for (int timeCount = 0; timeCount < 20; timeCount++)
    {
        if (Alarmed())
        {
            Serial.print(">>Interupt Count: ");
            Serial.print(interuptCount);
            Serial.println("<<");
        }
        delay(500);
    }

    if (ret>300)
      pumpOn();


      
//  sleep2.pwrDownMode(); //set sleep mode

  //Sleep till interrupt pin equals a particular state.
  //In this case "low" is state 0.
//  sleep2.sleepInterrupt(2,LOW); //(interrupt Pin Number, interrupt State)
  
}

long readValues()
{
  long sum = 0;
  for (int i = 0; i < NUM_READS; i++) 
  {
    buffer[i] = analogRead(sensorPin);
    sum += buffer[i];
    Serial.print("buffer[");
    Serial.print(i);
    Serial.print("]: ");
    Serial.println(buffer[i]);  
    delay(100);
  }
  return (long)(sum / NUM_READS);
}

bool Alarmed()
{
    bool wasAlarmed = false;
    if (interuptFlag)  // check our flag that gets sets in the interupt
    {
        wasAlarmed = true;
        interuptFlag = false; // reset the flag
        
        // this gives us which alarms triggered and
        // then allows for others to trigger again
        DS3231AlarmFlag flag = Rtc.LatchAlarmsTriggeredFlags();

        if (flag & DS3231AlarmFlag_Alarm1)
        {
            Serial.println("alarm one triggered");
        }
        if (flag & DS3231AlarmFlag_Alarm2)
        {
            Serial.println("alarm two triggered");
        }
    }
    return wasAlarmed;
}

void printDateTime(const RtcDateTime& dt)
{
  char datestring[20];

  snprintf_P(datestring, 
      countof(datestring),
      PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
      dt.Month(),
      dt.Day(),
      dt.Year(),
      dt.Hour(),
      dt.Minute(),
      dt.Second() );
    Serial.print(datestring);
}

void pumpOn()
{
    Serial.println("Water ON");  
    digitalWrite(pumpPin, HIGH);
    delay(pumpDuration);
    digitalWrite(pumpPin, LOW);
    Serial.println("Water OFF");  
}


