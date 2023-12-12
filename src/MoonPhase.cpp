#include <Arduino.h>
#include <Config.h>
#include "Audio.h"
#include <time.h>

//int MoonPhase(int ,int ,int ); 
//................. คำนวนข้างขึ้น ข้างแรม วันที่ปัจจุบัน ............................//
int MoonPhase(int NYear_Time_Now,int NMonth_Time_Now,int NDay_Time_Now) {
  String begin_thai_date = "2565-11-23";
  int NYear_Thai = (begin_thai_date.substring(0, 4)).toInt(); int NMonth_Thai= (begin_thai_date.substring(5, 7)).toInt();int NDay_Thai  = (begin_thai_date.substring(8,10)).toInt();
  int arr_atikawan_y[] = {2500,2506,2513,2516,2522,2530,2533,2540,2543,2549,2552,2559,2563,2568,2575,2578,2586,2589,2595};
  const int DaysInMonth[13] = {0,31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // DaysInMonth[0] = 0 ไม่ใช้
  int Start_DaysInMonth = (begin_thai_date.substring(5,7)).toInt();
  int begin_year = 1957;
  int check_year = 2023; // ปีที่จะหาข้อมูล
  int day_add = 0; // วันที่เพิ่มแต่ละปี
  int i;
  for(i = begin_year; i <= check_year; i++){
    int is_atikamas = (fmod(((i - 78) - 0.45222),2.7118886)<1)?1:0;
    for (int ii = 0;ii < 100;ii++) {if((i+543) == arr_atikawan_y[ii]) {int is_atikawan =  arr_atikawan_y[ii];break;}}
    int day_in_year = 354;int is_atikawan;
    if(is_atikamas==1){day_in_year = 384;}  //?384:day_in_year;
    if(is_atikawan==1){day_in_year = 355;}  //?355:day_in_year;
    
    // วันขึ้น 1 ค่ำ เดือน 1 ของแต่ละปี
    // String begin_buddhist_day_of_year = begin_thai_date.substring(1,7) ; // date("Y-m-d",strtotime(begin_thai_date," +day_add day")); // 
    day_add+= day_in_year; // เพิ่มวันแต่ละปี
    int c_day = 0; // นับวัน
    int c_month = 0; // นับเดือน
    int c_u_day = 0; // นับวันขึ้น
    int c_d_day = 0; // นับวันแรม   
    int current_month = 1; // เดือนเริ่มต้น
    int day_in_month = 0; // วันในแต่ละเดือน
    int month_in_year = 12; // เดือนใน 1 ปี
    bool double_month = false; // เดือน 8 สองหนหรือไม่
    month_in_year = (is_atikamas==1)?13:month_in_year; // มี 13 เดือน
    if(i== check_year){ // วนลูปเฉพาะปีที่จะดู
      for(int v = 0; v < (day_in_year+60); v++){c_day++; // วนลุป วันในปีนั้นๆ +เพิ่มวันเพื่อให้ข้ามปี
        int final_d_day = (current_month%2==1)?14:15;
        final_d_day = (is_atikawan==1 && current_month==7)?15:final_d_day;
        if(c_d_day==final_d_day){ // ถึงวันแรมสุดท้ายของเดือน เริ่มเดือนใหม่
          current_month++;
          // เข้าปีปฏิทินไทยใหม่ นับเดือนใหม่
          if(current_month==13){ current_month = 1;}
          // ปีที่มีแปด 2 หน
          if(is_atikamas && current_month==9 && double_month==false){current_month--; double_month = true;}
          c_u_day = 0; c_d_day = 0;
        }        
        
        NDay_Thai++;  // เพิ่มวันในปฏิทิน
        if(c_u_day<15){c_u_day++;}else{if(c_d_day<final_d_day){c_d_day++;}} //คำนวณนับ วันข้างขึ้น วันข้างแรม

        // เช็ควันปัจจุบัน แล้วออก
        if(NYear_Time_Now == NYear_Thai and NMonth_Time_Now == NMonth_Thai and NDay_Time_Now == NDay_Thai) {
          Serial.print(NYear_Thai);Serial.print("-");Serial.print(NMonth_Thai);Serial.print("-");Serial.print(NDay_Thai);// แสดงวันที่ในปฏิทิน
          if(c_d_day>0){Serial.print(" แรม ");Serial.println(c_d_day);
            CMoonPhase = ("Waning "+String(c_d_day));
          }else{Serial.print(" ขึ้น ");Serial.println(c_u_day);
            CMoonPhase = ("Waxing "+String(c_u_day));
          }// แสดงวันข้างขื้น หรือข้างแรม
          CMoonPhaseThai = CMoonPhase+" ค่ำ"; CMoonPhaseThai.replace("Waxing"," ขึ้น"); CMoonPhaseThai.replace("Waning"," แรม");
          String C_MoonPhaseThai = CMoonPhaseThai;C_MoonPhaseThai.replace(" ","");C_MoonPhaseThai.replace("แรม","");C_MoonPhaseThai.replace("ขึ้น",""); 
          NMoonPhase = C_MoonPhaseThai.toInt();
          break;
        }
        // คำนวนวัน เดือน ปี ที่เพิ่มขึ้น
        if(NDay_Thai == DaysInMonth[NMonth_Thai]){if(NMonth_Thai == 12){NYear_Thai++;NMonth_Thai = 1;}else{NMonth_Thai++;} NDay_Thai = 0;}
      } 
    }
  }
  return NYear_Time_Now,NMonth_Time_Now,NDay_Time_Now;
}