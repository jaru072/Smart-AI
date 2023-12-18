#include <Arduino.h>
#include <Config.h>
#include <LITTLEFS.h>

/* You only need to format LITTLEFS the first time you run a
   test or else use the LITTLEFS plugin to create a partition
   https://github.com/lorol/arduino-esp32littlefs-plugin */
#define FORMAT_LITTLEFS_IF_FAILED true
#define SPIFFS LITTLEFS

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.print (file.name());
            time_t t= file.getLastWrite();
            struct tm * tmstruct = localtime(&t);
            Serial.printf(" LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n",(tmstruct->tm_year)+1900,( tmstruct->tm_mon)+1, tmstruct->tm_mday,tmstruct->tm_hour , tmstruct->tm_min, tmstruct->tm_sec);
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.print(file.size());
            time_t t= file.getLastWrite();
            struct tm * tmstruct = localtime(&t);
            Serial.printf(" LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n",(tmstruct->tm_year)+1900,( tmstruct->tm_mon)+1, tmstruct->tm_mday,tmstruct->tm_hour , tmstruct->tm_min, tmstruct->tm_sec);
        }
        file = root.openNextFile();
    }
}

void createDir(fs::FS &fs, const char * path){
    Serial.printf("Creating Dir: %s\n", path);
    if(fs.mkdir(path)){
        Serial.println("Dir created");
    } else {
        Serial.println("mkdir failed");
    }
}

void removeDir(fs::FS &fs, const char * path){
    Serial.printf("Removing Dir: %s\n", path);
    if(fs.rmdir(path)){
        Serial.println("Dir removed");
    } else {
        Serial.println("rmdir failed");
    }
}

void readFile(fs::FS &fs, const char * path){Serial.printf("Reading file: %s\n", path);
    File file = fs.open(path);
    if(!file){Serial.println("Failed to open file for reading");return;}
    Serial.print("Read from file: ");while(file.available()){ Serial.write(file.read());}Serial.println("");
    file.close();
}

