#include <Arduino.h>
#include <Config.h>

int hourmin(int hourmin_h,int hourmin_m) {
  int hourmin = (hourmin_h*60)+hourmin_m;
  return hourmin;
}
//............................................ บอกเวลาเป็นเสียงพูด เวลาปัจจุบัน ....................................//
void talk_time_current() {
  if (hour == 0 && minute == 0) {
    Serial.println("Not Talk Time if hour = 0 and minute = 0");
  }else {
    if (Wifi_Connect == true) {
      Serial.print(" "+CWday);Serial.printf(" %02d-%02d-%d %02d:%02d:%02d\n",tmstruct.tm_mday, tmstruct.tm_mon + 1,tmstruct.tm_year + 1900, tmstruct.tm_hour,tmstruct.tm_min, tmstruct.tm_sec);
      // ข้อมูลที่จะดาวน์โหลด "วันของสัปดาห์ วัน/เดือน/ปี" คือตัวแปร CString , ตัวแปร CStringEng ใช้แสดงผลที่ Serial Monitor หรือ จอ LCD เท่านั้น
      if(start_time_relay == "*") {int NTemperature = t;int NHumidity = h;
          // audio.connecttospeech(("อุณหภูมิ "+String(NTemperature)+" องศาเซลเซียส"+" ความชื้น "+String(NHumidity)).c_str(), "th");
          if (C_Moon == "") {audio.connecttospeech((CMoonPhaseThai).c_str(), "th");}else{audio.connecttospeech((C_Moon).c_str(), "th");}
      }else{        
        if(start_time_relay.startsWith("**"))  {Serial.print("CMoonPhaseThai = ");Serial.println(CMoonPhaseThai);
          String String_Time = (" เวลา "+CDateTime.substring(11,16));
          CString = (CWdayThai+" ที่ "+String(tmstruct.tm_mday)+monthName[tmstruct.tm_mon + 1]+CMoonPhaseThai+String_Time);
        } 
        if(start_time_relay.startsWith("***")) {CString = (CWdayThai+" ที่ "+String(tmstruct.tm_mday)+monthName[tmstruct.tm_mon + 1]+CMoonPhaseThai);} 
        if(start_time_relay.startsWith("****")){CString = (" วันที่ "+String(tmstruct.tm_mday)+monthName[tmstruct.tm_mon + 1]+" "+String(tmstruct.tm_year + 1900+543)+ " เวลา "+CDateTime.substring(11,16));}       
        Serial.println(CString);audio.stopSong();audio.connecttospeech(CString.c_str(), "th");          
      }
    }
  }
}

// //................................. บอกเวลาเป็นเสียงพูด ทุกกี่นาที ระหว่างเวลาตี 5 ถึง  4 ทุ่ม ....................................//
void talk_everytime(int every_hour,int every_minute){
  if (Leof_speech == true and Leof_mp3 == true) {
    if ((hour >= 5 and hour < 22) and old_every_minute != minute) {old_every_minute = minute;
      if (LTalk_Everytime == true){total_every_minute++ ;
        Serial.print("total_every_minute = ");Serial.println(total_every_minute); 
        if (every_hour == 0) {
          if (total_every_minute >= every_minute){Serial.print("talk_everytime Minute = ");Serial.println(every_minute);total_every_minute=0;
            CString = ("เวลา "+CDateTime.substring(11,16));audio.connecttospeech(CString.c_str(), "th");
          }
        } else {Serial.println("talk_everytime Hour");
            CString = ("เวลา "+CDateTime.substring(11,16));audio.connecttospeech(CString.c_str(), "th");
        }
      }  
    }
  }
}
void Sawasdee(int Bhour_Start,int Bmin_Start,int Bhour_Stop,int Bmin_Stop,String CSawasdee) {
  if (Leof_speech == true and Lwait_Sawasdee == false) { 
    int Nstart = hourmin(Bhour_Start, Bmin_Start);
    int Nstop = hourmin(Bhour_Stop, Bmin_Stop);
    int Nhourmin = hourmin(hour, minute);
    if (Nstart <= Nhourmin and Nstop >= Nhourmin ) {
      Serial.println(CSawasdee); 
      Leof_speech = false;audio.connecttospeech(CSawasdee.c_str(), "th");
      Lwait_Sawasdee = true;    
    }
  }
}

void SammaArahang_Between(int Bhour_Start,int Bmin_Start,int Bhour_Stop,int Bmin_Stop) {
  int Nstart = hourmin(Bhour_Start, Bmin_Start);
  int Nstop = hourmin(Bhour_Stop, Bmin_Stop);
  int Nhourmin = hourmin(hour, minute);
  if (Nstart <= Nhourmin and Nstop >= Nhourmin ) {
    LTime_SammaArahang = true;
  }else{
    LTime_SammaArahang = false;
  }
}

