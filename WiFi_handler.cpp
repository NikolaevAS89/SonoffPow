#include "WiFi_handler.h"

/**
 * Старт WiFi
 */
void WiFi_begin(void) {
  if(isDebug) {
    Serial.print("WiFi_begin()\n");
  }
  WiFi.mode(WIFI_STA);
  char ssid_ap[] = "SmeAP";
  char pass_ap[] = "1qazxsw23edc";
  /*
  if(WiFi_CreateAP(ssid_ap, pass_ap)) {
    Serial.print("AP created\n");
  } else {
    Serial.print("AP not created\n");
  }*/
  // Подключаемся к WiFi
  char ssid_s[] = "Columbia";
  char pass_s[] = "****";
  if( !WiFi_ConnectToAP(ssid_s, pass_s) ) {
    Serial.print("Connected\n");
  } else {
    Serial.print("Not Connected\n");
  }
}

bool WiFi_CreateAP(char* ssid, char* pass) {
  if(isDebug){
    Serial.print("WiFi_CreateAP(");
    Serial.print(ssid);
    Serial.print(", ");
    Serial.print(pass);
    Serial.print(")\n");
  }
  return WiFi.softAP(ssid, pass, 1);
}

/**
 * Соединение с WiFi
 */
bool WiFi_ConnectToAP(char* ssid, char* pass) {
  if(isDebug){
    Serial.print("WiFi_connect(");
    Serial.print(ssid);
    Serial.print(", ");
    Serial.print(pass);
    Serial.print(")\n");
  }
  // Пытаемся соединиться с точкой доступа
  WiFi.begin(ssid, pass, 0);
  //TODO WiFi.config(ip, gateway, subnet);
  // Максиммум N раз проверка соединения
  for ( int j = 0; j < 15; j++ ) {
    delay(1000);
    if (WiFi.status() == WL_CONNECTED) {
      if( isDebug ) {
        Serial.print("\nWiFi connect: ");
        Serial.print(WiFi.localIP());
        Serial.print("/");
        Serial.print(WiFi.subnetMask());
        Serial.print("/");
        Serial.println(WiFi.gatewayIP());
        Serial.print("\n");
      }
      return true;
    }
    delay(1000);
    Serial.print(WiFi.status());
  }
  Serial.print("Connect WiFi failed ...\n");
  return false;
}

/**
 * Найти список WiFi сетей
 */
SSID* WiFi_list(void){
  if(isDebug){
    Serial.print("WiFi_list()\n");
  }
  int n = WiFi.scanNetworks();
  if( n <= 0 ){
    return WIFI_NOT_FOUND;
  } else {
    SSID* finded = new SSID[n];
    for (int i=0; i<n; i++){
      finded[i].vSSID=WiFi.SSID(i);
      finded[i].vRSSI=WiFi.RSSI(i);
      finded[i].vEncryptionType=(WiFi.encryptionType(i) == ENC_TYPE_NONE)?' ':'*';
    }
    return finded;
  }
}
