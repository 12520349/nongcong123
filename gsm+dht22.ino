
#define role 4
#define ledPin 13
int nguong = 1001;
#include <DHT.h> // Gọi thư viện DHT22

#include "SIM900.h"
#include <SoftwareSerial.h>
#include "sms.h"
#include "call.h"
#define DEBUG_ON false
#define DEBUG_PRINT false
/****************8
 #define _GSM_TXPIN_ 7
#define _GSM_RXPIN_ 8
** * * * * * * * */

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

 
const int DHTPIN = A0; //Đọc dữ liệu từ DHT22 ở chân A3 trên mạch Arduino
const int DHTTYPE = DHT22; //Khai báo loại cảm biến, có 2 loại là DHT11 và DHT22
DHT dht(DHTPIN, DHTTYPE);
 
void setup() {
  // put your setup code here, to run once:
  pinMode(A3, INPUT);
  pinMode(role, OUTPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
dht.begin(); // Khởi động cảm biến
   time = millis();

}

void loop() {

  // put your main code here, to run repeatedly:
   Serial.print("Gia tri cam bien rung: ");

  int value_cambienrung = analogRead(A3);
   Serial.println(value_cambienrung);
   if (value_cambienrung < nguong){
       Serial.print("Phat hien RUNG");
           if ((unsigned long)(millis() - time) > gioi_han_thoi_gian) {
         call.Call(phone_numberA);
         time = millis();
         Serial.println("Thuc hien cuoc goi bao dong!");
         delay(1000);
         }
       digitalWrite(ledPin, HIGH);
       digitalWrite(role, HIGH); // CHAN COM <-> NC
       delay(5000);
        Serial.println("Da bat role");
       
    }else{
   delay(200);
    Serial.print("Da tat role");
    digitalWrite(ledPin, LOW);
    digitalWrite(role, LOW);
    }

    float h = dht.readHumidity(); //Đọc độ ẩm
    float t = dht.readTemperature(); //Đọc nhiệt độ
    Serial.print("Nhiet do: ");
    Serial.println(t); //Xuất nhiệt độ
    if (t >= max_nhietdo){
      sms.SendSMS(phone_numberA, "Qua nhiet: "+ String(t) + " ^C")
      }
      /*
    Serial.print("Do am: ");
    Serial.println(h); //Xuất độ ẩm
 
    Serial.println(); //Xuống hàng
    */
}
