#include <Arduino.h>
#include <Config.h>
//Pinos de conexão do ESP32 e o módulo de cartão SD
#define SD_CS          5
#define SPI_MOSI      23
#define SPI_MISO      19
#define SPI_SCK       18

//Pinos de conexão do ESP32-I2S e o módulo I2S/DAC CJMCU 1334
#define I2S_DOUT      25
#define I2S_BCLK      27
#define I2S_LRC       26

const char* URL = "https://mp3.ffh.de/radioffh/hqlivestream.mp3";  //"https://tunein.com/radio/STAR-FM-s266883/";
String File_SD = "/006 ครบรอบ 138 ปี พระมงคลเทพมุนี.mp3";
File root;

unsigned long last_timer,last_Stopsong = millis();
int count = 0;
String AFolderFile[21][31] = { {}, {} }; // ตัวแปรอเรย์ 20 โฟลเดอร์/ละ 30 ไฟล์ แต่ต้องใช้ 21 Array
String ASong[100];
int TotalASong = 0;int ATotalASong[100];String S_info = "Check info";
bool LOpenURL = true;

String OldFolderName = "Old";
String FolderName = "FolderName";
int AFolderSong  = 0;
int NFolder,NFile = 0; 
int NAutoFolder = 1;
int NumberFile = 1;

String ssid = "HUAWEI MediaPad T3 10";
String pass = "00000000";
int TotalconnectCount = 20;
//******************************************INTERNET CONNECT****************************************//
TaskHandle_t Task_connectInternetHandle = NULL;
void Task_connectInternet(void * parameter) { 
  if (LscanNetworks == true) {  
    int connectCount = 0;  // Scan หาไวไฟ  
    WiFi.mode(WIFI_STA);WiFi.begin(ssid.c_str(), pass.c_str()); // 

    while (WiFi.status() != WL_CONNECTED) {connectCount++;Serial.print(".");
      vTaskDelay(500 / portTICK_PERIOD_MS);
      if (connectCount > TotalconnectCount) {Wifi_Connect = false ;break;}
    }

    if (WiFi.status() == WL_CONNECTED) { Wifi_Connect = true ;
      Serial.println(""); Serial.print("WiFi connected IP address: "); Serial.print(WiFi.localIP());Serial.print(" , MAC Address: ");Serial.println(WiFi.macAddress()); 
    }else{
      Serial.println("Wifi_Connect = false");Wifi_Connect = false; // SetSendReceive(); // Setup ส่ง-รับ ผ่าน Mac Address 
    }
  }
  vTaskDelete(NULL); 
}

TaskHandle_t Task_check_ssidHandle = NULL;
void Task_check_ssid(void * parameter) {
  // WiFi.mode(WIFI_STA);WiFi.disconnect();vTaskDelay(100 / portTICK_PERIOD_MS);
  for (int ii = 0; ii < 10; ii++) { 
    int n = WiFi.scanNetworks();
    if (n <= 0) {if(ii == 9){Serial.println("No networks found");}LscanNetworks = false;
      // vTaskDelay(200 / portTICK_PERIOD_MS);
    } else {
      Serial.print(n);Serial.println(" Networks found");LscanNetworks = true;
      for (int i = 0; i < n; ++i) {Serial.print(i + 1);Serial.print(": ");Serial.print(WiFi.SSID(i));Serial.print(" (");Serial.print(WiFi.RSSI(i));ssid = WiFi.SSID(i);}
      break;
    }
  }
  //Serial.println(""); vTaskDelay(500 / portTICK_PERIOD_MS);
  vTaskDelete(NULL); 
}

void Check_Wifi(int NConnect_Time) {
  // Serial.print(WiFi.status());Serial.println(" localIP = "+WiFi.localIP().toString());
  if (WiFi.status() != WL_CONNECTED or Wifi_Connect == false) {check_ssid(); }

  if (WiFi.localIP().toString() == "0.0.0.0" or WiFi.status() != WL_CONNECTED or Wifi_Connect == false) {
    connectInternet(); // ทำการเชื่อมต่อเน็ตจำนวน NConnect_Time ครั้ง 
    if (WiFi.localIP().toString() != "0.0.0.0") {
      Wifi_Connect = true ; Serial.print("Wifi Connected Ready IP address: ");Serial.println(WiFi.localIP()); Wifi_Connect = true ;
    }
  }
}

