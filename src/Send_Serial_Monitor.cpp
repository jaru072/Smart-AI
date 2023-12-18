#include <Arduino.h>
#include <Config.h>

String Question = "";
String SetConfig_Question = "";

void Set_Config(String Cupdate) {
    if (Question.indexOf(Cupdate) >= 0) {  
      Question.replace(" ","");
      int AT_Word = Question.indexOf(Cupdate)+Cupdate.length();  
      SetConfig_Question = Question.substring(AT_Word,Question.length()-1);
      Serial.println(SetConfig_Question);
      String Question_Speech = AI_Word[1][1] +" Set "+Cupdate+" "+SetConfig_Question;
      Serial.println(Question_Speech);
      audio.connecttospeech(Question_Speech.c_str(), "th");
      Question = "";
    }
}

void Send_SerialMonitor(){ 
  Serial.print("Ask your Question : ");
  while (!Serial.available()){audio.loop();}
  while (Serial.available()){char add = Serial.read();Question = Question + add;delay(1);}

  int len = Question.length();
  Question = Question.substring(0, (len));Question = "\"" + Question + "\"";Serial.println(Question);
  if (Question.indexOf("end") >= 0) {LSend_Serial = false;audio.connecttospeech("สิ้นสุด คำถาม", "th");}
  if (Question.indexOf("clear") >= 0) {audio.connecttospeech("ลบ คำถามเก่าออกหมด", "th");Question = "";}
  if (Question.indexOf("config") >= 0) {
    if (Question.indexOf("replace") >= 0) {Set_Config("replace");Check_Replace_SPIFFS(SetConfig_Question.c_str());} 
    if (Question.indexOf("delete") >= 0) {Set_Config("delete");Check_Delete_SPIFFS(SetConfig_Question.c_str());}
    if (Question.indexOf("default") >= 0) {Set_Config("default");Start_Config();}
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