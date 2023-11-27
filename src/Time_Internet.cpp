#include <Arduino.h>
#include <Config.h>
#include "Audio.h"
#include <time.h>

// bool GetLocalTime(struct tm * , uint32_t);
// int DayofWeek(int,int,int);
// void talk_time_current(void);
//....................................... Get Date Time from Internet ...........................................//
void GetTimeInternet(void) { 
  if (Wifi_Connect == true) {
    tmstruct.tm_year = 0; GetLocalTime(&tmstruct, 1000);hour = tmstruct.tm_hour;minute = tmstruct.tm_min;
    char Cha[32];strftime(Cha, sizeof Cha, "%Y-%m-%d %H:%M:%S", &tmstruct); CDateTime = String(Cha);
    NYear = (tmstruct.tm_year + 1900); NMonth = (tmstruct.tm_mon + 1); NDay = (tmstruct.tm_mday);
    CYear = String(NYear);if(NMonth < 10){CMon = ("0"+String(NMonth));}else{CMon = String(NMonth);} if(NDay < 10){CDay = ("0"+String(NDay));}else{CDay = String(NDay);}  DayofWeek(NDay,NMonth,NYear); // while (tmstruct.tm_year < 100);
    Serial.print(" "+CWday);Serial.printf(" %02d-%02d-%d %02d:%02d:%02d",tmstruct.tm_mday, tmstruct.tm_mon + 1,tmstruct.tm_year + 1900, tmstruct.tm_hour,tmstruct.tm_min, tmstruct.tm_sec);
//    Serial.print(" LPlayFinish ");Serial.print(LPlayFinish);LPlayFinish = 0;Serial.print(" Ltalk_Everytime = ");Serial.print(Ltalk_Everytime);
//    Serial.print(" N = ");Serial.print(N);Serial.print(" Leof_mp3 = ");Serial.print(Leof_mp3);Serial.print(" Leof_speech = ");Serial.println(Leof_speech);
    if (Ltalk_Firsttime == 0) {MoonPhase(NYear+543,NMonth,NDay);start_time_relay = "**";  // กำหนดค่า start_time_relay = "**" เพื่อส่งผลไปยัง talk_time_current() 
      Nmdaymonyear = (String(NYear)+CMon+CDay+String(NDoW)).toInt();
      Serial.println("***** talk_time First *****"); Ltalk_Firsttime = 1; talk_time_current();start_time_relay = "";  // บอกเวลาครั้งแรก ครั้งเดียว  
    } // คำนวณข้างขึ้น ข้างแรม ครั้งเดียว Ltalk_Firsttime = 1;
  }
}

int DayofWeek(int NDay,int NMonth,int NYear) {int d = NDay; int m = NMonth;int y = NYear - (14 - m) / 12;  
  int x = y + y / 4 - y / 100 + y / 400;  m = m + 12 * ( (14 - m) / 12 ) -2 ;  int DoW = (d + x + 31 * m /  12 ) % 7; NDoW = DoW;
  if(DoW == 0) { CWday = "Sunday";}if(DoW == 1) { CWday = "Monday";}if(DoW == 2) { CWday = "Tuesday";}if(DoW == 3) { CWday = "Wednesday";}if(DoW == 4) { CWday = "Thursday";}if(DoW == 5) { CWday = "Friday";}if(DoW == 6) { CWday = "Saturday";}
  if(DoW == 0){CWdayThai = wdayName[7];}else{CWdayThai = wdayName[DoW];} 
  return NDay,NMonth,NYear;
}

bool GetLocalTime(struct tm * info, uint32_t ms) {uint32_t count = ms / 10; time_t now;time(&now); localtime_r(&now, info);
  if (info->tm_year > (2016 - 1900)) {return true;}
  while (count--) {delay(10);time(&now);localtime_r(&now, info);if (info->tm_year > (2016 - 1900)) {return true;}}return false;
}
//............................................ บอกเวลาเป็นเสียงพูด เวลาปัจจุบัน ....................................//
void talk_time_current(void) {
  if (hour == 0 && minute == 0) {
    Serial.println("Not Talk Time if hour = 0 and minute = 0");
  }else {
    if (Wifi_Connect == true) {
      Serial.print(" "+CWday);Serial.printf(" %02d-%02d-%d %02d:%02d:%02d\n",tmstruct.tm_mday, tmstruct.tm_mon + 1,tmstruct.tm_year + 1900, tmstruct.tm_hour,tmstruct.tm_min, tmstruct.tm_sec);
      // ข้อมูลที่จะดาวน์โหลด "วันของสัปดาห์ วัน/เดือน/ปี" คือตัวแปร CString , ตัวแปร CStringEng ใช้แสดงผลที่ Serial Monitor หรือ จอ LCD เท่านั้น
      if(start_time_relay == "*") {int NTemperature = t;int NHumidity = h;
          audio.connecttospeech(("อุณหภูมิ "+String(NTemperature)+" องศาเซลเซียส"+" ความชื้น "+String(NHumidity)).c_str(), "th");
      }else{        
        if(start_time_relay.startsWith("**"))  {
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
