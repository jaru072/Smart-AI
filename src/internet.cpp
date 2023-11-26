#include <Arduino.h>
// #include "WiFi.h"
#include <Config.h>

String ssid = "HUAWEI MediaPad T3 10";
String pass = "00000000";
int TotalconnectCount = 20;
//******************************************INTERNET CONNECT****************************************//
void connectInternet(void) { 
  int connectCount = 0;  // Scan หาไวไฟ

  WiFi.mode(WIFI_STA);WiFi.begin(ssid.c_str(), pass.c_str()); // 

  while (WiFi.status() != WL_CONNECTED) {connectCount++;Serial.print(".");delay(500);
    if (connectCount > TotalconnectCount) {Wifi_Connect = false ;break;}
  }

  if (WiFi.status() == WL_CONNECTED) { Wifi_Connect = true ;
    Serial.println(""); Serial.print("WiFi connected IP address: "); Serial.print(WiFi.localIP());Serial.print(" , MAC Address: ");Serial.println(WiFi.macAddress()); 
  }else{
    Serial.println("Wifi_Connect = false");Wifi_Connect = false; // SetSendReceive(); // Setup ส่ง-รับ ผ่าน Mac Address 
  }
}

void check_ssid(void){
  int n = WiFi.scanNetworks();
  if (n == 0) {Serial.println("No networks found");} else {Serial.print(n);Serial.println(" Networks found");
    for (int i = 0; i < n; ++i) {Serial.print(i + 1);Serial.print(": ");Serial.print(WiFi.SSID(i));Serial.print(" (");Serial.print(WiFi.RSSI(i));ssid = WiFi.SSID(i);}
  }
  Serial.println(""); delay(500);  
}