void readWord(fs::FS &fs, const char * path){ String R_Text,Cwords,Cwords_value = ""; int AT_Time = 1; int AT_Equal,AT_Comma = 0;    
  if(!SPIFFS.begin()){Serial.println("Card Mount Failed");return;}
  File file = fs.open(path); if(!file){Serial.println("Failed to open file for reading"); return;}    
  while(file.available()){R_Text = R_Text+file.readString();}  
  int N = 1;
  for (int i = 1; i <= 40; i++){ // Serial.println(R_Text);
    if (R_Text.startsWith(",time")) { //... ใช้กับตารางเวลา เพราะต้องเก็บค่าลง Cwords เฉพาะเวลา ตัด ,time ออก
      Cwords = R_Text.substring(5,11);  int length_Word = 4; AT_Equal = 11;
    }else{                            //... ใช้กับ Config อื่น ๆ
      Cwords = R_Text.substring(1,R_Text.indexOf("=")); int length_Word = Cwords.length(); // ชื่อตัวแปรตัวแรกของประโยค        
      AT_Equal = R_Text.indexOf("=");
    }
    AT_Comma = R_Text.indexOf(",",2); // เช็คตำแหน่งของ = และ ,
    Cwords_value = R_Text.substring(AT_Equal+1,AT_Comma); // ค่าของตัวแปรที่ได้รับ
    ATime[i][1] = Cwords; ATime[i][1].replace(" ","");Cwords_value.replace(",","");ATime[i][2] = Cwords_value; // เอาทั้งชื่อและค่าของตัวแปรเก็บในตัวแปร Array ATime[i][1] , ATime[i][2]
    if (R_Text.startsWith(",time")){
      Ascheduled[N][1] = ATime[i][1].substring(0,2); Ascheduled[N][2] = ATime[i][1].substring(3,5);Ascheduled[N][3] = ATime[i][2];N = N+1;
//      Serial.print(Ascheduled[N][1]);Serial.print(":");Serial.print(Ascheduled[N][2]);Serial.print(" ");Serial.println(Ascheduled[N][3]);
    }  
    Serial.print("Read Word: ");      
    if (i < 10){Serial.print(" ");Serial.print(i);}else{Serial.print(i);}
    Serial.print(". ");Serial.print(ATime[i][1]);Serial.println(+" = "+ATime[i][2]);        
    // เก็บค่าลงในตัวแปร Config มาตราฐาน ยกเว้นตารางเวลา Scheduled เพราะเก็บในตัวแปร Array ATime[i][1] ,ATime[i][2] เรียบร้อยแล้ว
    if (ATime[i][1] == "Play_Test") {CPlay_Test = ATime[i][2];}
    if (ATime[i][1] == "Sound") {CSound = ATime[i][2];}
    if (ATime[i][1] == "Scheduled") {LScheduled = ATime[i][2];}
    if (ATime[i][1] == "Volume") {NVolume = ATime[i][2].toInt();} // เฉพาะตัวแปร int เท่านั้นที่ต้องแปลงค่า
    if (ATime[i][1] == "Connect_internet_Auto") {LConnect_internet_Auto = ATime[i][2];}
    if (ATime[i][1] == "StartSong") {LStartSong = ATime[i][2];} 
    if (ATime[i][1] == "Time_Schedu") {LTime_Schedu = ATime[i][2];} 
    if (ATime[i][1] == "PlayAuto") {LPlayAuto = ATime[i][2];}
    if (ATime[i][1] == "every_minute") {every_minute = ATime[i][2].toInt();}
    if (ATime[i][1].startsWith("SammaArahang")) {NSammaArahang = ATime[i][2].toInt();}
    if (ATime[i][1].startsWith("Talk_Everytime")) {LTalk_Everytime = ATime[i][2];}
    if (ATime[i][1].startsWith("Folder_Meditation")) {NFolder_Meditation = ATime[i][2].toInt();}
    if (ATime[i][1].startsWith("File_Meditation")) {NFile_Meditation = ATime[i][2].toInt();}

    // ................................................................................................//
    R_Text = R_Text.substring(AT_Comma,10000);  // ตัดประโยค ที่รับค่าแล้วออก  //Serial.println(R_Text);    
    if (R_Text.indexOf("=") == -1){break;}  // ถ้าตัดประโยคออกหมดแล้ว ให้ออกจาก for ดูจาก = มีค่าเท่ากับ -1
  }
  file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){Serial.printf("Writing file: %s\n", path);
    File file = fs.open(path, FILE_WRITE);
    if(!file){Serial.println("Failed to open file for writing");return;}
    if(file.print(message)){Serial.println("File written");} else {Serial.println("Write failed");}
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
    file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
    Serial.printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2)) {
        Serial.println("File renamed");
    } else {
        Serial.println("Rename failed");
    }
}

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\n", path);
    if(fs.remove(path)){
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
    }
}
//...................................................................................//
void DeleteFile(fs::FS &fs, const char * path,String Creplace){ 
  String R_Text = "";Creplace.replace(" ","");
  if (Creplace.indexOf(":") > 0) {Creplace = "time "+Creplace;}
  File file = fs.open(path); if(!file){Serial.println("Failed to open file for reading");return;}
  while(file.available()){R_Text = R_Text+file.readString();}
  int AT_Eqaual = Creplace.indexOf("=");
  Creplace = Creplace+",";  
  if (R_Text.indexOf(Creplace.substring(1,AT_Eqaual)) > 0) {  
    Serial.println("ลบ เข้าไปในไฟล์ Config.txt "+Creplace); 
    int Creplace_Len = Creplace.substring(0,Creplace.indexOf("=")).length();
    int AT_Found = R_Text.indexOf(Creplace.substring(0,Creplace_Len));
    String CFirst = R_Text.substring(0,AT_Found);
    R_Text = R_Text.substring(AT_Found+Creplace_Len,100000);
    R_Text = R_Text.substring(R_Text.indexOf(",")+1,100000);
    R_Text = CFirst+R_Text; 
    Replace_Config = R_Text.c_str();  
    writeFile(SPIFFS, "/mydir/config.txt", Replace_Config);
  }
  file.close();
}

