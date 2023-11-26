#include <Arduino.h>
#include <Config.h>

// put function declarations here:
int myFunction(int, int);

bool LTime_Between,LFirstOnly = false;
//................... ตัวแปร เก็บใน SPIFFS (in Ram of Board) ...........................//
String CSound,CPlay_Test = "";
bool LScheduled,LReplace_Config,LConnect_internet_Auto,LStartSong,LTime_Schedu,LPlayAuto,LTalk_Everytime = false;
int NVolume = 10;
String ATime[40][2];String Ascheduled[30][3];
const char * Replace_Config;
int NSammaArahang = 0;
void List_Config(void);
void Start_Config(void);
//....................... SETUP ...........................//
void setup() {
  Serial.begin(115200);delay(3000); Serial.println("initializing...");
  // Start_Config();
  List_Config();
}
int z,N;
void loop(){N++;
  z = myFunction(5,7);
  Serial.println(z+N);
  delay(1200);
}
