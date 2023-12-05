#include <Arduino.h>
#include <Config.h>
#include "Audio.h"
#include <time.h>

//....................................... Get Date Time from Internet ...........................................//
void GetTimeInternet(void) { 
  if (Wifi_Connect == true) {
    tmstruct.tm_year = 0; GetLocalTime(&tmstruct, 1000);
    hour = tmstruct.tm_hour;minute = tmstruct.tm_min;
    char Cha[32];strftime(Cha, sizeof Cha, "%Y-%m-%d %H:%M:%S", &tmstruct); CDateTime = String(Cha);
    NYear = (tmstruct.tm_year + 1900); NMonth = (tmstruct.tm_mon + 1); NDay = (tmstruct.tm_mday);
    CYear = String(NYear);if(NMonth < 10){CMon = ("0"+String(NMonth));}else{CMon = String(NMonth);} if(NDay < 10){CDay = ("0"+String(NDay));}else{CDay = String(NDay);}  DayofWeek(NDay,NMonth,NYear); // while (tmstruct.tm_year < 100);
    if (NYear > 1970) {
      Serial.print(" "+CWdayThai);Serial.printf(" %02d-%02d-%d %02d:%02d:%02d",tmstruct.tm_mday, tmstruct.tm_mon + 1,tmstruct.tm_year + 1900, tmstruct.tm_hour,tmstruct.tm_min, tmstruct.tm_sec);
    }
    if (Ltalk_Firsttime == 0 or CMoonPhaseThai == "") {MoonPhase(NYear+543,NMonth,NDay);start_time_relay = "**";  // กำหนดค่า start_time_relay = "**" เพื่อส่งผลไปยัง talk_time_current() 
      Nmdaymonyear = (String(NYear)+CMon+CDay+String(NDoW)).toInt();
      // คำนวณข้างขึ้น ข้างแรม ครั้งเดียว Ltalk_Firsttime = 1
      if (CMoonPhaseThai != "") { 
        Serial.print("*** คำนวณ ข้างขึ้น ข้างแรม *** "); Serial.println(CMoonPhaseThai); 
        Ltalk_Firsttime = 1; talk_time_current();start_time_relay = "";  // บอกเวลาครั้งแรก ครั้งเดียว  
      }
    } 
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