void Time_SammaArahang(int NEvery_Min,int NDouble_Min) {
  if (LMeditation == false) {  
    int Nhourmin = hourmin(hour, minute);
    if (13*60 <= Nhourmin) {SammaArahang_Between(13,0,19,0);}else{SammaArahang_Between(8,30,11,0);}
    // if(LTime_SammaArahang == false) {SammaArahang_Between(8,30,11,0 );Serial.println("สัมมาอะระหัง 08:30-11:00");}
    // if(LTime_SammaArahang == false) {SammaArahang_Between(13,0,13,59);Serial.println("สัมมาอะระหัง 13:00-13:59");}
    // if(LTime_SammaArahang == false) {SammaArahang_Between(14,30,17,0);Serial.println("สัมมาอะระหัง 14:30-17:00");}
    // if(LTime_SammaArahang == false) {SammaArahang_Between(18,2,18,59);Serial.println("สัมมาอะระหัง 18:02-18:59");}

    if (LTime_SammaArahang == false) {return;}
    NSongMode = 0;start_time_relay = "";
    if (LFirstOnly == false) {NEvery_Min_Future = NEvery_Min;LFirstOnly = true;}
    if (NEvery_Min_Future == minute) {
      if (LFirstShow == false) {
        LPlayAuto = false; LFirstShow = true; Serial.println("ได้เวลาหยุดนิ่ง สัมมาอะระหัง อย่างน้อย 1 นาที");
        audio.stopSong(); audio.connecttospeech("ได้เวลาหยุดนิ่ง สัมมาอะระหัง อย่างน้อย 1 นาที", "th");    
        audio.connecttoSD( AFolderFile[1][9].c_str()); 
      }
    }else{LFirstShow = false;
      if (NEvery_Min_Future <= minute) {
        NEvery_Min_Future = NEvery_Min_Future + NEvery_Min;
        Serial.print("สัมมา อะระหัง นาทีที่ ");Serial.println(NEvery_Min_Future);  
      } 
      if (NEvery_Min_Future == 60 and minute == 0) {NEvery_Min_Future = 0;}
      if (NEvery_Min_Future > 60) {NEvery_Min_Future = NEvery_Min;}
      if (minute == 1) {NEvery_Min_Future = NEvery_Min;}
    }
  }
}
void Check_connecttospeech(int Array_Number){
  if (Wifi_Connect == true) {
    Leof_speech = false;audio.connecttospeech(Ascheduled[Array_Number][3].c_str(), "th");
  }else{Leof_speech = true;}
}
void Time_Between(int Array_Number,int Bhour_Start,int Bmin_Start,int Bhour_Stop,int Bmin_Stop) {
  int Nstart = hourmin(Bhour_Start, Bmin_Start);
  int Nstop = hourmin(Bhour_Stop, Bmin_Stop);
  int Nhourmin = hourmin(hour, minute);
  if (Nstart <= Nhourmin and Nstop >= Nhourmin ) { 
    if (LTime_Between == true) {
      Serial.println(Ascheduled[3][1]+":"+Ascheduled[3][2]);Ascheduled[Array_Number][3];
      
      if (Lwait_Speech == false) {
        // if (Wifi_Connect == true) {
        //   Leof_speech = false;audio.connecttospeech(Ascheduled[Array_Number][3].c_str(), "th");
        // }else{Leof_speech = true;}
        Check_connecttospeech(Array_Number);
        Lwait_Speech = true;
      }else{ 
        if (Ascheduled[Array_Number][3].indexOf("ทำวัตรเช้า") >=0){audio.setVolume(7);audio.connecttoSD( AFolderFile[1][1].c_str());}
        if (Ascheduled[Array_Number][3].indexOf("ทำวัตรเย็น") >=0){audio.setVolume(7);audio.connecttoSD( AFolderFile[1][2].c_str());}
        if (Ascheduled[Array_Number][3].indexOf("ฉันเพล") >=0){audio.setVolume(5);audio.connecttoSD( AFolderFile[1][3].c_str());}
        if (Ascheduled[Array_Number][3].indexOf("นั่งสมาธิ") >=0){
          PlayNext_Meditation();
        }
        if (Ascheduled[Array_Number][3].indexOf("ความสะอาด") >=0){audio.connecttoSD( AFolderFile[8][4].c_str());}
        if (Ascheduled[Array_Number][3].indexOf("เดินเล่น ออกกำลังกาย") >=0){audio.connecttoSD( AFolderFile[8][3].c_str());}
        if (Ascheduled[Array_Number][3].indexOf("สรงน้ำ") >=0){audio.connecttoSD( AFolderFile[5][5].c_str());}
        if (Ascheduled[Array_Number][3].indexOf("จำวัตร") >=0){Save_Config(SPIFFS, "/mydir/config.txt");}
        Lwait_Speech = false;LTime_Between = false;LFirstOnly = true;
      }
    }    
  }else{
    // Lwait_Speech = false;
    LMeditation = false;
    Time_SammaArahang(NSammaArahang,2); // ไม่ใช้ ,2 NDouble_Min ใช้ NEvery_Min แทน ใช้ + แทน *
  }
}

