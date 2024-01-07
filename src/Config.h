#include "FS.h"
#include "Audio.h"
#include <vfs_api.h>
// #include <Adafruit_Sensor.h>
// #include <DHT.h>
// #include <DHT_U.h>
#include "WiFiMulti.h"
#include "SPI.h"
#include "SD.h"
// #include <esp_now.h>

// #include <BleKeyboard.h>
// extern BleKeyboard bleKeyboard;
// extern void Task_Ble_Keyboard(void * );
// extern void Ble_Keyboard();
//Pinos de conexão do ESP32 e o módulo de cartão SD
#define SD_CS          5
#define SPI_MOSI      23
#define SPI_MISO      19
#define SPI_SCK       18

//Pinos de conexão do ESP32-I2S e o módulo I2S/DAC CJMCU 1334
#define I2S_DOUT      25
#define I2S_BCLK      27
#define I2S_LRC       26

//............. Driver and Varible Control Audio ....................//
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
// .............. Blynk Varible ......................//
extern String CBlynk_Remote;
//...... Get Date Time from Internet
extern long timezone;  
extern byte daysavetime; 
extern void myTimerEvent();
extern void serialEvent();
extern String CBlynkReceive,CBlynk_Cut;
extern bool bool_startsWith;
extern void Add_Zero(int,int);
//................ Varible New .......................//
extern String R_Text;
extern bool LMeditation;

// ตัวแปร Send_Serial_Monitor // extern String Question;
extern String AI_Word[2][10];
extern bool LSend_Serial;

extern String ssid,pass;
extern int TotalconnectCount;

extern bool LscanNetworks;
extern int NFolder_Meditation,NFile_Meditation,count1,count2;
extern TaskHandle_t task1Handle;
extern TaskHandle_t task2Handle;

extern void PlayNext_Meditation(String );
extern void Send_Value(String ,int );
extern void Save_Config(fs::FS &fs, const char * path);
extern void Check_Replace_SPIFFS(const char* );
extern void Check_Delete_SPIFFS(const char* );
extern void printDirectory(File , int );

extern void RTOS_Setup();
extern void task2(void * );
extern void task1(void * );
extern void uploadToAWS(void * );
extern void toggleLED(void * );
extern void Send_SerialMonitor();
extern void Read_Ascheduled();
extern void Check_Wifi(int );
extern void Play_Speech();
extern void Sawasdee(int ,int ,int ,int ,String );
extern void Time_Between(int ,int ,int ,int ,int );
extern void Time_Schedu();
extern void talk_everytime(int ,int );
extern void Time_SammaArahang(int ,int );
extern void SammaArahang_Between(int,int,int,int);
extern void MonkDay();
extern void PlayAuto();
extern void Check_SDcard();
extern void check_ssid();
extern void connectInternet();
extern void talk_time_current(void);
extern void GetTimeInternet(void);
extern void IRrecv_void();
extern void List_Config(void);
extern void Check();
extern void Start_Config(void);
//... ตัวแปร SETUP
extern int NZero_Extra;
extern int hourmin(int ,int );
//... ตัวแปร Audio and Sdcard
extern bool Lwait_Slogan2,Lwait_Slogan1,Lwait_Sawasdee,Lwait_Slogan,Lwait_MonkDay,Lwait_Speech,LOpenURL,LPlayAuto,LFirst_Song,Leof_mp3,LSDcard;
extern unsigned long last_Sleep,last_Remote,last_timer,last_Stopsong;
extern String ASpeech[],AFolderFile[20][31],ASong[100]; // ตัวแปรอเรย์ 20 โฟลเดอร์/ละ 30 ไฟล์ แต่ต้องใช้ 21 Array
extern String OldFolder,FolderName,syncword,S_info,OldFolderName;
extern int TotalASpeech,count,NAutoFolder,NumberFile,TotalASong,AFolderSong,NFolder,NFile;
//... ตัวแปร Play Song
extern int NPlayEvery,NPlayAuto,FolderPlay,FilePlay,NSongMode;
extern bool LNumber_Sound,Leof_speech;
//... ตัวแปร Time
extern int minute_past,NEvery_Min_Future,total_every_minute,old_every_minute,every_hour,every_minute;
extern bool GetLocalTime(struct tm * , uint32_t);
extern int DayofWeek(int,int,int);
extern int NAlarmClock,every_minute,NYear,NMonth,NDay,Nmdaymonyear,NDoW;
extern String CDay,CMon,CYear,CWday,CDateTime,CWdayThai,CDateTime_Thai;
extern String wdayName[],monthName [];
extern int NMoonPhase,MoonPhase(int ,int ,int );   
extern bool LTime_SammaArahang,LFirstShow,LTime_Between,Wifi_Connect,Ltalk_Firsttime,LFirstOnly;
extern String Str_FileNumber,CTime,C_Moon,CMoonPhase,CMoonPhaseThai,start_time_relay,CString;
//... ตัวแปร อุณหภูมิ ความชื้น
extern float h,t;
//... ตัวแปร Remote
extern int NSleep,volume_old,NChange_Remote,Total_last_Sleep;
extern bool LcontrolBoard;
extern  String Control_Board;
//................... ตัวแปร เก็บใน SPIFFS (in Ram of Board) ...........................//
extern String CSound;
extern String CPlay_Test;
extern bool LScheduled,LReplace_Config,LConnect_internet_Auto,LStartSong,LTime_Schedu,LPlayAuto,LTalk_Everytime;
extern int NVolume,N,z;
extern String ATime[40][2];
extern String Ascheduled[30][3];
extern const char * Replace_Config;
extern int NSammaArahang;

// #define code_ON  33446055 // ค่าปุ่มกด Auto ON
// #define code_OFF 33446055 // ค่าปุ่มกด Auto OFF
// #define code_Mode1 33439935 // ค่าปุ่มกด 3H
// #define code_Mode2 33472575 // ค่าปุ่มกด 5H
// #define code_Mode3  33448095 // ค่าปุ่มกด 8H
// #define code_Mode4  33464415 // ค่าปุ่มกด ดำขาว

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

