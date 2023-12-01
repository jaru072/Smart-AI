#include <Arduino.h>
#include <Config.h>

#define ir_one 16753245   // ค่าปุ่มกด 0-9 * # OK สามเหลี่ยม บน ขวา ล่าง ซ้าย
#define ir_two 16736925
#define ir_three 16769565
#define ir_four 16720605
#define ir_five 16712445
#define ir_six 16761405
#define ir_seven 16769055
#define ir_eight 16754775
#define ir_nine 16748655
#define ir_zero 16750695
#define ir_star 16738455
#define ir_sharp 16756815
#define ir_ok 16726215
#define ir_top 16718055
#define ir_right 16734885
#define ir_bottom 16730805
#define ir_left 16716015

int Relay1 = 4;int Relay2 = 13;int Relay3 = 0;int Relay4 = 4;

//#include <time.h>
//...... Get Date Time from Internet
long timezone = 7;  // 2;
byte daysavetime = 0; // 1;
String CMoonPhase,CMoonPhaseThai,start_time_relay,CString = "";
String CDay,CMon,CYear,CWday,CDateTime,wdayName,CWdayThai,monthName = "";
int NAlarmClock,NMonth,NDay,Nmdaymonyear,NDoW = 0;
int NChange_Remote,NSleep,Total_last_Sleep = 0;
int NYear = 1970;
bool LTime_Between,Ltalk_Firsttime,LFirstOnly = false;
bool GetLocalTime(struct tm * info, uint32_t ms);

// Audio audio;
struct tm tmstruct ;

File file;
const char filename[] = "/recording.wav";
const int headerSize = 44;

//Pinos de conexão do ESP32 e o módulo de cartão SD
#define SD_CS          5
#define SPI_MOSI      23
#define SPI_MISO      19
#define SPI_SCK       18

//Pinos de conexão do ESP32-I2S e o módulo I2S/DAC CJMCU 1334
#define I2S_DOUT      25
#define I2S_BCLK      27
#define I2S_LRC       26

Audio audio;
WiFiMulti wifiMulti;

// #include <DHT.h>
#define DHTPIN 33  // 27 //Connect Out pin to D2 in NODE MCU
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);
float h,t = 0;

void sendDHT(void);
void sendDHT(void) {
  h = dht.readHumidity();
  t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {Serial.println("Failed to read from DHT sensor!");return;}
  Serial.print("  Temperature : ");Serial.print(t);
  Serial.print("  Humidity : ");Serial.println(h);
}

#include <driver/i2s.h>
#define I2S_WS 15
#define I2S_SD 32
#define I2S_SCK 14
#define I2S_PORT I2S_NUM_0
#define I2S_SAMPLE_RATE   (16000)
#define I2S_SAMPLE_BITS   (16)
#define I2S_READ_LEN      (16 * 1024)
#define RECORD_TIME       (20) //Seconds
#define I2S_CHANNEL_NUM   (1)
#define FLASH_RECORD_SIZE (I2S_CHANNEL_NUM * I2S_SAMPLE_RATE * I2S_SAMPLE_BITS / 8 * RECORD_TIME)

#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */

// put function declarations here:
int myFunction(int, int);

uint8_t hour    = 6;
uint8_t minute  = 59;
uint8_t sec     = 45;

bool f_time     = false;
int8_t timefile = -1;
char chbuf[100];

//................... ตัวแปร เก็บใน SPIFFS (in Ram of Board) ...........................//
String CSound,CPlay_Test = "";
bool Wifi_Connect,Lconfirm,LScheduled,LReplace_Config,LConnect_internet_Auto,LStartSong,LTime_Schedu,LPlayAuto,LTalk_Everytime = false;
int NVolume = 10;
int volume_old = NVolume;
String ATime[40][2];String Ascheduled[30][3];
const char * Replace_Config;
int NSammaArahang = 0;

#include <IRremote.h>
int RECV_PIN = 32;
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long last_Remote,last,last_Sleep = millis();

