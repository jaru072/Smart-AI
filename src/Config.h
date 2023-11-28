#include "FS.h"
#include <LITTLEFS.h>
#include "WiFi.h"    
#include "Audio.h"
#include <vfs_api.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
// #include <IRremote.h>
#include "WiFiMulti.h"
#include "SPI.h"
#include "SD.h"
#include <esp_now.h>

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
//...... Get Date Time from Internet
extern long timezone;  
extern byte daysavetime; 

// extern int RECV_PIN;
// extern IRrecv irrecv(RECV_PIN);
// extern decode_results results;

extern bool GetLocalTime(struct tm * , uint32_t);
extern int DayofWeek(int,int,int);
extern void talk_time_current(void);
extern void GetTimeInternet(void);
extern int NYear,NMonth,NDay,Nmdaymonyear,NDoW;
extern String CDay,CMon,CYear,CWday,CDateTime,wdayName,CWdayThai,monthName;
extern int MoonPhase(int ,int ,int );   
extern bool LTime_Between,Wifi_Connect,Ltalk_Firsttime,LFirstOnly;
extern String CMoonPhase,CMoonPhaseThai,start_time_relay,CString;
extern float h,t;
//................... ตัวแปร เก็บใน SPIFFS (in Ram of Board) ...........................//
extern String CSound;
extern String CPlay_Test;
extern bool LScheduled,LReplace_Config,LConnect_internet_Auto,LStartSong,LTime_Schedu,LPlayAuto,LTalk_Everytime;
extern int NVolume,N;
extern String ATime[40][2];
extern String Ascheduled[30][3];
extern const char * Replace_Config;
extern int NSammaArahang;

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

#define code_ON  33446055 // ค่าปุ่มกด Auto ON
#define code_OFF 33446055 // ค่าปุ่มกด Auto OFF
#define code_Mode1 33439935 // ค่าปุ่มกด 3H
#define code_Mode2 33472575 // ค่าปุ่มกด 5H
#define code_Mode3  33448095 // ค่าปุ่มกด 8H
#define code_Mode4  33464415 // ค่าปุ่มกด ดำขาว

#define R_zero 16738455
#define R_one 16724175
#define R_two 16718055
#define R_three 16743045
#define R_four 16716015
#define R_five 16726215
#define R_six 16734885
#define R_seven 16728765
#define R_eight 16730805
#define R_nine 16732845
#define R_star 16753245   // Power
#define R_sharp 16736925  // Mode
#define R_ok 16769055     // EQ
#define R_top 16748655
#define R_right 16761405
#define R_bottom 16754775
#define R_left 16712445
#define R_Mute 16769565  //   ---space16769565
#define R_PlayPause 16720605  // ---space
#define R_RPT 16750695 //   ---space
#define R_U_SD 16756815  // ---space


