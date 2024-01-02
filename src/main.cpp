#include <Arduino.h>
#include <Config.h>
//********************** Control Blynk **************************//
#include <BlynkConfig.h>
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
#define BLYNK_TEMPLATE_NAME         "Device"
#ifdef ESP32
  #include "WiFi.h"    
  #include <WiFiClient.h>
  #include <BlynkSimpleEsp32.h>
#else
  #include <ESP8266WiFi.h>
  #include <BlynkSimpleEsp8266.h>
#endif

BlynkTimer timer1;
int currentRow = 0;
WidgetTerminal terminal(V6);

const char* serverBlynk = "elec.cmtc.ac.th";  //"blynk.honey.co.th"; //
const char* auth = "aEYM11-kCYSFSlwHAqLQ4dchVMNTRqWe"; //"LZL6-eIu2L8i2R2FdrVHsZZwIfuKC5RL";  //"gXvK65su6DEXBgTIlSy_FZm5aZUQM3q1"; "QFRC8tiVkeug3cz2EAEXWqTnwbEWPSp2";  //
int portBlynk = 8080;
int Blynk_Connect_Count = 0;
int Total_Blynk_Connect = 6;
String CBlynk_Cut,CBlynkReceive = "";
bool bool_startsWith = false;
//*************************************************************//
bool LMeditation = false;
String R_Text = "";
int NMoonPhase,NZero_Extra = 0;
int Relay1 = 4;int Relay2 = 13;int Relay3 = 0;int Relay4 = 4;
int N = 1;
int NFolder_Meditation,NFile_Meditation = 0;
int TotalASpeech = 30;
String ASpeech[] = {"โชคดีที่รอดมาอีก 1 วัน, ขอสรรพสัตว์ทั้งหลายจงมีความสุข","อันตัวเรานั้น ตายแน่ ตายแน่","Good morning"};
// int TotalASpeech = (sizeof(ASpeech) / sizeof(ASpeech[0])) - 1;
String MonthNameEng[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
String wdayNameEng[] = {"Blank","Mon","Tue","Wed","Thr","Fri","Sat","Sun"};
String mdayName[] = {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31"};
String monthName[] = {"",".มกราคม", ".กุมภาพันธ์", ".มีนาคม", ".เมษายน", ".พฤษภาคม", ".มิถุนายน",".กรกฎาคม", ".สิงหาคม", ".กันยายน", ".ตุลาคม", ".พฤศจิกายน", ".ธันวาคม"};
String wdayName[] = {"","วันจันทร์","วันอังคาร","วันพุธ","วันพฤหัส","วันศุกร์","วันเสาร์","วันอาทิตย์"};
String yearName[] = {"2565","2566","2567","2568","2569","2570","2571","2572","2573","2574"};

struct tm tmstruct ;

int every_minute = 5;
int total_every_minute,old_every_minute,every_hour = 0;
int minute_past,New_hour,New_minute,NDoW,Nmdaymonyear = 0;
long timezone = 7;  // 2;
byte daysavetime = 0; // 1;
String C_Moon,CMoonPhase,CMoonPhaseThai,start_time_relay,CString = "";
String CDay,CMon,CYear,CWday,CDateTime,CWdayThai,CDateTime_Thai = "";
int NAlarmClock,NMonth,NDay = 0;
int NChange_Remote,NSleep,Total_last_Sleep = 0;
int NYear = 1970;
bool LscanNetworks,Lwait_Slogan2,Lwait_Slogan1,Lwait_Sawasdee,Lwait_MonkDay,Lwait_Speech,LSend_Serial,Ltalk_Firsttime,LTime_Between,LFirstOnly,LFirstShow,LBetween,LTime_SammaArahang = false;
int NEvery_Min,NEvery_Min_Future = 0;
bool GetLocalTime(struct tm * info, uint32_t ms);
uint8_t hour    = 6;
uint8_t minute  = 59;
uint8_t sec     = 45;

File file;
const char filename[] = "/recording.wav";
const int headerSize = 44;

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

bool f_time     = false;
int8_t timefile = -1;
char chbuf[100];

//................... ตัวแปร เก็บใน SPIFFS (in Ram of Board) ...........................//
String CSound,CPlay_Test = "";
bool Wifi_Connect,Lconfirm,LScheduled,LReplace_Config,LConnect_internet_Auto,LStartSong,LTime_Schedu,LPlayAuto,LTalk_Everytime = false;
int NVolume = 6;
int volume_old = NVolume;

String ATime[40][2];String Ascheduled[30][3];
String AI_Word[2][10] = {{"","OK Wait a minute","OK Follow that","OK No problem"},{"","โอเค จัดไป","โอเค ตามนั้น","โอเค No problem"}};

const char * Replace_Config;
int NSammaArahang = 13;
//............. Driver and Varible Control Remote ....................//
#include <IRremote.h>
int RECV_PIN = 32;
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long last_Wifi,last_Remote,last,last_Sleep = millis();

bool Leof_speech = false; // จำเป็นต้องเป็น false เพราะไม่ให้เข้า บอกกิจวัตรประจำวัน แต่ให้เข้า Play_Speech(); ก่อน
bool Leof_mp3 = true;
bool LFirst_Song,LSDcard = false;
bool LNumber_Sound = true;
//................ Varible Control SDcard ....................//
//... กดตัวเลขแล้วตามด้วย * เล่นเพลงอัตโนมัติโฟลเดอร์นั้น เช่น 5* 
//... กด # ควบคุมบอร์ดอื่น ควมคุมบอร์ดตัวเอง หรือ ทำให้ตัวเลขที่กดผ่านมาว่างเปล่า 
int NPlayEvery,NPlayAuto,FolderPlay,FilePlay,NSongMode = 0;
String timerelay,FolderNumber,FolderFileNumber,Str_FileNumber,Cevery_minute = ""; 
int colon_pos = 0;
bool LcontrolBoard = false;
String syncword,Control_Board = "";

//............................................................//
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
    Check();
  }
}
// กด 0 นำหน้า เข้า void Zero_Extra()
void Zero_Extra() {
  addnumber();  //ใส่ตัวเลข
  //...........................................กด OK ลูกศร บน ล่าง ซ้าย ขวา ...........................................//
  if (results.value == ir_ok) {start_time_relay.replace(" ","");N = TotalASpeech+1;last_Sleep = millis(); // ลบช่องว่างใน start_time_relay
    if (NZero_Extra == 7){NSammaArahang = start_time_relay.toInt();
      audio.connecttospeech(("สัมมา อะระหังทุก "+String(NSammaArahang)+" นาที").c_str(), "th");      
      Serial.print("NSammaArahang = ");Serial.println(NSammaArahang);
    }
    if (NZero_Extra == 8){every_minute = start_time_relay.toInt();
      audio.connecttospeech(("บอกเวลาทุก "+String(every_minute)+" นาที").c_str(), "th");      
      Serial.print("every_minute = ");Serial.println(every_minute);
    }
    start_time_relay = "";NZero_Extra = 0;
  }
  if (results.value == ir_sharp) {
    start_time_relay = "";Serial.println(" start_time_relay = EMPTY ");audio.connecttoSD("/99 Control Sound/006 Empty.mp3");
  }
}