bool LcontrolBoard,Leof_speech = false;
bool LNumber_Sound = true;
String Control_Board = "";
int every_minute,NPlayAuto,FolderPlay,FilePlay,NSongMode = 0;
//........................................ Remote Control .................................................//
//... กดตัวเลขแล้วตามด้วย * เล่นเพลงอัตโนมัติโฟลเดอร์นั้น เช่น 5* 
//... กด # ควบคุมบอร์ดอื่น ควมคุมบอร์ดตัวเอง หรือ ทำให้ตัวเลขที่กดผ่านมาว่างเปล่า 
String timerelay,FolderNumber,FolderFileNumber,Str_FileNumber,Cevery_minute = ""; 
int colon_pos = 0;

void addnumber() {
  // เพิ่มตัวเลขให้ ตัวแปร start_time_relay
  if (results.value == ir_zero) {start_time_relay = (start_time_relay+"0");}if (results.value == ir_one) {start_time_relay = (start_time_relay+"1");}if (results.value == ir_two) {start_time_relay = (start_time_relay+"2");}if (results.value == ir_three) {start_time_relay =(start_time_relay+"3");}if (results.value == ir_four) {start_time_relay = (start_time_relay+"4");}if (results.value == ir_five) {start_time_relay = (start_time_relay+"5");}if (results.value == ir_six) {start_time_relay = (start_time_relay+"6");}if (results.value == ir_seven) {start_time_relay = (start_time_relay+"7");}if (results.value == ir_eight) {start_time_relay = (start_time_relay+"8");}if (results.value == ir_nine) {start_time_relay = (start_time_relay+"9");}if (results.value == ir_star) {start_time_relay = (start_time_relay+"*");} 
    //Serial.print(results.value);Serial.println(start_time_relay);
  // เฉพาะกดรีโมท ตัวเลข 0-9 และจะลบช่องว่าง กับ ดอกจัน ยกเลิก LPlayAuto เล่นเพลงอัตโนมัติ
  if (results.value == ir_zero || results.value == ir_one || results.value == ir_two || results.value == ir_three || results.value == ir_four || results.value == ir_five || results.value == ir_six || results.value == ir_seven || results.value == ir_eight || results.value == ir_nine) {
    start_time_relay.replace(" ","");start_time_relay.replace("*","");Serial.print("start_time_relay = ");Serial.println(start_time_relay);
    if (LNumber_Sound == true) {int FileNumber = start_time_relay.toInt(); 
      // Serial.println(FileNumber);
      if (FileNumber <= 1000) { //  and FileNumber > 0
        if (FileNumber <= 1000) {FolderNumber = "/99 Number 751-1000/";}if (FileNumber <= 750)  {FolderNumber = "/99 Number 501-750/";}if (FileNumber <= 500)  {FolderNumber = "/99 Number 251-500/";}if (FileNumber <= 250)  {FolderNumber = "/99 Number 1-250/";}
        Add_Zero(FileNumber);
        FolderFileNumber = FolderNumber+Str_FileNumber+".mp3";Serial.println(FolderFileNumber);
        audio.connecttoSD( FolderFileNumber.c_str() );
      }
    }
    LPlayAuto = false;N = TotalASpeech+1;
//     if (Wifi_Connect == true) {
//       if (start_time_relay.startsWith("0") and start_time_relay.toInt() >= 1){audio.stopSong();
//         //.......................... กดเลข 0 นำหน้า ตามด้วยตัวเลข .....................................//
//         if (start_time_relay.toInt() == 1){audio.connecttospeech("ควบคุม Plug ไฟ", "th");LcontrolBoard = true;Control_Board = "ควบคุม Plug ไฟ";}
//         if (start_time_relay.toInt() == 2){audio.connecttospeech("อ่านข้อมูลจาก Config", "th");readFile(SPIFFS, "/mydir/config.txt");} //readWord(SPIFFS, "/mydir/config.txt");
//         if (start_time_relay.toInt() == 3){audio.connecttospeech("เขียนข้อมูลลง Config", "th");Check_Replace_SPIFFS("every_minute = 2");}  // อ่านค่าจาก Rom ภายในบอร์ด แล้ว แทนที่ หรือ เพิ่ม        
// //        if (start_time_relay.toInt() == 4){audio.connecttospeech("ลบคำสั่งใน Config", "th");Check_Delete_SPIFFS("LTalk_Everytime=false");} // อ่านค่าจาก Rom ภายในบอร์ด แล้ว ลบออก       
          
//         if (start_time_relay.toInt() == 9){audio.connecttospeech("Save ตัวแปร Config ลงใน Ram ของ Board", "th");Save_Config(SPIFFS, "/mydir/config.txt");}  
//         start_time_relay = "";      
//         //.........................................................................................//
//       }      
//     }
    Check();
  }
}

