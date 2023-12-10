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
  int Nstart = hourmin(Bhour_Start, Bmin_Start);
  int Nstop = hourmin(Bhour_Stop, Bmin_Stop);
  int Nhourmin = hourmin(hour, minute);
  if (Nstart <= Nhourmin and Nstop >= Nhourmin ) {
    Serial.println(CSawasdee); 
    audio.connecttospeech(CSawasdee.c_str(), "th");
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
  int Nhourmin = hourmin(hour, minute);
  if (13*60 <= Nhourmin) {SammaArahang_Between(13,0,17,0);}else{SammaArahang_Between(8,30,11,0);}

  if (LTime_SammaArahang == false) {return;}
  NSongMode = 0;start_time_relay = "";
  if (LFirstOnly == false) {NEvery_Min_Future = NEvery_Min;LFirstOnly = true;}
  if (NEvery_Min_Future == minute) {
    if (LFirstShow == false) {
      LPlayAuto = false; LFirstShow = true; Serial.println("ได้เวลาหยุดนิ่ง สัมมาอะระหัง อย่างน้อย 1 นาที");
      audio.stopSong(); audio.connecttospeech("ได้เวลาหยุดนิ่ง สัมมาอะระหัง อย่างน้อย 1 นาที", "th");     
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

void Time_Between(int Array_Number,int Bhour_Start,int Bmin_Start,int Bhour_Stop,int Bmin_Stop) {
  int Nstart = hourmin(Bhour_Start, Bmin_Start);
  int Nstop = hourmin(Bhour_Stop, Bmin_Stop);
  int Nhourmin = hourmin(hour, minute);
  if (Nstart <= Nhourmin and Nstop >= Nhourmin ) {
    if (LTime_Between == true) {
      Serial.println(Ascheduled[8][1]+" = "+Ascheduled[8][2]);LTime_Between = false;LFirstOnly = true;
      audio.connecttospeech(Ascheduled[Array_Number][3].c_str(), "th");
    }    
  }else{
    Time_SammaArahang(NSammaArahang,2); // ไม่ใช้ ,2 NDouble_Min ใช้ NEvery_Min แทน ใช้ + แทน *
  }
}

void Time_Schedu(){
  //..... Scheduled ตารางเวลาประจำวัน 4 เวลา ค่าพื้นฐาน Nhour_schedu1=6;Nhour_schedu2=13;Nhour_schedu3=18;Nhour_schedu4=22; ......//
  if(LTime_Schedu == true and minute_past != minute) {minute_past = minute;
    if (Leof_speech == true) {LTime_Between = true;}
    for (int i = 1; i <= 20; i++){
      Time_Between(i,Ascheduled[i][1].toInt(),Ascheduled[i][2].toInt(),Ascheduled[i][1].toInt(),Ascheduled[i][2].toInt()+1);
    }    
  }
}

void Play_Speech() {
  Sawasdee(4,0,10,59,"สวัสดีตอนเช้า, วันนี้ฉันมีความสุขมาก");    
  Sawasdee(11,0,11,59,"สวัสดีตอนเพล, วันนี้ฉันมีความสุขมาก");    
  Sawasdee(12,0,12,59,"สวัสดีตอนเที่ยง, วันนี้ฉันมีความสุขมาก");    
  Sawasdee(13,0,15,59,"สวัสดีตอนบ่าย, วันนี้ฉันมีความสุขมาก");    
  Sawasdee(16,0,17,59,"สวัสดีตอนเย็น, วันนี้ฉันมีความสุขมาก");    
  Sawasdee(18,0,18,30,"สวัสดีตอนพลบค่ำ, วันนี้ฉันมีความสุขมาก");    
  Sawasdee(18,31,23,59,"สวัสดีตอนค่ำ, วันนี้ฉันมีความสุขมาก");    
  Sawasdee(0,0,0,10,"สวัสดีตอนเที่ยงคืน, ทำไมวันนี้อยู่ดึกจัง");    
  Sawasdee(0,11,3,59,"สวัสดีตอนดึก, ขณะนี้เวลานอน ควรหลับในอู่ทะเลบุญ");  
  // MonkDay();  // เช็ค พรุ่งนี้วันพระ วันนี้วันพระ
  LFirst_Song = true; 
}
