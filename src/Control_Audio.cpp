#include <Arduino.h>
#include <Config.h>

//........................................ Void for Control Audio .............................................//
void audio_info(const char *info) {
  if (info != "syncword not found") { syncword = ""; //if(Total_last_Sleep != 7000) {Leof_mp3 = false;}
    Serial.print("info        "); Serial.println(info);String Strinfo = info;
    if (Strinfo.endsWith("failed!") or Strinfo.endsWith("Bad Request")) {Serial.println("Can not open URL");LOpenURL = false;Leof_speech = true;}
    if (Strinfo.endsWith("with PSRAM!")) {Leof_mp3 = true;}    
  }else{if(syncword == "") {syncword = "syncword not found";Serial.println(info);}}
}

void audio_id3data(const char *info) {
  Serial.print("id3data     "); Serial.println(info);
  Leof_mp3 = false;
}  //id3 metadata

void audio_eof_mp3(const char *info){ last_Sleep=millis();
  Leof_mp3 = true;Leof_speech = true;Lwait_Speech = false;
  Serial.print("Leof_mp3 = ");Serial.print(Leof_mp3); Serial.print(" eof_mp3     "); Serial.println(info);
  audio.setVolume(NVolume);
}

void audio_showstation(const char *info) {Serial.print("station     "); Serial.println(info);}

void audio_showstreaminfo(const char *info) {Serial.print("streaminfo  "); Serial.println(info);}

void audio_showstreamtitle(const char *info) {Serial.print("streamtitle "); Serial.println(info);}

void audio_bitrate(const char *info) {Serial.print("bitrate     "); Serial.println(info);}

void audio_commercial(const char *info) {Serial.print("commercial  "); Serial.println(info);} //commercial

void audio_icyurl(const char *info){ Serial.print("icyurl      "); Serial.println(info);} //homepage

void audio_lasthost(const char *info) { //stream URL played
  Serial.print("lasthost    "); Serial.println(info);
  Leof_speech = false;
  audio.setVolume(NVolume);//Serial.println("Volume = "+String(NVolume));
}

void audio_eof_speech(const char *info){last_Sleep=millis();
  Leof_mp3 = true;Leof_speech = true;
  Serial.print("eof_speech "); Serial.println(info);
}

void PlayAuto() { if (NAutoFolder == 2 or NAutoFolder == 3 or NAutoFolder == 4) {NAutoFolder = 5;NumberFile = 1;} // ไม่เปิดนำนังสมาธิ
  String Autofile = AFolderFile[NAutoFolder][NumberFile];Serial.print("Autofile = ");Serial.println(Autofile);
  if (Autofile == "") {NAutoFolder= 1;NumberFile = 1;Autofile = AFolderFile[NAutoFolder][NumberFile];}
  audio.connecttoSD( Autofile.c_str() ); NumberFile++;  //audio.loop();

  if (AFolderFile[NAutoFolder][NumberFile] == ""){ 
    // NumberFile = 1; LPlayAuto = false;  // Auto เฉพาะในโฟลเดอร์ที่เลือกเท่านั้น 
    if (NAutoFolder == 8 ) {NAutoFolder = 0;}
    NAutoFolder++; NumberFile = 1; //
  }
}