void ControlBoard() {  
  if (LcontrolBoard == false) {; //Serial.println(results.value); // ควบคุม Board ของตัวเอง
    addnumber();  //ใส่ตัวเลข
    //...........................................กด OK ลูกศร บน ล่าง ซ้าย ขวา ...........................................//
    if (results.value == ir_ok) { start_time_relay.replace(" ","");N = TotalASpeech+1;last_Sleep = millis(); // ลบช่องว่างใน start_time_relay
      if (start_time_relay.startsWith("99")) { ; // AutoPlay Folder
        String CloopFolder = start_time_relay.substring(2,4);int NloopFolder = CloopFolder.toInt();
      }  

      if (start_time_relay.startsWith("*")) {
        NSongMode = 0;Serial.println(CWday+" "+CDay+"/"+CMon+"/"+CYear);//mp3.playMp3FolderTrack((CMon+CDay).toInt());if(CDay.toInt() < 10){waitMilliseconds(6000);}else{waitMilliseconds(7000);} myData.z = 0;
        if (Wifi_Connect == true) {audio.stopSong();
          audio.connecttospeech(CMoonPhaseThai.c_str(), "th");
        }else{ 
          int NWaxing = (CMoonPhase.substring(6,9)).toInt();
          if (NWaxing == 0) {
            MoonPhase(NYear+543,NMonth,NDay); NWaxing = (CMoonPhase.substring(6,9)).toInt(); // คำนวณข้างขึ้น ข้างแรม ครั้งเดียว
          } else {
            if(CMoonPhase.startsWith("Waxing")){NWaxing = (100+NWaxing);}else{NWaxing = (200+NWaxing);}// mp3.playFolderTrack(98, NWaxing);waitMilliseconds(5000);
          }
        }
        start_time_relay = ""; return;
      }
      if (start_time_relay.isEmpty()){  // ถ้ากด OK โดย start_time_relay = "" ว่างเปล่า
        LPlayAuto = !LPlayAuto; // ยกเลิก หรือ เล่น LPlayAuto
        audio.pauseResume();    // Pause , Resume
        Serial.print("PauseResume");Serial.print(" , LPlayAuto = ");Serial.println(LPlayAuto);N = TotalASpeech+1;return;  
      }

      if (start_time_relay.length() > 1) {
        if(Leof_speech == true) {Leof_speech = false;}else{audio.stopSong();}
        FolderPlay = start_time_relay.substring(0,1).toInt(); FilePlay = start_time_relay.substring(1,2).toInt();
        audio.connecttoSD( AFolderFile[FolderPlay][FilePlay].c_str() );
      }else { 
        if(Leof_speech == true) {Leof_speech = false;}else{
          // audio.stopSong();
        }  
        FilePlay = start_time_relay.substring(0,1).toInt();
        if (FilePlay > 0 and FilePlay < 7) {audio.connecttoSD( AFolderFile[1][FilePlay].c_str() );}
        if (FilePlay == 7){NAlarmClock = 0;LConnect_internet_Auto = !LConnect_internet_Auto;Serial.print("LConnect_internet_Auto = ");Serial.print(LConnect_internet_Auto);  //SetSendReceive(); Serial.println(" and SetSendReceive Again");
          if (LConnect_internet_Auto == true) {audio.connecttoSD("/99 Control Sound/011 Connect_Internet_Auto.mp3");}else{audio.connecttoSD("/99 Control Sound/012 NotConnect_Internet_Auto.mp3");}
        }
        if (FilePlay == 8) {if(every_minute < 6){every_minute++;if(every_minute == 6){every_minute = 10;}
        }else{Cevery_minute="00"+String(every_minute);audio.connecttoSD(("/99 Control Sound/"+Cevery_minute+" every_minute"+String(every_minute)+".mp3").c_str());
          if(every_minute == 60){every_minute = 1;}if(every_minute == 30){every_minute = 60;}if(every_minute == 20){every_minute = 30;}if(every_minute == 10){every_minute = 20;}}if (every_minute <= 5 || every_minute == 10 || every_minute == 20 || every_minute == 30 || every_minute == 60) {
            audio.connecttospeech(("บอกเวลาทุก "+String(every_minute)+" นาที").c_str(), "th");
          }Serial.println("every_minute = "+String(every_minute));
        }
        if (FilePlay == 9) {NPlayAuto = 0;Serial.println("NPlayAuto = 0");} //RecordWAV(); อัดเสียง และ ยกเลิกตั้งเวลา PlayAuto ให้เล่นเพลงให้จบ แล้วค่อยขึ้นเพลงต่อไป
        if (FilePlay == 0 and start_time_relay != "*") {Serial.print("LTalk_Everytime = ");Serial.println(!LTalk_Everytime);LTalk_Everytime = !LTalk_Everytime;
          if (LTalk_Everytime == true) {audio.connecttoSD("/99 Control Sound/016 LTalk_Everytime_True.mp3");}else{audio.connecttoSD("/99 Control Sound/015 LTalk_Everytime_False.mp3");}
        } 
      }
      Serial.println(start_time_relay); start_time_relay = ""; // delay(1000);
    }
  }else {;  // ควบคุม Board ของที่อื่น
    if (Control_Board == "ควบคุม Plug ไฟ"){ 
      if (results.value == ir_one){digitalWrite(Relay1,!digitalRead(Relay1));}            
      if (results.value == ir_two){digitalWrite(Relay2,!digitalRead(Relay2));}
      if (results.value==ir_three){digitalWrite(Relay3,!digitalRead(Relay3));}
      if (results.value== ir_four){digitalWrite(Relay4,!digitalRead(Relay4));}
      if (results.value== ir_five){digitalWrite(Relay1, LOW);digitalWrite(Relay2, LOW);digitalWrite(Relay3, LOW);digitalWrite(Relay4, LOW);}      
      // myData.z = 0;
    }else{  
      // if (results.value == ir_one)  {Serial.println("Switch 1 ON-OFF");  Send(0,0,1,0);}
      // if (results.value == ir_two)  {Serial.println("Switch 2 ON-OFF");  Send(0,0,2,0);}
      // if (results.value == ir_three){Serial.println("Switch 3 ON-OFF");  Send(0,0,3,0);}
      // if (results.value == ir_four) {Serial.println("Switch 4 ON-OFF");  Send(0,0,4,0);}
      // if (results.value == ir_five) {Serial.println("Switch All ON-OFF");Send(0,0,5,0);}
      // if (results.value == ir_six)  {Serial.println("Restart Board");    Send(0,0,6,0);}
      // myData.z = 0;
    }
  }
  
 // ควบคุมทั้งหมด      
  if (results.value == ir_star) {N = TotalASpeech+1; // บอกเวลาเป็นเสียงพูด เวลาปัจจุบัน
    if (!start_time_relay.startsWith("PlayFolder 10 up")) {
      if(start_time_relay.startsWith("*****")){start_time_relay = "*";}   
      if (Wifi_Connect == false) {
      // Send(0,0,7,0);
      }else{
        if (start_time_relay.startsWith("*")) {  
          NSongMode = 0; LPlayAuto = false; talk_time_current();//Send(hour,minute,Nmdaymonyear,100);
        }
      } 
      // myData.z = 0;
      // Play Auto Loop Folder ตัวเลขที่อยู่หน้า *
      if (start_time_relay.substring(0,2).toInt() > 0 and start_time_relay.length() > 1) {LPlayAuto = true;NumberFile = 1;NPlayAuto = 0;
        NAutoFolder = start_time_relay.substring(0,2).toInt(); audio.connecttoSD( AFolderFile[NAutoFolder][NumberFile].c_str());
        start_time_relay = "";FolderPlay = NAutoFolder;FilePlay = NumberFile;
      }
    }  
  }
  if (results.value == ir_bottom){Serial.print("decrease "); if(NVolume >  1) {NVolume--;audio.setVolume(NVolume);Serial.print("Volume = "+String(NVolume));}}
  if (results.value == ir_top)   {Serial.print("increase "); if(NVolume < 30) {NVolume++;volume_old = NVolume;audio.setVolume(NVolume);Serial.println("Volume = "+String(NVolume));}}
  if (results.value == ir_bottom){Serial.print("decrease "); if(NVolume >  1) {NVolume--;volume_old = NVolume;audio.setVolume(NVolume);Serial.println("Volume = "+String(NVolume));}}
  if (results.value == ir_right) {FilePlay++;audio.connecttoSD( AFolderFile[FolderPlay][FilePlay].c_str() );Serial.println("Folder "+String(FolderPlay)+" File "+String(FilePlay));}
  if (results.value == ir_left)  {FilePlay--;audio.connecttoSD( AFolderFile[FolderPlay][FilePlay].c_str() );Serial.println("Folder "+String(FolderPlay)+" File "+String(FilePlay));}

  if (results.value == ir_sharp) {N = TotalASpeech+1;
    if (start_time_relay == "") {
      LcontrolBoard = !LcontrolBoard;
      if (LcontrolBoard == false){
        Serial.println("Play MP3");audio.stopSong();audio.connecttospeech("Play MP3", "th"); //audio.connecttoSD("/99 Control Sound/013 Control_Board_Me.mp3");
      }else{
        if (Control_Board == "ควบคุม Plug ไฟ") {
          Control_Board = "ควบคุม Board ของที่อื่น";          
          Serial.println("Control Board Other") ;audio.connecttoSD("/99 Control Sound/014 Control_Board_Other.mp3");
        }else{
          Control_Board = "ควบคุม Plug ไฟ";
          Serial.println("Control Plug") ;audio.stopSong();audio.connecttospeech("ควบคุม Plug ไฟ", "th");
        }
      }
    }else{
      start_time_relay = "";Serial.println(" start_time_relay = EMPTY ");audio.connecttoSD("/99 Control Sound/006 Empty.mp3");
    }
  }
} 

