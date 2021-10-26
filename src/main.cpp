#include <Arduino.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

const char* ssid = "1118-2.4G";
const char* password = "kpu123456!";

const int Relay = 15;

ESP8266WebServer server(80);

void handleRoot(){
  String message = (server.method() == HTTP_GET)?"GET":"POST";
  message +=" "+server.uri()+"\n";
  for(uint8_t i=0; i<server.args(); i++){
    message +=" "+server.argName(i)+" : "+server.arg(i)+"\n";
  }
  message += "\nHello form ESP8266!\n";
  server.send(200,"text/plain",message);
}
  
void handleNotFound(){
    String message = "File Not Found\n\n";
    server.send(404,"test/plain",message);
}

void mypage(){
    String message = "<html><head><meta charset=\"utf-8\"><title>Test</title></head>"
                    "<body>"
                    "<head>my page<head>"
                    " IoT 기초 "
                    "</body></html>";
    server.send(200,"text/html",message);
}

void Relay_on(){
  Serial.println("Relay_On");
  digitalWrite(Relay, HIGH);
  String message = "Realy ON";
  server.send(200,"text/plain",message);
}

void Relay_off(){
  Serial.println("Relay_Off");
  digitalWrite(Relay, LOW);
  String message = "Realy OFF";
  server.send(200,"text/plain",message);
}

/* Relay_toggle function
void Relay_toggle(){
  Serial.println("Relay_Toggle");
  digitalWrite(Relay, !digitalRead(Relay));
  String message = "Realy Toggle";
  server.send(200,"text/plain",message);
} */

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  Serial.println("Connecting...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.print("Connected to "); 
  Serial.println(ssid);

  Serial.print("IP address: "); 
  Serial.println(WiFi.localIP());

   if (MDNS.begin("wonazz")){
    Serial.println("MDNS responder started");
  }

  server.on("/",handleRoot);
  server.on("/inline",[](){
    server.send(200,"text/plain","Hello from the inline function \n");
  });
  server.on("/mypage",mypage);
  server.on("/Relay_on",Relay_on);
  server.on("/Relay_off",Relay_off);
  //server.on("/Relay_toggle",Relay_toggle);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
  pinMode(Relay,OUTPUT);
  digitalWrite(Relay,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  MDNS.update();
  server.handleClient();
}