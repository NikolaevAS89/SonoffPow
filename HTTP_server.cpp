#include "HTTP_server.h"

void HTTP_begin(void){
    if(isDebug) {
        Serial.print("WiFi_begin()\n");
    }
    // Поднимаем WEB-сервер  
    server.on ( "/", HTTP_handleRoot );
    //here the list of headers to be recorded
    server.begin();
    Serial.println("HTTP server started");
}

void HTTP_loop(void) {
  server.handleClient();
}

void HTTP_handleRoot(void) {
  if(isDebug){
    Serial.print("HTTP_handleRoot()\n");
  }
  server.send(200, "text/plain", "hello from esp8266!");
}