void Add_Zero(int FileNumber) {Str_FileNumber = String(FileNumber);
  if (FileNumber < 10){Str_FileNumber = "00"+String(FileNumber);}else{if (FileNumber < 100){Str_FileNumber = "0"+String(FileNumber);}}  // เติม 00 หน้าตัวเลขหลักหน่วย กับ 0 หลักสิบ
}

void ir_remote() {
  if (irrecv.decode(&results)) {  
    if (millis() - last_Remote > 50) {
      if ( (results.value != 0xFFFFFFFF) && (results.value != 0x00) ) { 
        // ถ้ากด 1 , 3 , 7 , 9 จะเปลี่ยนเป็นรีโมท 21 ปุ่ม
        if (results.value == R_one || results.value == R_three || results.value == R_seven || results.value == R_nine) {NChange_Remote = 1;}
        if (NChange_Remote == 1) {
          if (results.value == R_Mute){if(NVolume == 0){NVolume = volume_old;}else{volume_old = NVolume;NVolume = 0;} audio.setVolume(NVolume);Serial.println("Volume = "+String(NVolume));goto EXT;}
          if (results.value == R_PlayPause) {audio.pauseResume();goto EXT;}
          if (results.value == R_U_SD) {if (Wifi_Connect == true) {audio.connecttospeech("เรียกอีกแล้ว จะนอนซะหน่อย.", "th");}goto EXT;}
          if (results.value == R_RPT) {Serial.println("Sleep long until the remote wakes up");N = TotalASpeech+1;LFirst_Song = true;esp_deep_sleep_start();delay(1000);}  // Li2sInit = !Li2sInit; 
          // Change_Value_Remote();
        } 
        if (Total_last_Sleep == 2000) {Total_last_Sleep = 30000;NSleep = 0;}  //คืนค่าเวลาหลับ เมื่อกดปุ่มรีโมทอันใดอันหนึ่ง จะตั้งค่า Total_last_Sleep = 30000
        ControlBoard();
      }
      EXT:
      irrecv.resume(); // Receive the next value  
      last_Remote = millis();
    }     
  }  
}

