#include"WiFi.h"
#include"Webserver.h"
#include"ArduinoJson.h"
Webserver server;
#include "WiFi.h"
#include "WebServer.h"
#include "ArduinoJson.h"

WebServer server;
char* ssid = "ONEPLUS";
char* password = "12345678900";

void setup(){
    WiFi.begin(ssid,password);
    Serial.begin(115200);
    while(WiFi.status()!=WL_CONNECTED){
                Serial.print(".");
                delay(500);
              }
    Serial.println("");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    server.on("/",handleIndex);
    server.begin();
}

void loop(){
    server.handleClient();
}

void handleIndex(){
    // Send a JSON-formatted request with key "type" and value "request"
    // then parse the JSON-formatted response with keys "gas" and "distance"
    DynamicJsonDocument doc(1024);
    double hb1 = 0, hb2  = 0, hb3 = 0, flg = 0;
    // Sending the request
    doc["type"] = "request";
    serializeJson(doc,Serial);
    // Reading the response
    boolean messageReady = false;
    String message = "";
    while(messageReady == false) { 
// blocking but that's ok
              if(Serial.available()) {
                    message = Serial.readString();
                    messageReady = true;
                  }
            }
    // Attempt to deserialize the JSON-formatted message
    DeserializationError error = deserializeJson(doc,message);
    if(error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
      }
        hb1 = doc["hb1"];
        hb2 = doc["hb2"];
        hb3 = doc["hb3"];
        flg = doc["RCWL"];
    // Prepare the data for serving it over HTTP
    String output = String(hb1) + "," + String(hb2) + "," + String(hb3) + "," + String(flg);
    // Serve the data as plain text, for example
    server.send(200,"text/plain",output);
}