int count1,count2 = 0;
//..................Task Control SD Card ...........................//
void printDirectory(File dir, int numTabs) {
  while (true) {File entry =  dir.openNextFile(); 
    if (! entry) {break;}
    String Fname = entry.name(); int Number = Fname.substring(1,3).toInt();
    if (Number <= 20) {   // นอกนั้นอาจมากกว่า 20 จะไม่เก็บในตัวแปรอเรย์
      if (Fname.endsWith(".mp3")||Fname.endsWith(".aac")||Fname.endsWith(".wav")||Fname.endsWith(".m4a")) {  // ||Fname.endsWith(".flac")
        FolderName = Fname.substring(0,Fname.lastIndexOf("/")); AFolderFile[NFolder][0] = FolderName; 
        AFolderFile[NFolder][NFile] = "/"+OldFolderName+"/"+Fname; 
        NFile++; 
      }

      if (entry.isDirectory()) { NFile=1;
        OldFolderName = entry.name();
        printDirectory(entry, numTabs + 1); NFolder++;
      } 
    }else{
      break;
    }
    entry.close();
  }
}
TaskHandle_t task_Check_SDcardHandle = NULL;
void task_Check_SDcard(void * parameter){
  if (!SD.begin(SD_CS)) {LSDcard = false; //root = false;
    if (SD.exists(AFolderFile[1][1].c_str()) == 0) {  
      Serial.println("initialization failed. Things to check:");Serial.println("Please insert SD Card");
      for (int i=1;i <= 10;i++) { 
        if (!SD.begin(SD_CS)){
          Serial.print(".");vTaskDelay(1000 / portTICK_PERIOD_MS);
        } else {
          Serial.println("initialization done.");LSDcard = true;root = SD.open("/"); Serial.println(root); 
          printDirectory(root, 0); Serial.println("Directory SD-Card done!");Serial.print("Root Directory = ");Serial.println(root);break;
        }
      }
    }
  } else{   // Serial.println(root);
    // if (SD.exists("/Covid19 No Money.mp3") == 0){
    if (SD.exists(AFolderFile[1][1].c_str()) == 0){
        Serial.println("Not found file Please check SD card");
    }else{
      Serial.println("... SD Card Ready ...");
    }
    if (root == 0) {
      Serial.println("initialization done.");LSDcard = true;root = SD.open("/");  
      printDirectory(root, 0); //Serial.print("Root Directory = ");Serial.println(root);}
    }
  }
  vTaskDelete(NULL);
}
//............. Task Control Led blink every 500 ms ................//
void toggleLED(void * parameter){
  for(;;){ 
    digitalWrite(LED_BUILTIN, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);//Pause the task for 500ms
    digitalWrite(LED_BUILTIN, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);//Pause the task again for 500ms
  }
}
//................... Task Control uploadToAWS.....................//
void uploadToAWS(void * parameter){
    // Implement your custom logic here
    // When you're done, call vTaskDelete. Don't forget this!
    vTaskDelete(NULL);
}
//....................... Task Control task2 ......................//
TaskHandle_t task2Handle = NULL;
void task2(void * parameter){
  for(;;){count2++;
    // Serial.print("Task 2 counter");Serial.println(count2);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
//........................ xTaskCreate Main ........................//
void RTOS_Setup() {
  //............... Setup RTOS xTaskCreate ................//
  xTaskCreate(task2, "Task 2", 1000, NULL, 1, &task2Handle);
  // Function that should be called// Name of the task (for debugging)// Stack size (bytes)// Parameter to pass// Task priority// Task handle
  xTaskCreate(toggleLED, "Toggle LED", 1000, NULL, 1, NULL);  
  xTaskCreate(uploadToAWS, "Upload to AWS", 1000, NULL, 1, NULL);
  // vTaskDelete(NULL);
}
//.................. xTaskCreate Check_SDcard only .................//
void Check_SDcard() {  
  xTaskCreate(task_Check_SDcard, "Check SDcard", 3000, NULL, 3, &task_Check_SDcardHandle);
}

void connectInternet() {
  xTaskCreate(Task_connectInternet, "connectInternet", 2500, NULL, 1, &Task_connectInternetHandle);
}
void check_ssid() {
  xTaskCreate(Task_check_ssid, "check ssid", 2800, NULL, 2, &Task_check_ssidHandle);
}
