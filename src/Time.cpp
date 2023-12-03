#include <Arduino.h>
#include <Config.h>

//............................................ บอกเวลาเป็นเสียงพูด เวลาปัจจุบัน ....................................//
void talk_time_current() {
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

// //................................. บอกเวลาเป็นเสียงพูด ทุกกี่นาที ระหว่างเวลาตี 5 ถึง  4 ทุ่ม ....................................//
void talk_everytime(int every_hour,int every_minute){
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
