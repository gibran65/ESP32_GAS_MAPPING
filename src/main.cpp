#include <Arduino.h>
#include <WiFi.h>
#include <LittleFS.h>
#include <ESPAsyncWebServer.h>

// Config
const char* AP_SSID = "ESP32-GAS";
const char* AP_PASS = "12345678";

AsyncWebServer server(80);

float gasMap[12][24]; // Contoh data mapping

void setup() {
  Serial.begin(115200);
  
if(!LittleFS.begin()){  // Ganti dari SPIFFS.begin()
    Serial.println("Gagal mount LittleFS");
    return;
  }
  
  server.serveStatic("/", LittleFS, "/");  // Ganti parameter
}

  // WiFi AP Mode
  WiFi.softAP(AP_SSID, AP_PASS);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  // API Endpoint
  server.on("/api/map", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "{";
    for(int i=0; i<12; i++){
      json += "\"" + String(i) + "\":[";
      for(int j=0; j<24; j++){
        json += String(gasMap[i][j]);
        if(j<23) json += ",";
      }
      json += "]";
      if(i<11) json += ",";
    }
    json += "}";
    request->send(200, "application/json", json);
  });

  // Serve Web UI
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
  
  server.begin();
}

void loop() {}