void ReplaceFile(fs::FS &fs, const char * path,String Creplace){ 
  String R_Text = "";Creplace.replace(" ","");
  if (Creplace.indexOf(":") > 0) {Creplace = "time "+Creplace;}
  File file = fs.open(path); if(!file){Serial.println("Failed to open file for reading");return;}
  while(file.available()){R_Text = R_Text+file.readString();}
  int AT_Eqaual = Creplace.indexOf("=");
  Creplace = Creplace+",";  
  if (R_Text.indexOf(Creplace.substring(1,AT_Eqaual)) > 0) {  
    Serial.println("แทนที่ เข้าไปในไฟล์ Config.txt "+Creplace); 
    // if (Creplace.startsWith("SammaArahang")) {Creplace.replace("ทุก","ทุก=");Creplace.replace("นาที","");}
    int Creplace_Len = Creplace.substring(0,Creplace.indexOf("=")).length();
    int AT_Found = R_Text.indexOf(Creplace.substring(0,Creplace_Len));
    String CFirst = R_Text.substring(0,AT_Found);
    R_Text = R_Text.substring(AT_Found+Creplace_Len,100000);
    R_Text = R_Text.substring(R_Text.indexOf(",")+1,100000);
    R_Text = CFirst+Creplace+R_Text; 
    Replace_Config = R_Text.c_str();  
    writeFile(SPIFFS, "/mydir/config.txt", Replace_Config);
  }else{                                
    Serial.println("เพิ่ม เข้าไปในไฟล์ Config.txt "+Creplace);  
    // if (Creplace.startsWith("SammaArahang")) {Creplace.replace("ทุก","ทุก=");Creplace.replace("นาที","");}
    appendFile(SPIFFS, "/mydir/config.txt", Creplace.c_str());        
  }
  file.close();
}
void Check_Replace_SPIFFS(const char* Replace_Config) {
  if(!SPIFFS.begin()){Serial.println("Card Mount Failed");}
  ReplaceFile(SPIFFS, "/mydir/config.txt",Replace_Config);
  readFile(SPIFFS, "/mydir/config.txt");
  readWord(SPIFFS, "/mydir/config.txt");
}
void Check_Delete_SPIFFS(const char* Replace_Config) {
  if(!SPIFFS.begin()){Serial.println("Card Mount Failed");return;}
  DeleteFile(SPIFFS, "/mydir/config.txt",Replace_Config);
  readFile(SPIFFS, "/mydir/config.txt");
  readWord(SPIFFS, "/mydir/config.txt");
}
// เขียนค่า Config เริ่มต้นลง Rom ภายในบอร์ด
void Check_SPIFFS() {  
  if(!SPIFFS.begin()){Serial.println("Card Mount Failed");return;}
  createDir(SPIFFS, "/mydir");
  listDir(SPIFFS, "/", 0);//    readFile(SPIFFS, "/mydir/config.txt");//    removeDir(SPIFFS, "/mydir");//    createDir(SPIFFS, "/mydir");
  deleteFile(SPIFFS, "/mydir/config.txt");
  
  const char* Str_Config    = ",Volume=5,Sound=on,Play_Test=on,Scheduled=true";
  const char* Str_Scheduled1 = ",time 04:00=ตื่นนอน ออกกำลังกาย,time 04:50=สวดมนต์ ทำวัตรเช้า,time 05:22=เตรียมออกบิณฑบาต,time 07:30=ฉันภัตตาหารเช้า,time 09:00=นั่งสมาธิ,time 10:00=เขียนโปรแกรม,time 11:00=ฉันเพล,time 14:00=นั่งสมาธิ,time 15:00=ทำความสะอาดกุฏิ,time 17:00=เดินเล่น ออกกำลังกาย,time 18:00=สรงน้ำ,time 19:00=สวดมนต์ ทำวัตรเย็น,time 19:30=นั่งสมาธิ,time 21:30=จำวัตร,";
  
  writeFile(SPIFFS, "/mydir/config.txt", Str_Config);
  appendFile(SPIFFS, "/mydir/config.txt", Str_Scheduled1);

  readFile(SPIFFS, "/mydir/config.txt");
  readWord(SPIFFS, "/mydir/config.txt");
}

