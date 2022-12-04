#include <WiFi.h>
#include <Keypad.h>
#include <MQTT.h>
#include <TM1637Display.h>
const char ssid[] = "OKBABY";
const char password[] = "12345678A";
String myip;
WiFiClient net;
MQTTClient client(256);

String mqtt_topic = "AIR_KEYBOARD_1";
char airname[]    = "AIR_KEYBOARD_1";
unsigned long lastMillis = 0;



String myString = "0";
String final_str = "";
boolean flag = false;
const int CLK = 22; //Set the CLK pin connection to the display
const int DIO = 23; //Set the DIO pin connection to the display

TM1637Display display = TM1637Display(CLK, DIO);
// Create array that turns all segments on:
const uint8_t data[] = {
  0xff,
  0xff,
  0xff,
  0xff
};


const uint8_t Fail[] = {0x71, 0x77, 0x30, 0x38};
const uint8_t SEUP[] =  {0x6D, 0x79, 0x3E, 0x73};
const uint8_t yEEy[] = {0x6E, 0x79, 0x79, 0x6E};
const uint8_t xong[] = {0x76, 0x3F, 0x37, 0x6F};
// Create array that turns all segments off:
const uint8_t blank[] = {
  0x00,
  0x00,
  0x00,
  0x00
};
// You can set the individual segments per digit to spell words or create other symbols:
const uint8_t done[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G, // d
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F, // O
  SEG_C | SEG_E | SEG_G, // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G // E
};
// Create degree Celsius symbol:
const uint8_t oooo[] = {
  SEG_A | SEG_B | SEG_F | SEG_G, // Circle
  SEG_A | SEG_B | SEG_F | SEG_G,
  SEG_A | SEG_B | SEG_F | SEG_G,
  SEG_A | SEG_B | SEG_F | SEG_G,
};

#define ROW_NUM 4 // four rows
#define COLUMN_NUM 3 // three columns

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
/*
byte pin_rows[ROW_NUM] = {18, 5, 17, 16}; // GIOP18, GIOP5, GIOP17, GIOP16 connect to the row pins
byte pin_column[COLUMN_NUM] = {4, 0, 2};  // GIOP4, GIOP0, GIOP2 connect to the column pins
*/
/*
byte pin_rows[ROW_NUM] = {13,12,14,27}; // GIOP18, GIOP5, GIOP17, GIOP16 connect to the row pins
byte pin_column[COLUMN_NUM] = {26,25,33};  // GIOP4, GIOP0, GIOP2 connect to the column pins

byte pin_rows[ROW_NUM] = {32,33,25,26}; // GIOP18, GIOP5, GIOP17, GIOP16 connect to the row pins
byte pin_column[COLUMN_NUM] = {12,14,27};  // GIOP4, GIOP0, GIOP2 connect to the column pins
*/

byte pin_rows[ROW_NUM] = {19,18, 5, 17}; // GIOP18, GIOP5, GIOP17, GIOP16 connect to the row pins
byte pin_column[COLUMN_NUM] = {16, 4, 0};  //  GIOP4, GIOP0, GIOP2 connect to the column pins

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);


// Include WiFiManager Library
#include <WiFiManager.h>

void setup() {

  // Setup Serial Monitor
  Serial.begin(115200);
  display.setBrightness(0x0a); //set the diplay to maximum brightness
 display.setSegments(Fail);
  // Create WiFiManager object
  WiFiManager wfm;
  // Supress Debug information
  //wfm.setDebugOutput(false);

  // Remove any previous network settings
  //wfm.resetSettings();
//SEUP
  // Define a text box, 50 characters maximum
  WiFiManagerParameter custom_text_box("my_text", "Enter your string here", "default string", 50);

  // Add custom parameter
  wfm.addParameter(&custom_text_box);
   bool res;
  res = wfm.autoConnect(airname,password); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        display.setSegments(Fail);
          ESP.restart();
        delay(1000);
    } 
    else {
        //if you get here you have connected to the WiFi    
        display.setSegments(yEEy);
        Serial.println("connected...yeey :)");
    }
 

  // Connected!
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  

  // Print custom text box value to serial monitor
  Serial.print("Custom text box entry: ");
  Serial.println(custom_text_box.getValue());
  client.begin("18.138.40.49", net);
  client.onMessage(messageReceived);
  display.setSegments(oooo);
  connect();
  push_mqtt("hello from VN");
}

void loop() {
  /*
     Serial.println();
  Serial.printf("pass: %s ",WiFi.SSID().c_str() );
    Serial.println();
  Serial.printf("pass: %s ", WiFi.psk().c_str() );
*/

  if (!client.connected()) {
            Serial.println("WIFI IN LOOP BI NGAT");
  display.setSegments(data);

    connect();
  }
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  char key = keypad.getKey();

  if (key) {
    Serial.println(key);
    if (key == '#') {
       int num_max = myString.toInt();

      final_str = String(myString.toInt());
      _trypushmqtt(final_str);
      Serial.println("myString: ");
      Serial.print(final_str);
      //myString = "0";
      
      flag = true;
    } else if (key == '*') {
      flag = false;
      myString = "0";
    } else {
      int num_max = myString.toInt();
      if (num_max<=999){
      String key2str = String(key);
      myString += key2str;
      }else{
       display.clear();
       delay(500);
       display.showNumberDec(num_max); //Display the numCounter value;

      }
    }
    if (flag == false) {
      int number = key - 48;
      display.showNumberDec(myString.toInt()); //Display the numCounter value;
      delay(100);
    } else {
      display.clear();
      delay(500);
      /*
      display.showNumberDec(final_str.toInt()); //Display the numCounter value;
      delay(1000);
      display.clear();
   
      delay(500);
         */
      display.setSegments(xong);
      delay(500);
    }
  }
}




void _trypushmqtt(String content) {

  if (!client.connected()) {
    Serial.println("_trypushmqtt => WIFI BI NGAT");
    _reconnect(content);
  } else {
    push_mqtt(content);
  }
}

void _reconnect(String content) {
  Serial.print("_reconnect checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect(airname)) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");
  push_mqtt(content);
  client.subscribe(airname);
}

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect(airname)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nconnected!");

  client.subscribe(airname);
}
void messageReceived(String & topic, String & payload) {
  Serial.println("incoming: " + topic + " - " + payload);

}

void push_mqtt(String content) {
  client.publish(airname, content);

}