//....................... SETUP ...........................//
void setup() {
  Serial.begin(115200); Serial.println("initializing...");
  pinMode(Relay1, OUTPUT);pinMode(Relay2, OUTPUT);pinMode(Relay3, OUTPUT);pinMode(Relay4, OUTPUT);
  //... Load data from Spiffs file /mydir/config.txt (Ram on board)
  // Start_Config();
  List_Config();  
  //... Set Audio
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  SPI.setFrequency(1000000);

  Check_SDcard(10); // เช็ค SD Card ซ้ำ 10 ครั้ง
  
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(NVolume); delay(1000);

  //........... Start Wifi .............//
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  check_ssid();
  connectInternet(20);  
  if (Wifi_Connect == true) {
    configTime(3600 * timezone, daysavetime * 3600, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");
    delay(1000);GetTimeInternet();
  }
  irrecv.enableIRIn(); //..... Start the receiver ...............//
}


void loop() {
  ir_remote();    // ใช้ Remote Control 

  if (Wifi_Connect == true and NYear == 1970) {
    configTime(3600 * timezone, daysavetime * 3600, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");
    GetTimeInternet();
  }

  // sendDHT();  // Send values Tempurature and Humidity

  if (millis() - last_timer > 4000) {last_timer = millis();
    Check_SDcard(1); // เช็ค SD Card 
    if (WiFi.status() == WL_CONNECTED) {Wifi_Connect = true ;} else {LFirst_Song=true;Wifi_Connect=false ;TotalASpeech=0;LStartSong = true;}
  }
  if (LFirst_Song == false and Wifi_Connect == true) {LFirst_Song = true; Leof_mp3 = false;Lspeech = false;
    // audio.connecttoSD("/07/015 -robot-repair-1407.mp3");
    audio.connecttospeech("สวัสดีตอนเช้า, วันนี้ฉันมีความสุขมาก", "th");
  }
  if ((Leof_mp3 == true or Lspeech == true) and N <= TotalASpeech and Wifi_Connect == true)  {Leof_mp3 = false;Lspeech = false;  Serial.println(TotalASpeech);
    Serial.print(N);Serial.print(" TotalASpeech = ");Serial.print(TotalASpeech);Serial.print(" , TotalASong = ");Serial.println(TotalASong);
    audio.connecttospeech(ASpeech[N].c_str(), "th"); if (N <= TotalASpeech){N++;} // else{if (LSDcard == false){N=1;}}
  }

  if (LOpenURL == false ) {LOpenURL = true;Lspeech = true;audio.stopSong();PlayAuto();}

  if ((Leof_mp3 == true or Lspeech == true) and N > TotalASpeech and LPlayAuto == true and LSDcard == true) {Leof_mp3 = false;Lspeech = false;
    PlayAuto();LStartSong = true;
  }
  if (millis() - last_Stopsong > 10000) {last_Stopsong = millis();if (LFirst_Song == true and LPlayAuto == true and LStartSong == true and LSDcard==true){audio.stopSong();PlayAuto();}}// เล่นเพลงต่อไป

  audio.loop(); //Executa o loop interno da biblioteca audio
}

