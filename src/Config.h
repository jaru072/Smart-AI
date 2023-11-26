#include "FS.h"
#include <LITTLEFS.h>
#include "WiFi.h"
#ifndef CONFIG_LITTLEFS_FOR_IDF_3_2
 #include <time.h>
#endif
/* You only need to format LITTLEFS the first time you run a
   test or else use the LITTLEFS plugin to create a partition
   https://github.com/lorol/arduino-esp32littlefs-plugin */
   


// extern String ssid;
// extern String pass;
extern bool LTime_Between,Wifi_Connect,LFirstOnly;

//................... ตัวแปร เก็บใน SPIFFS (in Ram of Board) ...........................//
extern String CSound;
extern String CPlay_Test;
extern bool LScheduled,LReplace_Config,LConnect_internet_Auto,LStartSong,LTime_Schedu,LPlayAuto,LTalk_Everytime;
extern int NVolume;
extern String ATime[40][2];
extern String Ascheduled[30][3];
extern const char * Replace_Config;
extern int NSammaArahang;

