// this example will play a track and then every 60 seconds
// it will play an advertisement
//
// it expects the sd card to contain the following mp3 files
// but doesn't care whats in them
//
// sd:/01/001.mp3 - the song to play, the longer the better
// sd:/advert/0001.mp3 - the advertisement to interrupt the song, keep it short
#include <IRremote.h>
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

#define code_ON  33446055 // ค่าปุ่มกด ON
#define code_OFF 33446055 // ค่าปุ่มกด OFF
#define code_Mode1 33439935 // ค่าปุ่มกด Mode 1
#define code_Mode2 33472575 // ค่าปุ่มกด Mode 2
#define code_Mode3  33448095 // ค่าปุ่มกด Mode 3
#define code_Mode4  33464415 // ค่าปุ่มกด Mode 4

int RECV_PIN = 2;
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long last = millis();

#include <SoftwareSerial.h>
#include <DFMiniMp3.h>

// implement a notification class,
// its member methods will get called 
//
class Mp3Notify
{
public:
  static void PrintlnSourceAction(DfMp3_PlaySources source, const char* action)
  {
    if (source & DfMp3_PlaySources_Sd) 
    {
        Serial.print("SD Card, ");
    }
    if (source & DfMp3_PlaySources_Usb) 
    {
        Serial.print("USB Disk, ");
    }
    if (source & DfMp3_PlaySources_Flash) 
    {
        Serial.print("Flash, ");
    }
    Serial.println(action);
  }
  static void OnError(uint16_t errorCode)
  {
    // see DfMp3_Error for code meaning
    Serial.println();
    Serial.print("Com Error ");
    Serial.println(errorCode);
  }
  static void OnPlayFinished(DfMp3_PlaySources source, uint16_t track)
  {
    Serial.print("Play finished for #");
    Serial.println(track);  
  }
  static void OnPlaySourceOnline(DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "online");
  }
  static void OnPlaySourceInserted(DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "inserted");
  }
  static void OnPlaySourceRemoved(DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "removed");
  }
};

// instance a DFMiniMp3 object, 
// defined with the above notification class and the hardware serial class
//
// DFMiniMp3<HardwareSerial, Mp3Notify> mp3(Serial1);

// Some arduino boards only have one hardware serial port, so a software serial port is needed instead.
// comment out the above definition and uncomment these lines
SoftwareSerial secondarySerial(7, 6); // RX, TX
DFMiniMp3<SoftwareSerial, Mp3Notify> mp3(secondarySerial);

uint32_t lastAdvert; // track time for last advertisement

void setup() 
{
  Serial.begin(115200);
  irrecv.enableIRIn(); // Start the receiver

  Serial.println("initializing...");
  
  mp3.begin();
  uint16_t volume = mp3.getVolume();
  Serial.print("volume was ");
  Serial.println(volume);
  mp3.setVolume(20);
  volume = mp3.getVolume();
  Serial.print(" and changed to  ");
  Serial.println(volume);
  
  Serial.println("track 1 from folder 1"); 
  // mp3.playFolderTrack(1, 3); // sd:/01/003.mp3

  lastAdvert = millis();
}

void loop() {
  // uint32_t now = millis();
  // if ((now - lastAdvert) > 30000)
  // {
  //   // interrupt the song and play the advertisement, it will
  //   // return to the song when its done playing automatically
  //   mp3.playAdvertisement(1); // sd:/advert/0001.mp3
  //   // mp3.playRandomTrackFromAll();
  //   lastAdvert = now;
  // }
  // mp3.loop();
  IRrecv_void();
}

//........................................ รับค่าของ Remote Control ....................................//
void IRrecv_void() {
  if (irrecv.decode(&results)) { 
    if (millis() - last > 50) {
      if ( (results.value != 0xFFFFFFFF) && (results.value != 0x00) )
        if (results.value == ir_zero) {}
        // if (results.value == ir_one)  {Serial.println("Button 1");mp3.stop();delay(100);mp3.playFolderTrack(1, 1);} // sd:/01/001.mp3}
        // if (results.value == ir_two)  {Serial.println("Button 2");mp3.stop();delay(100);mp3.playFolderTrack(1, 2);} // sd:/01/002.mp3}
        // if (results.value == ir_three){Serial.println("Button 3");mp3.stop();delay(100);mp3.playFolderTrack(1, 3);} // sd:/01/003.mp3}
        if (results.value == ir_one)  {Serial.println("Button 1");mp3.playFolderTrack(1, 1);} // sd:/01/001.mp3}
        if (results.value == ir_two)  {Serial.println("Button 2");mp3.playFolderTrack(1, 2);} // sd:/01/002.mp3}
        if (results.value == ir_three){Serial.println("Button 3");mp3.playFolderTrack(1, 3);} // sd:/01/003.mp3}
        if (results.value == ir_four) {Serial.println("Button 4");mp3.playFolderTrack(1, 8);waitMilliseconds(4000);mp3.playFolderTrack(1, 15);} // sd:/01/003.mp3}
        if (results.value == ir_five) {mp3.playFolderTrack(1, 17);}
        if (results.value == ir_six) {mp3.playFolderTrack(1, 15);}
        if (results.value == ir_seven) {}
        if (results.value == ir_eight) {}
        if (results.value == ir_nine) {}
        if (results.value == ir_zero) {}
        if (results.value == ir_star) {Serial.println("Random Track");mp3.playRandomTrackFromAll();}
        if (results.value == ir_sharp) {Serial.println("Pause");mp3.pause();delay(5000);mp3.start();}
        if (results.value == ir_ok) {Serial.println("Stop");mp3.stop();}
        if (results.value == ir_top) {Serial.println("increaseVolume");mp3.increaseVolume();}
        if (results.value == ir_right) {mp3.nextTrack();}
        if (results.value == ir_bottom) {Serial.println("decreaseVolume");mp3.decreaseVolume();}
        if (results.value == ir_left) {mp3.prevTrack();}
        // delay(1000);
      }
      irrecv.resume(); // Receive the next value                       
    }
  // }
}  

void waitMilliseconds(uint16_t msWait)
{
  uint32_t start = millis();
  
  while ((millis() - start) < msWait)
  {
    // calling mp3.loop() periodically allows for notifications 
    // to be handled without interrupts
    mp3.loop(); 
    delay(1);
  }
}
