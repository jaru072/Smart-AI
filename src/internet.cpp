#include <Arduino.h>
#include <Config.h>

// String ssid = "HUAWEI MediaPad T3 10";
// String pass = "00000000";
// int TotalconnectCount = 20;
// //******************************************INTERNET CONNECT****************************************//
// void connectInternet(int TotalconnectCount) { 
//   int connectCount = 0;  // Scan หาไวไฟ

//   WiFi.mode(WIFI_STA);WiFi.begin(ssid.c_str(), pass.c_str()); // 

//   while (WiFi.status() != WL_CONNECTED) {connectCount++;Serial.print(".");delay(500);
//     if (connectCount > TotalconnectCount) {Wifi_Connect = false ;break;}
//   }

//   if (WiFi.status() == WL_CONNECTED) { Wifi_Connect = true ;
//     Serial.println(""); Serial.print("WiFi connected IP address: "); Serial.print(WiFi.localIP());Serial.print(" , MAC Address: ");Serial.println(WiFi.macAddress()); 
//   }else{
//     Serial.println("Wifi_Connect = false");Wifi_Connect = false; // SetSendReceive(); // Setup ส่ง-รับ ผ่าน Mac Address 
//   }
// }

// void check_ssid() {
//   // WiFi.mode(WIFI_STA);WiFi.disconnect();delay(100);
//   int n = WiFi.scanNetworks();
//   if (n <= 0) {
//     Serial.println("No networks found");LscanNetworks = false;
//   } else {
//     LscanNetworks = true;Serial.print(n);Serial.println(" Networks found");
//     for (int i = 0; i < n; ++i) {Serial.print(i + 1);Serial.print(": ");Serial.print(WiFi.SSID(i));Serial.print(" (");Serial.print(WiFi.RSSI(i));ssid = WiFi.SSID(i);}
//   }
//   // Serial.println(""); delay(500);  
// }

// void Check_Wifi(int NConnect_Time) {
//   // Serial.print(WiFi.status());
//   // Serial.println(" localIP = "+WiFi.localIP().toString());
//   if (WiFi.status() != WL_CONNECTED or Wifi_Connect == false) {check_ssid(); }

//   if (WiFi.localIP().toString() == "0.0.0.0" or WiFi.status() != WL_CONNECTED) {
//     connectInternet(NConnect_Time); // ทำการเชื่อมต่อเน็ตจำนวน NConnect_Time ครั้ง 
//     if (WiFi.localIP().toString() != "0.0.0.0") {
//       Wifi_Connect = true ; //Serial.print("Wifi Connected Ready IP address: ");Serial.println(WiFi.localIP()); Wifi_Connect = true ;
//     }
//   }

// }
