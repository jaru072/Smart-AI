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
bool LFirst_Song,Leof_mp3,Lspeech,LSDcard = false;

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

int N = 1;
String ASpeech[] = {"","โชคดีที่รอดมาอีก 1 วัน, ขอสรรพสัตว์ทั้งหลายจงมีความสุข","อันตัวเรานั้น ตายแน่ ตายแน่","ฉันตื่นนอน ตีห้า, ทำวัตรเช้า ตีห้าครึ่ง","6 โมงเช้า ออกกำลังกาย, นั่งสมาธิ","7 โมง ทำความสะอาดกุฏิ , ดูข่าว","8 โมงเช้า ไปรับภัตตาหาร, ฉันเช้า 9 โมง"};
int TotalASpeech = (sizeof(ASpeech) / sizeof(ASpeech[0])) - 1;

void PlayAuto() { if (NAutoFolder == 3 or NAutoFolder == 4 or NAutoFolder == 5) {NAutoFolder = 6;NumberFile = 1;} // ไม่เปิดนำนังสมาธิ
  String Autofile = AFolderFile[NAutoFolder][NumberFile];Serial.print("Autofile = ");Serial.println(Autofile);
  if (Autofile == "") {NAutoFolder= 1;NumberFile = 1;Autofile = AFolderFile[NAutoFolder][NumberFile];}
  
  audio.connecttoSD( Autofile.c_str() ); NumberFile++;  //audio.loop();

  if (AFolderFile[NAutoFolder][NumberFile] == ""){ 
    NumberFile = 1; NAutoFolder++;
  }
}

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

void Check_SDcard(int NCount) {
  if (!SD.begin(SD_CS)) {LSDcard = false; //root = false;
    if (NCount > 1) {Serial.println("initialization failed. Things to check:");Serial.println("Please insert SD Card");}
    for (int i=1;i <= NCount;i++) { 
      if (!SD.begin(SD_CS)){
        Serial.print(".");delay(1000);
      } else {
        Serial.println("initialization done.");LSDcard = true;root = SD.open("/"); Serial.println(root); 
        if (NCount != 0 or TotalASpeech == 0) {printDirectory(root, 0); Serial.println("Directory SD-Card done!");Serial.print("Root Directory = ");Serial.println(root);break;}
      }
    }
  } else{ // Serial.println(root);
    if (root == 0) {
      Serial.println("initialization done.");LSDcard = true;root = SD.open("/");  
      if (NCount != 0 or TotalASpeech == 0) {printDirectory(root, 0);} //Serial.print("Root Directory = ");Serial.println(root);}
    }
  }
}

//........................................ Void for Control Audio .............................................//
void audio_info(const char *info)
{
  Serial.print("info        "); Serial.println(info);String String_info = info;
  if (String_info.endsWith("failed!")) {Serial.println("Can not open URL");LOpenURL = false;}
  if (String_info.endsWith("with PSRAM!")) {Leof_mp3 = true;}
}

void audio_id3data(const char *info)
{ //id3 metadata
  Serial.print("id3data     "); Serial.println(info);
}

void audio_eof_mp3(const char *info)
{ //end of file
  Leof_mp3 = true;
  Serial.print("Leof_mp3 = ");Serial.print(Leof_mp3); Serial.print(" eof_mp3     "); Serial.println(info);
}

void audio_showstation(const char *info)
{
  Serial.print("station     "); Serial.println(info);
}

void audio_showstreaminfo(const char *info)
{
  Serial.print("streaminfo  "); Serial.println(info);
}

void audio_showstreamtitle(const char *info)
{
  Serial.print("streamtitle "); Serial.println(info);
}

void audio_bitrate(const char *info)
{
  Serial.print("bitrate     "); Serial.println(info);
}

void audio_commercial(const char *info)
{ //duração
  Serial.print("commercial  "); Serial.println(info);
}

void audio_icyurl(const char *info)
{ //homepage
  Serial.print("icyurl      "); Serial.println(info);
}

void audio_lasthost(const char *info)
{ //stream URL played
  Serial.print("lasthost    "); Serial.println(info);
}

void audio_eof_speech(const char *info)
{
  Lspeech = true;
  Serial.print("eof_speech test "); Serial.println(info);
}