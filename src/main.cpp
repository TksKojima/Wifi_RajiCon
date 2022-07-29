#include <Arduino.h>

#include  <WiFi.h>
#include  <WiFiClient.h>
#include  <WebServer.h>
#include  <ESPmDNS.h>

#include  "defhtml.h"
 
const char* ssid = "wifi";
const char* password = "12345678";
const int ledPin = A19;

int fow_input = 0;
int st_input = 0;

WebServer server(80);
const int led = 4;
void handleRoot() {

 server.send(200, "text/html", index_html);
 
}

void handleRC() {
//  NoDat = 0;
  for (int i = 0; i < server.args(); i++) {
    int Val_i = server.arg(i).toInt();
    Serial.print(server.argName(i) + "=" + server.arg(i) + ", ");
    
    switch (server.argName(i)[0]) {
      case 'F': fow_input = Val_i; break;
      case 'S': st_input  = Val_i; break;
    }
  }
  Serial.println();
  server.send(200, "text/plain", "\n\n\n");
}
 

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup_pwm(){
  ledcSetup(0, 12800, 8);
  ledcAttachPin(ledPin, 0);  
}

void loop_pwm(){
  static uint8_t brightness = 0;
  brightness = ( fow_input + 100 )/2;
  ledcWrite(0, brightness);

  
  dacWrite( 25, max( abs(fow_input), abs(st_input) ) * 255 /100 );

  delay(2);  
}

void loop(void) {
 server.handleClient();
 loop_pwm();

}

void setup(void) {
 pinMode(led, OUTPUT);
 digitalWrite(led, LOW);
 Serial.begin(115200);
 //WiFi.mode(WIFI_STA);
 WiFi.mode(WIFI_AP);
 //WiFi.begin(ssid, password);
 WiFi.softAP(ssid, password, 3, 0, 4);
   delay(200);

 WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));
   

 Serial.println("");
  Serial.print("AP IP address: ");
  Serial.print(WiFi.softAPIP());
  Serial.println();
   delay(100);
   
 if (MDNS.begin("esp32led")) {
   Serial.println("MDNS responder started");
 }
 server.on("/", handleRoot);
 server.on("/rc", handleRC);
 
 server.on("/inline", []() {
 server.send(200, "text/plain", "hello from esp8266!");
 });
 server.onNotFound(handleNotFound);
 server.begin();
 Serial.println("HTTP server started");

 setup_pwm();
 
}
