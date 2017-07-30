/*
  To upload through terminal you can use: curl -F "image=@firmware.bin" esp8266-webupdate.local/update
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <RCSwitch.h>

const char* host = "esp8266Switch2";
const char* ssid = "xyz";
const char* password = "xyz";
IPAddress ip(192,168,10,38);

IPAddress gateway(192,168,10,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);
RCSwitch mySwitch = RCSwitch();

const char* serverIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='UPDATE'></form>";

void setup(void)
{
  Serial.begin(115200);

  mySwitch.enableTransmit(D2);
  mySwitch.setPulseLength(514);
  mySwitch.setProtocol(5);
  
  Serial.println();
  Serial.println("Booting Sketch...");
  WiFi.config(ip, gateway, subnet);
  WiFi.hostname(host);
  WiFi.begin(ssid, password);
  if(WiFi.waitForConnectResult() == WL_CONNECTED){
    MDNS.begin(host);
    server.on("/", HTTP_GET, [](){
      server.sendHeader("Connection", "close");
      server.sendHeader("Access-Control-Allow-Origin", "*");

      char combinedArray[sizeof(host) + sizeof(serverIndex) + 1];
      sprintf(combinedArray, "%s %s", host, serverIndex); // with word space
      
      server.send(200, "text/html", combinedArray);
    });
    server.on("/switch", switchArg); //Associate the handler function to the path
    server.on("/update", HTTP_POST, [](){
      server.sendHeader("Connection", "close");
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(200, "text/plain", (Update.hasError())?"FAIL":"OK");
      ESP.restart();
    },[](){
      HTTPUpload& upload = server.upload();
      if(upload.status == UPLOAD_FILE_START){
        Serial.setDebugOutput(true);
        WiFiUDP::stopAll();
        Serial.printf(" Update: %s\n", upload.filename.c_str());
        uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
        if(!Update.begin(maxSketchSpace)){//start with max available size
          Update.printError(Serial);
        }
      } else if(upload.status == UPLOAD_FILE_WRITE){
        if(Update.write(upload.buf, upload.currentSize) != upload.currentSize){
          Update.printError(Serial);
        }
      } else if(upload.status == UPLOAD_FILE_END){
        if(Update.end(true)){ //true to set the size to the current progress
          Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
          Update.printError(Serial);
        }
        Serial.setDebugOutput(false);
      }
      yield();
    });
    server.begin();
    MDNS.addService("http", "tcp", 80);
  
    Serial.printf("Ready!");
    Serial.printf("Open http://%s.local in your browser\n", host);
    Serial.println(WiFi.localIP()); 
  } else {
    Serial.println("WiFi Failed");
  }
}

void switchArg() 
{ 
  String message = "";
  if (server.arg("code")== "")
  {   //Parameter not found
      message = "code Argument not found";
  }
  else
  {     //Parameter found
    message = "code = ";
    message += server.arg("code");     //Gets the value of the query parameter
    message += "\n";
    char codeSeq[server.arg("code").length()+1];
    server.arg("code").toCharArray(codeSeq, server.arg("code").length()+1);
    mySwitch.send(codeSeq); 
  }
  server.send(200, "text/plain", message);          //Returns the HTTP response
}
 
void loop(void){
  server.handleClient();
  delay(1);
} 