#include <Arduino.h>
#include <Config.h>
//#include <time.h>
//...... Get Date Time from Internet
long timezone = 7;  // 2;
byte daysavetime = 0; // 1;
String CMoonPhase,CMoonPhaseThai,start_time_relay,CString = "";
String CDay,CMon,CYear,CWday,CDateTime,wdayName,CWdayThai,monthName = "";
int NMonth,NDay,Nmdaymonyear,NDoW = 0;
int NYear = 1970;
bool LTime_Between,Ltalk_Firsttime,LFirstOnly = false;
bool GetLocalTime(struct tm * info, uint32_t ms);

Audio audio;
struct tm tmstruct ;

File file;
const char filename[] = "/recording.wav";
const int headerSize = 44;

//Pinos de conexão do ESP32 e o módulo de cartão SD
#define SD_CS          5
#define SPI_MOSI      23
#define SPI_MISO      19
#define SPI_SCK       18

//Pinos de conexão do ESP32-I2S e o módulo I2S/DAC CJMCU 1334
#define I2S_DOUT      25
#define I2S_BCLK      27
#define I2S_LRC       26

// Audio audio;
// WiFiMulti wifiMulti;

// #include <DHT.h>
#define DHTPIN 33  // 27 //Connect Out pin to D2 in NODE MCU
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);
float h,t = 0;

void sendDHT(void);
void sendDHT(void) {
  h = dht.readHumidity();
  t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {Serial.println("Failed to read from DHT sensor!");return;}
  Serial.print("  Temperature : ");Serial.print(t);
  Serial.print("  Humidity : ");Serial.println(h);
}

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

// put function declarations here:
int myFunction(int, int);

uint8_t hour    = 6;
uint8_t minute  = 59;
uint8_t sec     = 45;

bool f_time     = false;
int8_t timefile = -1;
char chbuf[100];

//................... ตัวแปร เก็บใน SPIFFS (in Ram of Board) ...........................//
String CSound,CPlay_Test = "";
bool Wifi_Connect,Lconfirm,LScheduled,LReplace_Config,LConnect_internet_Auto,LStartSong,LTime_Schedu,LPlayAuto,LTalk_Everytime = false;
int NVolume = 10;
String ATime[40][2];String Ascheduled[30][3];
const char * Replace_Config;
int NSammaArahang = 0;

#include <IRremote.h>
int RECV_PIN = 32;
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long last,last_Sleep = millis();

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
  connectInternet();  
  if (Wifi_Connect == true) {
    configTime(3600 * timezone, daysavetime * 3600, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");
    delay(1000);GetTimeInternet();
  }
  irrecv.enableIRIn(); //..... Start the receiver ...............//
}
int z,N;
void IRrecv_void(void);
void IRrecv_void(void) {
  if (irrecv.decode(&results)) { 
    if (results.value == ir_seven){if (WiFi.status() == false){connectInternet();}else{WiFi.disconnect();Serial.println("disconnect");N=N+1;Serial.println(N);}}
  }
  irrecv.resume(); // Receive the next value  
  delay(50);
}

void loop(){N++;
  IRrecv_void();    // ใช้ Remote Control 
  z = myFunction(5,7);
  Serial.println(z+N);
  delay(1500);
  if (Wifi_Connect == true and NYear == 1970) {
    configTime(3600 * timezone, daysavetime * 3600, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");
    GetTimeInternet();
  }
}