void List_Config() {  
  if(!SPIFFS.begin()){Serial.println("Card Mount Failed");return;}
  readFile(SPIFFS, "/mydir/config.txt");
  readWord(SPIFFS, "/mydir/config.txt");
}

void Start_Config(void) {  
  Check_SPIFFS();   // เขียนค่า Config เริ่มต้นลง Rom ภายในบอร์ด
  Check_Replace_SPIFFS("04:50 = ทำวัตรเช้า");    
  Check_Replace_SPIFFS("Time_Schedu=true");    
  Check_Replace_SPIFFS("StartSong = true");    
  Check_Replace_SPIFFS("Connect_internet_Auto=true");    
  Check_Replace_SPIFFS("Talk_Everytime = false");    
  Check_Replace_SPIFFS("every_minute = 5");
  Check_Replace_SPIFFS("SammaArahang = 29");
  Check_Replace_SPIFFS("Folder_Meditation = 2");
  Check_Replace_SPIFFS("File_Meditation = 1");
}

// Name_Config ชื่อตัวแปรที่จะ Save , Config_Value ค่าตัวแปรที่จะ Save
void Send_Value(String Name_Config,int Config_Value) {
  if(R_Text.indexOf(Name_Config) >= 0) {int AT_Word = R_Text.indexOf(Name_Config);
    int Nlenght_Word = Name_Config.length();
    String String_Value = R_Text.substring(AT_Word+Nlenght_Word,AT_Word+Nlenght_Word+2);String String_New_Value = String(Config_Value);
    String_Value = String(String_Value.toInt());  
    R_Text.replace(Name_Config+String_Value,Name_Config+String_New_Value);
  }
}
//.................. เขียนไฟล์ลง RAM of Board แล้วเรียกกลับไปใช้ setup ค่าตัวแปร .........................//
void Save_Config(fs::FS &fs, const char * path){ //String R_Text = ""; // Save ตัวแปร Config ทั้งหมดลงกลับไปใน Ram
  File file = fs.open(path); if(!file){Serial.println("Failed to open file for reading"); return;}    
  while(file.available()){R_Text = R_Text+file.readString();}    
  //................... Save ค่าตัวแปร int ไม่เกิน 2 หลัก ..................//
  Send_Value("Volume=" , NVolume);
  Send_Value("every_minute=" , every_minute);
  Send_Value("SammaArahang=" , NSammaArahang);
  Send_Value("Folder_Meditation=" , NFolder_Meditation);
  Send_Value("File_Meditation=" , NFile_Meditation);
  //........................ แก้ไขค่าตัวแปร String ......................//
  if(CPlay_Test == "on") {R_Text.replace("Play_Test=off","Play_Test=on");}else{R_Text.replace("Play_Test=on","Play_Test=off");}
  if(CSound == "on") {R_Text.replace("Sound=off","Sound=on");}else{R_Text.replace("Sound=on","Sound=off");}
  //........................ แก้ไขค่าตัวแปร Boolean .....................//
  if(LScheduled == true) {R_Text.replace("Scheduled=false","Scheduled=true");}else{R_Text.replace("Scheduled=true","Scheduled=false");}
  if(LConnect_internet_Auto == true) {R_Text.replace("Connect_internet_Auto=false","Connect_internet_Auto=true");}else{R_Text.replace("Connect_internet_Auto=true","Connect_internet_Auto=false");}
  if(LStartSong == true) {R_Text.replace("StartSong=false","StartSong=true");}else{R_Text.replace("StartSong=true","StartSong=false");}
  if(LTime_Schedu == true) {R_Text.replace("Time_Schedu=false","Time_Schedu=true");}else{R_Text.replace("Time_Schedu=true","Time_Schedu=false");}
  if(LPlayAuto == true) {R_Text.replace("PlayAuto=false","PlayAuto=true");}else{R_Text.replace("PlayAuto=true","PlayAuto=false");}
  if(LTalk_Everytime == true) {R_Text.replace("Talk_Everytime=false","Talk_Everytime=true");}else{R_Text.replace("Talk_Everytime=true","Talk_Everytime=false");}
  //................... writeFile file.close() readWord ..............//
  writeFile(SPIFFS, "/mydir/config.txt", R_Text.c_str());
  file.close();
  readWord(SPIFFS, "/mydir/config.txt");
}

