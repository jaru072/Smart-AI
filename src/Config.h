#include "FS.h"
#include <LITTLEFS.h>
#include "WiFi.h"    
#include "Audio.h"
#include <vfs_api.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

extern Audio audio;
extern struct tm tmstruct ;
extern time_t now;
extern uint8_t hour,minute,second,sec;
extern bool f_time;
extern int8_t timefile;
extern char chbuf[100];

#ifndef CONFIG_LITTLEFS_FOR_IDF_3_2
 #include <time.h>
#endif
extern bool GetLocalTime(struct tm * , uint32_t);
extern int DayofWeek(int,int,int);
extern void talk_time_current(void);
extern void GetTimeInternet(void);
extern int NYear,NMonth,NDay,Nmdaymonyear,NDoW;
extern String CDay,CMon,CYear,CWday,CDateTime,wdayName,CWdayThai,monthName;
extern int MoonPhase(int ,int ,int );   
extern bool LTime_Between,Wifi_Connect,LFirstOnly;
extern String CMoonPhase,CMoonPhaseThai,Ltalk_Firsttime,start_time_relay,CString;
extern float h,t;
//................... ตัวแปร เก็บใน SPIFFS (in Ram of Board) ...........................//
extern String CSound;
extern String CPlay_Test;
extern bool LScheduled,LReplace_Config,LConnect_internet_Auto,LStartSong,LTime_Schedu,LPlayAuto,LTalk_Everytime;
extern int NVolume;
extern String ATime[40][2];
extern String Ascheduled[30][3];
extern const char * Replace_Config;
extern int NSammaArahang;

