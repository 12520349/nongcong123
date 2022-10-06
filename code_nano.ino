
#define role 4
#define ledPin 13
int nguong = 500;
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
int so_lan_rung = 0;
unsigned long gioi_han_thoi_gian = 15000; //Khoang thoi gian giua 2 sms la: 15s
int max_nhietdo = 32;

int btn = 11;
const int DHTPIN = A0; //Đọc dữ liệu từ DHT22 ở chân A3 trên mạch Arduino
const int DHTTYPE = DHT22; //Khai báo loại cảm biến, có 2 loại là DHT11 và DHT22
DHT dht(DHTPIN, DHTTYPE);
 
void setup() {
  // put your setup code here, to run once:
  pinMode(A5, INPUT);
  pinMode(role, OUTPUT);
  pinMode(ledPin, OUTPUT);
    pinMode(btn, INPUT_PULLUP);

  Serial.begin(9600);
  dht.begin(); // Khởi động cảm biến
   time = millis();
   if (gsm.begin(2400)){     
        Serial.println("\nstatus=READY");     
        started=true;     
    }   else 
        Serial.println("\nstatus=IDLE");  
    
    if(started){   
         Serial.print("gsm khoi dong xong! ");
  
      //  sms.SendSMS("+84387521116", "Online");   //đổi lại số của bạn nhé :D
    } 
}

void loop() {
  int value_cambienmua = analogRead(A5);// analogRead(A5);
    Serial.print("Gia tri cam bien mua: ");
      Serial.println(value_cambienmua);
  // put your main code here, to run repeatedly:
   Serial.print("Gia tri cam bien rung: ");

  int value_cambienrung = digitalRead(btn);// analogRead(A5);
   Serial.println(value_cambienrung);
   if (value_cambienrung == LOW ){
       Serial.print("Phat hien RUNG");
so_lan_rung++;
   }else{
    so_lan_rung = 0;
    }
   if (so_lan_rung >= 2){
     // Serial.println( (unsigned long)(millis() - time));
      if ((unsigned long)(millis() - time) > gioi_han_thoi_gian) {
        so_lan_rung = 0;
         Serial.println("Thuc hien cuoc goi bao dong!");
       call.Call(phone_numberA);
         time = millis();
          delay(1000);
        
       
        digitalWrite(ledPin, HIGH);
        digitalWrite(role, HIGH); // CHAN COM <-> NC
        delay(5000);
        Serial.println("Da bat role");
        }
    }else{
   delay(100);
    //Serial.println("Da tat role");
    digitalWrite(ledPin, LOW);
    digitalWrite(role, LOW);
    }

    float h = dht.readHumidity(); //Đọc độ ẩm
    float t = dht.readTemperature(); //Đọc nhiệt độ
    Serial.print("Nhiet do hien tai: ");
    Serial.println(t); //Xuất nhiệt độ
    if (t >= max_nhietdo){
      String stringOne = "Qua nhiet: "+ String(t) + " ^C";
        char noidung_sms[100];
        stringOne.toCharArray(noidung_sms, 100);
      sms.SendSMS(phone_numberA, noidung_sms);
       Serial.print("Da gui sms voi noi dung: ");
            Serial.println(stringOne);
      }
      /*
    Serial.print("Do am: ");
    Serial.println(h); //Xuất độ ẩm
 
    Serial.println(); //Xuống hàng
    */
}