void Time_Schedu(){
  //..... Scheduled ตารางเวลาประจำวัน 4 เวลา ค่าพื้นฐาน Nhour_schedu1=6;Nhour_schedu2=13;Nhour_schedu3=18;Nhour_schedu4=22; ......//
  if(LTime_Schedu == true and minute_past != minute) {minute_past = minute;
    if (Leof_speech == true) {LTime_Between = true;}
    for (int i = 1; i <= TotalASpeech; i++){
      Time_Between(i,Ascheduled[i][1].toInt(),Ascheduled[i][2].toInt(),Ascheduled[i][1].toInt(),Ascheduled[i][2].toInt()+1);
    }    
  }
}

void MonkDay() { String Last_MoonPhaseThai = CMoonPhaseThai;  
  if (Leof_speech == true and Lwait_MonkDay == false) { 
    if (NMoonPhase == 7){C_Moon = "พรุ่งนี้วันพระ แรม 8 ค่ำ";Leof_speech = false;audio.connecttospeech(C_Moon.c_str(), "th");}
    if (NMoonPhase == 8 or NMoonPhase == 15){C_Moon = "วันนี้วันพระ "+CMoonPhaseThai;Leof_speech = false;audio.connecttospeech(C_Moon.c_str(), "th");}  
    if (NMoonPhase == 13) { MoonPhase(NYear+543,NMonth,NDay+2);
      String C_MoonPhaseThai = CMoonPhaseThai;C_MoonPhaseThai.replace(" ","");C_MoonPhaseThai.replace("แรม","");C_MoonPhaseThai.replace("ขึ้น",""); C_MoonPhaseThai.replace("ค่ำ",""); 
      int NMoonPhase = C_MoonPhaseThai.toInt(); CMoonPhaseThai = Last_MoonPhaseThai;
      if (NMoonPhase == 1){C_Moon = "พรุ่งนี้วันพระ แรม 14 ค่ำ";}   
      Leof_speech = false;audio.connecttospeech(C_Moon.c_str(), "th");  
      Serial.println(C_Moon); //delay(4000);
    }
    if (NMoonPhase == 14) { MoonPhase(NYear+543,NMonth,NDay+1);
      String C_MoonPhaseThai = CMoonPhaseThai;C_MoonPhaseThai.replace(" ","");C_MoonPhaseThai.replace("แรม","");C_MoonPhaseThai.replace("ขึ้น",""); 
      int NMoonPhase = C_MoonPhaseThai.toInt(); CMoonPhaseThai = Last_MoonPhaseThai;
      if (NMoonPhase == 1){C_Moon = "พรุ่งนี้วันพระ แรม 15 ค่ำ";}    
      Leof_speech = false; audio.connecttospeech(C_Moon.c_str(), "th");  
    }
    Lwait_MonkDay = true;
  }
}

void Slogan1() {
  if (Leof_speech == true and Lwait_Slogan1 == false) { 
    Leof_speech = false;audio.connecttospeech(ASpeech[0].c_str(), "th");
    Lwait_Slogan1 = true;   
  }
}
void Slogan2() {
  if (Leof_speech == true and Lwait_Slogan2 == false) { 
    Leof_speech = false;audio.connecttospeech(ASpeech[1].c_str(), "th");
    Lwait_Slogan2 = true;LFirst_Song = true;    
  }
}

void Play_Speech() {
  MonkDay();  // เช็ค พรุ่งนี้วันพระ วันนี้วันพระ 
  if (Lwait_MonkDay == true) {
    Sawasdee(4,0,10,59,"สวัสดีตอนเช้า, วันนี้ฉันมีความสุขมาก");    
    Sawasdee(11,0,11,59,"สวัสดีตอนเพล, วันนี้ฉันมีความสุขมาก");    
    Sawasdee(12,0,12,59,"สวัสดีตอนเที่ยง, วันนี้ฉันมีความสุขมาก");    
    Sawasdee(13,0,15,59,"สวัสดีตอนบ่าย, วันนี้ฉันมีความสุขมาก");    
    Sawasdee(16,0,17,59,"สวัสดีตอนเย็น, วันนี้ฉันมีความสุขมาก");    
    Sawasdee(18,0,18,30,"สวัสดีตอนพลบค่ำ, วันนี้ฉันมีความสุขมาก");    
    Sawasdee(18,31,23,59,"สวัสดีตอนค่ำ, วันนี้ฉันมีความสุขมาก");    
    Sawasdee(0,0,0,10,"สวัสดีตอนเที่ยงคืน, ทำไมวันนี้อยู่ดึกจัง");    
    Sawasdee(0,11,3,59,"สวัสดีตอนดึก, ขณะนี้เวลานอน ควรหลับในอู่ทะเลบุญ");
  } 
  if (Lwait_MonkDay == true and Lwait_Sawasdee == true) { // คำขวัญประจำวัน
    Slogan1(); 
    if (Lwait_Slogan1 == true ) {Slogan2();}
  }
}