void Read_Ascheduled() {
  for (int i = 1; i <= 31; i++){
    if (Ascheduled[i][1].isEmpty() == false) {  
      Serial.print(Ascheduled[i][1]+":");Serial.print(Ascheduled[i][2]+" ");Serial.println(Ascheduled[i][3]);
      TotalASpeech = i;
    }else{break;}
  }
}

void Check() {
    if (Wifi_Connect == true) {
      if (start_time_relay.startsWith("0") and start_time_relay.toInt() > 0){audio.stopSong();
        //.......................... กดเลข 0 นำหน้า ตามด้วยตัวเลข .....................................//
        if (start_time_relay.toInt() == 1){audio.connecttospeech("ควบคุม Plug ไฟ", "th");LcontrolBoard = true;Control_Board = "ควบคุม Plug ไฟ";}
        if (start_time_relay.toInt() == 2){audio.connecttospeech("อ่านข้อมูลจาก Config", "th");readFile(SPIFFS, "/mydir/config.txt");readWord(SPIFFS, "/mydir/config.txt");} 
        if (start_time_relay.toInt() == 3){audio.connecttospeech("เขียนข้อมูลลง Config", "th");Check_Replace_SPIFFS("21:30=จำวัตร");}        
        if (start_time_relay.toInt() == 4){audio.connecttospeech("ลบคำสั่งใน Config", "th");Check_Delete_SPIFFS("21:00=จำวัตร");}      
        if (start_time_relay.toInt() == 5){Leof_speech = false;audio.connecttospeech("ฉันชื่อ จารุณี มีอะไรให้รับใช้", "th");LSend_Serial = true;} // ส่งค่าตัวแปรผ่าน Serial Monitor//อ่านตารางเวลา กิจวัตรประจำวัน Read_Ascheduled();
        if (start_time_relay.toInt() == 6){audio.connecttospeech("ตั้งค่า Config เป็น Default ค่าเริ่มต้น", "th");Start_Config();}       
        // if (start_time_relay.toInt() == 7){audio.connecttospeech("ตั้งค่าสัมมา อะระหัง ทุกกี่นาที", "th");NZero_Extra = 7;}  
        if (start_time_relay.toInt() == 7){audio.connecttospeech("หลวงพ่อนำนั่ง ง่ายแต่ลึก ต่อไป", "th");PlayNext_Meditation();}  
        if (start_time_relay.toInt() == 8){audio.connecttospeech("ตั้งค่าบอกเวลา ทุกกี่นาที", "th");NZero_Extra = 8;}  
        if (start_time_relay.toInt() == 9){audio.connecttospeech("Save ตัวแปร Config ลงใน Ram ของ Board", "th");Save_Config(SPIFFS, "/mydir/config.txt");}  
        start_time_relay = "";      
        //.........................................................................................//
      }      
    }
}

void PlayNext_Meditation() {
  if (AFolderFile[NFolder_Meditation][NFile_Meditation].isEmpty()){
    NFolder_Meditation++;NFile_Meditation = 1;
    Serial.print("NFolder_Meditation = ");Serial.println(NFolder_Meditation);
    Serial.print("NFile_Meditation = ");Serial.println(NFile_Meditation);
  }
  if (NFolder_Meditation == 5){NFolder_Meditation = 2;NFile_Meditation = 1;}
  audio.connecttoSD( AFolderFile[NFolder_Meditation][NFile_Meditation].c_str());
  audio.setVolume(10);Serial.println("Volume = 10");LMeditation = true;
  NFile_Meditation++;Serial.print("NFile_Meditation = ");Serial.println(NFile_Meditation);
  Save_Config(SPIFFS, "/mydir/config.txt");
}