void ControlBoard() {  
  if (LcontrolBoard == false) {; //Serial.println(results.value); // ควบคุม Board ของตัวเอง
    addnumber();  //ใส่ตัวเลข
    //...........................................กด OK ลูกศร บน ล่าง ซ้าย ขวา ...........................................//
    if (results.value == ir_ok or bool_startsWith == true) { 
      if (bool_startsWith == true) {
        bool_startsWith = false; start_time_relay = CBlynk_Cut;CBlynk_Cut = "";
        if (start_time_relay.startsWith("0")) {
          Check(); return;
        }
      }
      start_time_relay.replace(" ","");N = TotalASpeech+1;last_Sleep = millis(); // ลบช่องว่างใน start_time_relay
      if (start_time_relay.startsWith("99")) { ; // AutoPlay Folder
        String CloopFolder = start_time_relay.substring(2,4);int NloopFolder = CloopFolder.toInt();
      }  
      if (start_time_relay.startsWith("*")) {
        NSongMode = 0;Serial.println(CWday+" "+CDay+"/"+CMon+"/"+CYear);//mp3.playMp3FolderTrack((CMon+CDay).toInt());if(CDay.toInt() < 10){waitMilliseconds(6000);}else{waitMilliseconds(7000);} myData.z = 0;
        if (Wifi_Connect == true) {audio.stopSong();
          Lwait_MonkDay = false;Leof_speech = true;
          MonkDay();  // เช็ค พรุ่งนี้วันพระ วันนี้วันพระ
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
        NFile = 0;  audio.pauseResume();    // Pause , Resume
        Serial.print("PauseResume");Serial.print(" , LPlayAuto = ");Serial.println(LPlayAuto);N = TotalASpeech+1;return;  
      }

      if (start_time_relay.length() > 1) {
        // if(Leof_speech == true) {Leof_speech = false;}else{audio.stopSong();}
        FolderPlay = start_time_relay.substring(0,1).toInt(); FilePlay = start_time_relay.substring(1,2).toInt();
        Serial.print("FolderPlay = ");Serial.println(FolderPlay);
        Serial.println(AFolderFile[FolderPlay][FilePlay].c_str());
        // audio.connecttoSD("/08 Music Popular/007 Unstoppable.mp3");
        Leof_mp3 = false; audio.connecttoSD( AFolderFile[FolderPlay][FilePlay].c_str() );
      }else { 
        // if(Leof_speech == true) {Leof_speech = false;}else{
        //   // audio.stopSong();
        // }  
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
      if (start_time_relay.substring(0,2).toInt() > 0 and start_time_relay.length() > 1 and Leof_mp3 == true) {LPlayAuto = true;NumberFile = 1;NPlayAuto = 0;
        NAutoFolder = start_time_relay.substring(0,2).toInt();Leof_mp3 = false; audio.connecttoSD( AFolderFile[NAutoFolder][NumberFile].c_str());
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
        if (NZero_Extra == 0){  
          ControlBoard();
        }else{
          Zero_Extra(); // กด 0 นำหน้าเข้าสู่โหมดพิเศษ
        }
      }
      EXT:
      irrecv.resume(); // Receive the next value  
      last_Remote = millis();
    }     
  }  
}
void Check_Wifi(int NConnect_Time) {
  // Serial.print(WiFi.status());Serial.println(" localIP = "+WiFi.localIP().toString());
  if (WiFi.status() != WL_CONNECTED or Wifi_Connect == false) {check_ssid(); }

  if (WiFi.localIP().toString() == "0.0.0.0" or WiFi.status() != WL_CONNECTED or Wifi_Connect == false) {
    connectInternet(); // ทำการเชื่อมต่อเน็ตจำนวน NConnect_Time ครั้ง 
    if (WiFi.status() == WL_CONNECTED) {
      Wifi_Connect = true ; Serial.print("Wifi Connected Ready IP address: ");Serial.println(WiFi.localIP()); Wifi_Connect = true ;
    }else{
      Serial.println("Wifi not connected !!!");
    }
  }
}

// This function sends Arduino's up time every second to Virtual Pin (5).
void myTimerEvent() {
  // Blynk.virtualWrite(V6, "Row: " + String(currentRow + 1));
  // for (int j = 0; j < COLS; j++) {
  //   String CNameMusic = AFolderFile[FolderPlay][j];
  //   int AT_Word = CNameMusic.lastIndexOf("/");
  //   CNameMusic = CNameMusic.substring(AT_Word+1,CNameMusic.length());

  //   Blynk.virtualWrite(V7, "Col " + String(j + 1) + ": " +String(tableData[currentRow][j]) );
  //   Blynk.virtualWrite(V7, CNameMusic);
  // }
  // Blynk.virtualWrite(V6, "-----------------------");
  // currentRow = (currentRow + 1) % ROWS;  
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  // String CTest_Blynk = "คลองสาม คลองหลวง ปทุมธานี";
  // Blynk.virtualWrite(V1, CTest_Blynk);Blynk.syncVirtual(V1);
  // Blynk.virtualWrite(V2, 1);Blynk.syncVirtual(V2);
  // Blynk.virtualWrite(V2, millis() / 1000);         // ใช้ไม่ได้
  // String Blynk_Leof = "Leof_speech = "+String(Leof_speech)+" Leof_mp3 = "+String(Leof_mp3);
  // Blynk.virtualWrite(V4, Blynk_Leof);//Blynk.syncVirtual(V4);
}
BLYNK_WRITE(V0) {
  if (param.asInt() != 0) {Lwait_Slogan1 = true;Lwait_Slogan2 = true;Leof_speech = true;N = TotalASpeech+1;}
  switch (param.asInt())
  {
    case 1: // Item 1
      FolderPlay = 1; FilePlay = 1;
      Serial.println(AFolderFile[FolderPlay][FilePlay]);
      audio.connecttoSD(AFolderFile[FolderPlay][FilePlay].c_str());
      break;
    case 2: // Item 2
      FolderPlay = 1; FilePlay = 2;
      Serial.println(AFolderFile[FolderPlay][FilePlay]);
      audio.connecttoSD( AFolderFile[FolderPlay][FilePlay].c_str());
      break;
    case 3: // Item 3
      FolderPlay = 1; FilePlay = 8;
      Serial.println(AFolderFile[FolderPlay][FilePlay]);
      audio.connecttoSD( AFolderFile[FolderPlay][FilePlay].c_str());
      break;
    default:
      Serial.println("Unknown item selected");
  }
}
bool String_startsWith(String Str1,String Str2,String Str3) { 
  // CBlynkReceive จะไม่โดนตัดคำออก , CBlynk_Cut จะโดนตัดคำออก เช่น config replace delete default
  if (CBlynk_Cut.startsWith(Str1) or CBlynk_Cut.startsWith(Str2) or CBlynk_Cut.startsWith(Str3)) {
    bool_startsWith = true;CBlynk_Cut.replace(Str1,"");CBlynk_Cut.replace(Str2,"");CBlynk_Cut.replace(Str3,"");CBlynk_Cut.replace(" ","");return true;
  }else{
    bool_startsWith = false;return false;
  }
}
bool String_indexOf(String Str1,String Str2,String Str3) { 
  if (CBlynkReceive.indexOf(Str1) >= 0 or CBlynkReceive.indexOf(Str2) >= 0 or CBlynkReceive.indexOf(Str3) >= 0) {CBlynk_Cut.replace(Str1,"");CBlynk_Cut.replace(Str2,"");CBlynk_Cut.replace(Str3,"");CBlynk_Cut.replace(" ","");return true;}else{return false;}
}

BLYNK_WRITE(V1) {
  if (param.asInt() != 0) {Lwait_Slogan1 = true;Lwait_Slogan2 = true;Leof_speech = true;N = TotalASpeech+1;}
  CBlynkReceive = param.asStr();
  if (!CBlynkReceive.isEmpty()) {CBlynk_Cut = CBlynkReceive;
    // .................. เข้าโหมด Config ...................//
    if (String_startsWith("config","setup","ตั้งค่า")) {
      // int bytesSent = Serial.write(CBlynkReceive.c_str());
      // ....เข้าโหมดย่อยของ Config คือ "replace","delete","default"....//
      if (String_startsWith("replace","delete","default")) {
        audio.connecttospeech(CBlynk_Cut.c_str(), "th");
        if (CBlynkReceive.indexOf("replace") >= 0) {Check_Replace_SPIFFS(CBlynk_Cut.c_str());} 
        if (CBlynkReceive.indexOf("delete") >= 0) {Check_Delete_SPIFFS(CBlynk_Cut.c_str());}
        if (CBlynkReceive.indexOf("default") >= 0) {Start_Config();}
      }
    }else{ 
      // .............. เข้าโหมด "remote","เพลง","play............//
      if (String_startsWith("remote","เพลง","play")) {  
        ControlBoard();
      }else{
        // .............. เข้าโหมด แปลงข้อความ เป็นคำพูด ............//
        Serial.print(" ความยาวตัวอักษร = ");Serial.println(CBlynkReceive.length());
        if (CBlynkReceive.length() >= 137) {CBlynkReceive = CBlynkReceive.substring(0,136);}
        audio.connecttospeech(CBlynkReceive.c_str(), "th");
      }
    }
  }
}
BLYNK_WRITE(V2) {int Blynk_Volume = param.asInt();
  NVolume = Blynk_Volume; audio.setVolume(NVolume);Serial.print("ระดับเสียง = ");Serial.println(NVolume);
}
BLYNK_WRITE(V3) {
  String action = param.asStr();
  if (action == "play") {audio.pauseResume();}
  if (action == "stop") {audio.pauseResume();}
  if (action == "next") {FilePlay++;if(AFolderFile[FolderPlay][FilePlay].isEmpty()) {FilePlay--;Serial.println("ไฟล์สุดท้าย ของโฟลเดอร์");}else{audio.connecttoSD( AFolderFile[FolderPlay][FilePlay].c_str() );Serial.println("Folder "+String(FolderPlay)+" File "+String(FilePlay));}}
  if (action == "prev") {if(FilePlay>1){FilePlay--;audio.connecttoSD( AFolderFile[FolderPlay][FilePlay].c_str() );Serial.println("Folder "+String(FolderPlay)+" File "+String(FilePlay));}else{Serial.println("ไฟล์แรก ของโฟลเดอร์");}}
  Blynk.setProperty(V3, "label", action); Serial.print(action);Serial.println();
  
  String CNameMusic = AFolderFile[FolderPlay][FilePlay];
  int AT_Word = CNameMusic.lastIndexOf("/");
  CNameMusic = CNameMusic.substring(AT_Word+1,CNameMusic.length());
  Blynk.virtualWrite(V5, CNameMusic);Blynk.syncVirtual(V5);      
}
BLYNK_WRITE(V4) {}

void terminal_ReadWord() {// ถ้ามีการลบข้อมูล จะยังอ่านค่าที่ลบไปแล้ว ยกเว้น Reset Board
  int NCount = 0;
  terminal.println("ค่า Config ระบบ");
  for (int i = 1; i <= 40; i++){ 
    if (ATime[i][1].isEmpty()) {break;}
    if (ATime[i][1].indexOf(":") < 0) { NCount++;
      if (NCount < 10){terminal.print(" "+String(NCount));}else{terminal.print(NCount);}
      terminal.println(" "+ATime[i][1]+" = "+ATime[i][2]);
    }
  }
  terminal.println();
  terminal.println("ตารางเวลา กิจวัตรประจำวัน");
  for (int i = 1; i <= 40; i++){ 
    if (Ascheduled[i][3].isEmpty()) {break;}
    terminal.println(Ascheduled[i][1]+":"+Ascheduled[i][2]+" "+Ascheduled[i][3]);
  }
  terminal.flush();
}

BLYNK_WRITE(V6) { 
  //   // Send it back
  // terminal.print("You said:");
  // terminal.write(param.getBuffer(), param.getLength());
  // audio.connecttospeech(Str_param.c_str(), "th"); 
  // terminal.println();terminal.flush(); // Ensure everything is sent

  CBlynkReceive = param.asStr();
  
  if (param.asInt() != 0 or CBlynkReceive.length() > 0) { // ยกเลิกทักทายตอนเปิดเครื่อง
    Lwait_Slogan1 = true;Lwait_Slogan2 = true;Leof_speech = true;N = TotalASpeech+1;
  }
  if (!CBlynkReceive.isEmpty()) {CBlynk_Cut = CBlynkReceive;
    if (String_startsWith("@","/","*"))  { //CBlynkReceive = CBlynkReceive.substring(1,CBlynkReceive.length());CBlynk_Cut = CBlynkReceive;
      if (String_indexOf("read","อ่าน","None!")) {audio.connecttospeech(CBlynkReceive.c_str(), "th");terminal_ReadWord();terminal.flush();return;}

      // .................. เข้าโหมด Config ...................//
      if (String_startsWith("config","setup","ตั้งค่า")) {
        // ....เข้าโหมดย่อยของ Config คือ "replace","delete","default"....//
          audio.connecttospeech(CBlynk_Cut.c_str(), "th");
          if (String_indexOf("default","เริ่มต้น","start")) {Start_Config();return;}
          if (String_indexOf("delete","ลบ","None!")) {Check_Delete_SPIFFS(CBlynk_Cut.c_str());return;}        
          if (String_indexOf("replace","แทนที่","None!")) {Check_Replace_SPIFFS(CBlynk_Cut.c_str());return;} 
      }else{ 
        // .............. เข้าโหมด "remote","เพลง","play............//
        if (String_startsWith("remote","เพลง","play")) {  
          ControlBoard();
        }
      }
    }else{
          // .............. เข้าโหมด แปลงข้อความ เป็นคำพูด ............//
          Serial.print(" ความยาวตัวอักษร = ");Serial.println(CBlynkReceive.length());
          if (CBlynkReceive.length() >= 137) {CBlynkReceive = CBlynkReceive.substring(0,136);}
          audio.connecttospeech(CBlynkReceive.c_str(), "th");
          // terminal.write(param.getBuffer(), param.getLength());
    }
    // Blynk.virtualWrite(V6, CBlynkReceive);Blynk.syncVirtual(V6);
    terminal.flush();
  }
}

BLYNK_CONNECTED() {
  Serial.print(" Server ");Serial.print(serverBlynk);Serial.print(" Auth ");Serial.println(auth);
  // Automaticly runs at Mega bootup
  Blynk.virtualWrite(V6, "\n"
                    "   ___  __          __\n"
                    "  / _ )/ /_ _____  / /__\n"
                    " / _  / / // / _ \\/  '_/\n"
                    "/____/_/\\_, /_//_/_/\\_\\\n"
                    "       /___/ v." BLYNK_VERSION " on\n            " BLYNK_INFO_DEVICE "\n");
  size_t ram = BlynkFreeRam();
  Blynk.virtualWrite(V6, "\n Blynk Free RAM: ", ram);
  Blynk.virtualWrite(V6, " , ESP FreeHeap RAM: " + String(ESP.getFreeHeap()) + "\n");
  if (NYear > 1970) {
   Blynk.virtualWrite(V6, "\n ", CDateTime, "\n");
  }
  terminal.print("Wifi Connected Ready IP address: ");terminal.println(WiFi.localIP());
  Blynk.virtualWrite(V2, NVolume);Blynk.syncVirtual(V2);

  BlynkParamAllocated items(256); // list length, in bytes
  for (int j = 0; j <= 10; j++) {
    // String CNameMusic = AFolderFile[FolderPlay][j];
    String CNameMusic = AFolderFile[2][j+1];
    int AT_Word = CNameMusic.lastIndexOf("/");
    CNameMusic = CNameMusic.substring(AT_Word+1,CNameMusic.length());
    if (CNameMusic.isEmpty()) {break;}
    items.add(CNameMusic);
  }
  Blynk.setProperty(V7, "labels", items);
  Blynk.virtualWrite(V7, 1);Blynk.syncVirtual(V7);
  // Blynk.syncAll();
}

//....................... SETUP ...........................//
void setup() {
  Serial.begin(115200); Serial.println("initializing...");
  irrecv.enableIRIn(); //..... Start the receiver ...............//
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(Relay1, OUTPUT);pinMode(Relay2, OUTPUT);pinMode(Relay3, OUTPUT);pinMode(Relay4, OUTPUT);
  //... Load data from Spiffs file /mydir/config.txt (Ram on board)
    // Start_Config();
    List_Config();  
  //... Set Audio
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  SPI.setFrequency(1000000);
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(NVolume); 
  
  // RTOS_Setup(); // xTaskCreate
  Check_SDcard(); // xTaskCreate

  //... Start Wifi and Connect Internet and get time from internet .............//
  WiFi.mode(WIFI_STA);WiFi.disconnect();delay(100);
  check_ssid();
  connectInternet();  
  if (Wifi_Connect == true) {
    Serial.print("Wifi Connected Ready IP address: ");Serial.println(WiFi.localIP()); Wifi_Connect = true ;
  }
// if (Wifi_Connect == true) {EXIT2:
  //   configTime(3600 * timezone, daysavetime * 3600, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");
  //   delay(1000);GetTimeInternet();
  //   if (CMoonPhaseThai == "") {goto EXIT2;} // Must be GetTimeInternet() pass
  // if (Wifi_Connect == true)
    // Blynk.config(auth,serverBlynk,portBlynk);delay(1000);
    // Blynk.disconnect();
    // Blynk.connect(); Serial.print(" Server ");Serial.print(serverBlynk);Serial.print(" Auth ");Serial.println(auth);
    // delay(1000);
  // }
  Read_Ascheduled();

  if (Blynk.connected() == true){
    terminal.clear();
    terminal.println(F("Blynk v" BLYNK_VERSION ": Device started"));
    terminal.println(F("-------------"));
    terminal.println(F("Type 'Marco' and get a reply, or type"));
    terminal.println(F("anything else and get it printed back."));
    terminal.flush();
    // timer1.setInterval(2000L, myTimerEvent);
  }  
}

void loop() {
  ir_remote();    // ใช้ Remote Control 

  if (WiFi.status() == WL_CONNECTED and NYear == 1970) {
    configTime(3600 * timezone, daysavetime * 3600, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");
    GetTimeInternet();
  }
  if (millis() - last_Wifi > 20000) {last_Wifi = millis();
    Check_SDcard(); // xTaskCreate // เช็ค SD Card 
    if (LConnect_internet_Auto == true) {Check_Wifi(5);}  //{configTime(3600 * timezone, daysavetime * 3600, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");Send_Time();} // ตั้งค่าเวลานาฬิกา และ Send_Time(); ส่งค่าเวลาไปบอร์ดอื่น
    // if (Wifi_Connect==true) {
    //   if (Blynk.connected()==false){
    //     if (N >= TotalASpeech) {
    //       // Blynk.connect(); Serial.print(" Server ");Serial.print(serverBlynk);Serial.print(" Auth ");Serial.println(auth);
    //     }else{ if (N == 1) {Blynk.config(auth, serverBlynk, portBlynk);}}
    //   }
    // } 
  }  

  if (Wifi_Connect == true and WiFi.status() == WL_CONNECTED) {
    //........................ Blynk  Run and Connect ..............................//
    if (Blynk.connected() == true){
      if (Blynk_Connect_Count <= Total_Blynk_Connect and Blynk_Connect_Count > 1) {Blynk_Connect_Count = 1;}
      Blynk.run();  //timer1.run(); 
    }else{
      if (Blynk_Connect_Count <= Total_Blynk_Connect) {  
        // if (Leof_speech == true and Leof_mp3 == true) {
          if (Blynk.connect() == false) {Blynk.config(auth, serverBlynk, portBlynk);}else{ 
            Blynk_Connect_Count++;  // Serial.print(" Server ");Serial.print(serverBlynk);Serial.print(" Auth ");Serial.println(auth);
          }
        // }
      }
    }
    //..................... ส่งค่าตัวแปรผ่าน Serial Monitor ............................//
    if (LSend_Serial == true and Leof_speech == true){Send_SerialMonitor();}
    //.......... บอกเวลาเป็นเสียงพูดทุกกี่ชั่วโมง หรือ นาที (0,1) = ทุก ๆ 1 นาที ..............//  
    talk_everytime(every_hour,every_minute);  
  }
  
  Time_Schedu(); // ตารางเวลาประจำวัน 4 เวลา 6:00 , 13:00 , 18:00 , 22:00

  if (millis() - last_timer > 2000) {last_timer = millis();
    if (Wifi_Connect == true){ GetTimeInternet();
      // แสดงผลใน Serial Monitor และ Blynk Terminal ทุก 2 วินาที
      String C_Print = " NMoon = "+String(NMoonPhase)+" Leof_speech = "+Leof_speech+" Leof_mp3 = "+Leof_mp3;
      Serial.println(C_Print); terminal.println(CWdayThai+" "+CDateTime+C_Print);
    }
    // sendDHT();    // ส่งค่าอุณหภูมิ ความชื้นSerial.println(" ");
  }
  
  if (millis() - last_timer > 4000) {last_timer = millis();
    if (WiFi.status() == WL_CONNECTED) {Wifi_Connect = true ;} else {LFirst_Song=true;Wifi_Connect=false ;TotalASpeech=0;LStartSong = true;}
  }
  if (Wifi_Connect == true) {
    if (LFirst_Song == false) { // audio.connecttoSD("/07/015 -robot-repair-1407.mp3");
      Play_Speech();  // ใช้เสียงจาก Google Speech audio.connecttospeech(ASpeech[N].c_str(), "th");
    }
  }else{Leof_speech = true;}
  if ((Leof_speech == true and Lwait_Slogan2 == true) and N <= TotalASpeech and Wifi_Connect == true)  {Serial.println(TotalASpeech);
    Serial.print(N);Serial.print(" TotalASpeech = ");Serial.print(TotalASpeech);Serial.print(" , TotalASong = ");Serial.println(TotalASong);
    Leof_speech = false;
    if (Ascheduled[N][3].isEmpty() == false) { 
      String Str_Speech = "เวลา "+Ascheduled[N][1]+":"+Ascheduled[N][2]+"  "+Ascheduled[N][3];
      // audio.connecttospeech(ASpeech[N].c_str(), "th"); 
      audio.connecttospeech(Str_Speech.c_str(), "th"); 
      if (N <= TotalASpeech){N++;} // else{if (LSDcard == false){N=1;}}
    }
  }
  // if (LOpenURL == false ) {LOpenURL = true;audio.stopSong();PlayAuto();}
  if ((Leof_mp3 == true) and N > TotalASpeech and LPlayAuto == true and LSDcard == true) {
    PlayAuto();LStartSong = true;
  }
  if (millis() - last_Stopsong > NPlayEvery) {last_Stopsong = millis();if (LFirst_Song == true and LPlayAuto == true and LStartSong == true and LSDcard==true){audio.stopSong();PlayAuto();}}// เล่นเพลงต่อไป

  audio.loop(); 
}
