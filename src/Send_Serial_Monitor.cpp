#include <Arduino.h>
#include <Config.h>

String Question = "";

void Send_SerialMonitor(){ 
  Serial.print("Ask your Question : ");
  while (!Serial.available())
  {
    audio.loop();
  }
  while (Serial.available())
  {
    char add = Serial.read();
    Question = Question + add;
    delay(1);
  }
  int len = Question.length();
  Question = Question.substring(0, (len));
  Question = "\"" + Question + "\"";
  Serial.println(Question);
  if (Question.indexOf("end") >= 0) {LSend_Serial = false;audio.connecttospeech("สิ้นสุด คำถาม", "th");}
  if (Question.indexOf("clear") >= 0) {audio.connecttospeech("ลบ คำถามเก่าออกหมด", "th");Question = "";}
  if (Question.indexOf("config") >= 0) {
    audio.connecttospeech("กำหนดค่า Config", "th");
    if (Question.indexOf("replace") >= 0) {  
      Question.replace(" ","");
      int AT_Word = Question.indexOf("replace")+7;
      String Question_Replace = Question.substring(AT_Word,Question.length()-1);
      Serial.println(Question_Replace);
      Check_Replace_SPIFFS(Question_Replace.c_str());
      Question = "";
    }
  }
}

// void utf8AsciiConvert(char* src, char*des)                 
// {       int k=0;
//        char c;
//        for (int i=0; src[i]; i++){
//             c = utf8Ascii(src[i]);
//             if (c!='\0') //if (c!=0)
//             des[k++]=c;
//        }
//        des[k]='\0';  //des[k]=0;
// }

// char msgDisplay[100] = "olá cão";
// utf8AsciiConvert(msgDisplay,msgDisplay);  //return olá cão