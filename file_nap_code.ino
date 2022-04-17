#include "SIM900.h"
#include <SoftwareSerial.h>
#include "sms.h"
#include "call.h"
#define DEBUG_ON false
#define DEBUG_PRINT false


byte stat = 0;
CallGSM call;
SMSGSM sms;
int numdata;
boolean started = false;
char smsbuffer[160];
char n[20];
unsigned long time;
char sodienthoai[13] = "+84915269373";
char noidung[160] = "Co Trom!";
char phone_numberA[] = "0915269373";

unsigned long gioi_han_thoi_gian = 20000; //Khoang thoi gian giua 2 sms la: 15s
 void setup() {
   //Serial connection.
   time = millis();
   Serial.begin(9600);
   Serial.println("GSM Shield testing.");
   if (gsm.begin(2400)) {
     Serial.println("\nstatus=READY");
     started = true;
   } else Serial.println("\nstatus=IDLE");

   if (started) {
     //Enable this two lines if you want to send an SMS.
     //if (sms.SendSMS("+84387521116", "Khoi dong OK!"))
     //  Serial.println("\nSMS sent OK");
   }

 };

 void loop() {
  stat = call.CallStatusWithAuth(sodienthoai, 1, 5); //kiểm tra trạng thái xem có cuộc gọi nào đến không
  //Serial.println(stat);
    //if(stat == CALL_INCOM_VOICE_AUTH) {
     if(stat == 2) {
       delay(500);
       call.HangUp(); //gác máy khỏi tốn hehehe!
       delay(200);
        
    }
   int value = analogRead(A0); //A0 là chân analogA0 á
   //nếu mà cảm biến rung đangrung :3 thì giá trị cảm biến nhảy ở khoảng 1022 - 1023 :) 
   if (value < 1021) {
     Serial.println("Khong Rung:");
     //Serial.println(value); 
   } else {
     Serial.println("Rung");
     if ((unsigned long)(millis() - time) > gioi_han_thoi_gian) {
       //if (sms.SendSMS(sodienthoai, noidung)) {
         call.Call(phone_numberA);
         time = millis();
         Serial.println("Thuc hien cuoc goi bao dong!");
         delay(1000);
         }
    }
 };
