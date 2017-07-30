// Import required libraries
#include <ESP8266WiFi.h>
#include <aREST.h>
#include <RCSwitch.h>


// Create aREST instance
aREST rest = aREST();

// WiFi parameters
const char* ssid = "xyz";
const char* password = "xyz";
const char* host = "espRelay1";

IPAddress ip(192,168,10,39);
IPAddress gateway(192,168,10,1);
IPAddress subnet(255,255,255,0);

// The port to listen for incoming TCP connections
#define LISTEN_PORT           80
//#define LIGHTWEIGHT 1

// Create an instance of the server
WiFiServer server(LISTEN_PORT);


// Declare functions to be exposed to the API
int switch1Control(String command);
int switch2Control(String command);
int switch3Control(String command);
int switch4Control(String command);

RCSwitch mySwitch = RCSwitch();

void setup(void)
{
  // Start Serial
  Serial.begin(9600);

  mySwitch.enableTransmit(D2);
  mySwitch.setPulseLength(514);
  mySwitch.setProtocol(5);


  // Function to be exposed
  rest.function("switch1",switchControl1);
  rest.function("switch2",switchControl2);
  rest.function("switch3",switchControl3);
  rest.function("switch4",switchControl4);
  rest.set_id("000001");
  rest.set_name("esp8266_switchcontrol");

  WiFi.config(ip, gateway, subnet);
  WiFi.hostname(host);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(host);
  Serial.println("WiFi connected");
  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP()); 
}

void loop() {

  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);

}

// Custom function accessible by the API
int switchControl1(String command)
{
  Serial.println("switchControl1 start");
  int state = command.toInt();
  Serial.print("state: ");
  Serial.println(state);
  Serial.println("command: " + command);
  if (state == 1)
  {
    Serial.println("ON");
    mySwitch.send("000001000000010101010001");
  }
  else
  {
    Serial.println("OFF");
    mySwitch.send("000001000000010101010100");
  }
  Serial.println("switchControl1 end");
  return state;
}

// Custom function accessible by the API
int switchControl2(String command)
{
  Serial.println("switchControl2 start");
  int state = command.toInt();
  Serial.print("state: ");
  Serial.println(state);
  Serial.println("command: " + command);
  if (state == 1)
  {
    Serial.println("ON");
    mySwitch.send("000001000000010101010001");
  }
  else
  {
    Serial.println("OFF");
    mySwitch.send("000001000000010101010100");
  }
  Serial.println("switchControl2 end");
  return state;
}

// Custom function accessible by the API
int switchControl3(String command)
{
  Serial.println("switchControl3 start");
  int state = command.toInt();
  Serial.print("state: ");
  Serial.println(state);
  Serial.println("command: " + command);
  if (state == 1)
  {
    Serial.println("ON");
    mySwitch.send("000001000000010101010001");
  }
  else
  {
    Serial.println("OFF");
    mySwitch.send("000001000000010101010100");
  }
  Serial.println("switchControl3 end");
  return state;
}

// Custom function accessible by the API
int switchControl4(String command)
{
  Serial.println("switchControl4 start");
  int state = command.toInt();
  Serial.print("state: ");
  Serial.println(state);
  Serial.println("command: " + command);
  if (state == 1)
  {
    Serial.println("ON");
    mySwitch.send("000001000000010101010001");
  }
  else
  {
    Serial.println("OFF");
    mySwitch.send("000001000000010101010100");
  }
  Serial.println("switchControl4 end");
  return state;
}



