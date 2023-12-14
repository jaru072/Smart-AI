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

// int N = 1;
// String ASpeech[] = {"1","2","3","4","5","6","7","8","โชคดีที่รอดมาอีก 1 วัน, ขอสรรพสัตว์ทั้งหลายจงมีความสุข","อันตัวเรานั้น ตายแน่ ตายแน่","ฉันตื่นนอน ตีห้า, ทำวัตรเช้า ตีห้าครึ่ง","6 โมงเช้า ออกกำลังกาย, นั่งสมาธิ","7 โมง ทำความสะอาดกุฏิ , ดูข่าว","8 โมงเช้า ไปรับภัตตาหาร, ฉันเช้า 9 โมง"};
// int TotalASpeech = (sizeof(ASpeech) / sizeof(ASpeech[0])) - 1;

int count1,count2 = 0;

void printDirectory(File dir, int numTabs) {
  while (true) {File entry =  dir.openNextFile(); 
    if (! entry) {break;}
    // for (uint8_t i = 0; i < numTabs; i++) {Serial.print('\t');}
    String Fname = entry.name(); int Number = Fname.substring(1,3).toInt();
    if (Number <= 20) {   // นอกนั้นอาจมากกว่า 20 จะไม่เก็บในตัวแปรอเรย์
      if (Fname.endsWith(".mp3")||Fname.endsWith(".aac")||Fname.endsWith(".wav")||Fname.endsWith(".m4a")) {  // ||Fname.endsWith(".flac")
        FolderName = Fname.substring(0,Fname.lastIndexOf("/")); AFolderFile[NFolder][0] = FolderName; 
        // if (OldFolderName != FolderName) {Serial.println(AFolderFile[NFolder][0]);} //OldFolderName = FolderName;
        Serial.print("NFolder = ");Serial.print(NFolder);Serial.print(" NFile = ");Serial.print(NFile); 
        AFolderFile[NFolder][NFile] = "/"+OldFolderName+"/"+Fname; Serial.println(" "+ AFolderFile[NFolder][NFile]);  
        NFile++; 
      }

      if (entry.isDirectory()) { NFile=1;
        OldFolderName = entry.name();
        printDirectory(entry, numTabs + 1); NFolder++;
      } else {
        // Serial.print("\t\t");Serial.println(entry.size(), DEC); // files have sizes, directories do not
      }
    }else{Serial.print("--Break--"); Serial.println(Fname.substring(1,2).toInt()); break;}
    entry.close();
  }
}

void toggleLED(void * parameter){
  for(;;){ 
    digitalWrite(LED_BUILTIN, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);//Pause the task for 500ms
    digitalWrite(LED_BUILTIN, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);//Pause the task again for 500ms
  }
}
void uploadToAWS(void * parameter){
    // Implement your custom logic here
    // When you're done, call vTaskDelete. Don't forget this!
    vTaskDelete(NULL);
}
// This TaskHandle will allow 
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
          if (10 != 0 or TotalASpeech == 0) {printDirectory(root, 0); Serial.println("Directory SD-Card done!");Serial.print("Root Directory = ");Serial.println(root);break;}
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
// This TaskHandle will allow 
TaskHandle_t task2Handle = NULL;
void task2(void * parameter){
  for(;;){count2++;
    // Serial.print("Task 2 counter");Serial.println(count2);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void RTOS_Setup() {
  //............... Setup RTOS xTaskCreate ................//
  // xTaskCreate(task_Check_SDcard, "Task 1", 5000, NULL, 1, &task_Check_SDcardHandle);
  xTaskCreate(task2, "Task 2", 1000, NULL, 1, &task2Handle);
  // Function that should be called// Name of the task (for debugging)// Stack size (bytes)// Parameter to pass// Task priority// Task handle
  xTaskCreate(toggleLED, "Toggle LED", 1000, NULL, 1, NULL);  
  xTaskCreate(uploadToAWS, "Upload to AWS", 1000, NULL, 1, NULL);
  // vTaskDelete(NULL);
}

void Check_SDcard() {  
  xTaskCreate(task_Check_SDcard, "Check_SDcard", 5000, NULL, 1, &task_Check_SDcardHandle);
}