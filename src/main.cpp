#include <Arduino.h>
#include <Config.h>

// put function declarations here:
int myFunction(int, int);

bool LTime_Between,LFirstOnly = false;
//................... ตัวแปร เก็บใน SPIFFS (in Ram of Board) ...........................//
String CSound,CPlay_Test = "";
bool Wifi_Connect,Lconfirm,LScheduled,LReplace_Config,LConnect_internet_Auto,LStartSong,LTime_Schedu,LPlayAuto,LTalk_Everytime = false;
int NVolume = 10;
String ATime[40][2];String Ascheduled[30][3];
const char * Replace_Config;
int NSammaArahang = 0;
void List_Config(void);
void Start_Config(void);
void check_ssid(void);
void connectInternet(void);
//....................... SETUP ...........................//
void setup() {
  Serial.begin(115200); Serial.println("initializing...");
  //... Load data from Spiffs file /mydir/config.txt (Ram on board)
  // Start_Config();
  List_Config();  
  //........... Start Wifi .............//
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  check_ssid();
  int connectInternet(20);  
}
int z,N;
void loop(){N++;
  z = myFunction(5,7);
  Serial.println(z+N);
  delay(1200);
}
