/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
//#define BLYNK_TEMPLATE_ID           "TMPLYvcNR1TB"
//#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_TEMPLATE_ID "TMPLXe8rEHxw"
#define BLYNK_DEVICE_NAME "LEDCONTROL"
#define BLYNK_AUTH_TOKEN "DyGDVaSKYcwqPCN29U7N09YU66CPWAxM"
// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
char blynk_token[34] = "DyGDVaSKYcwqPCN29U7N09YU66CPWAxM";

#include <FS.h>    
#include "SPIFFS.h"


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>


#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson


 #include <LiquidCrystal_I2C.h>

int totalColumns = 16;
int totalRows = 2;

LiquidCrystal_I2C lcd(0x27, totalColumns, totalRows);



#include "DHTesp.h"

#define DHTpin 15    //D15 of ESP32 DevKit

DHTesp dht;
const int analogPin = 36;  // Analog input pin 0 (GPIO 36)

//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "OKBABE";
char pass[] = "12345678A";

BlynkTimer timer;
//int led = 4;
int congtac1 = 4;
int congtac2 = 16;
int congtac3 = 17;
int congtac4 = 5;
// This function is called every time the Virtual Pin 0 state changes
/*
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();

  // Update state
  Blynk.virtualWrite(V1, value);
}
*/
BLYNK_WRITE(V1)
{
  int trangthai = param.asInt();
  digitalWrite(congtac1, trangthai);
  lcd.clear(); 
lcd.setCursor(0, 0);
  if (trangthai == 1){
lcd.print("Relay 1 => ON");
}else{
  lcd.print("Relay 1 => OFF");

  }

}
BLYNK_WRITE(V2)
{
  int trangthai = param.asInt();
  digitalWrite(congtac2, trangthai);
lcd.clear(); 
lcd.setCursor(0, 0);
if (trangthai == 1){
lcd.print("Relay 0 => ON");
}else{
  lcd.print("Relay 0 => OFF");

  }
}
BLYNK_WRITE(V3)
{
  int trangthai = param.asInt();
  digitalWrite(congtac3, trangthai);
lcd.clear(); 
lcd.setCursor(0, 0);
if (trangthai == 1){
lcd.print("Relay 3 => ON");
}else{
  lcd.print("Relay 3 => OFF");

  }
}

BLYNK_WRITE(V4)
{
  int trangthai = param.asInt();
  digitalWrite(congtac4, trangthai);
lcd.clear(); 
lcd.setCursor(0, 0);
if (trangthai == 1){
lcd.print("Relay 2 => ON");
}else{
  lcd.print("Relay 2 => OFF");

  }
}
// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
}
void sendSensor()
{
   
   //delay(dht.getMinimumSamplingPeriod());

  float h = dht.getHumidity();
  float t = dht.getTemperature();


  /*
  float h = dht.readHumidity();
// Read temperature as Celsius (the default)
float t = dht.readTemperature();
*/
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
    }  else {
    lcd.clear(); 
    lcd.setCursor(10,0);
    lcd.print(round(t));
    lcd.print(" ");
    lcd.write(1);
    lcd.print("C");

    lcd.setCursor(10,1);
    lcd.print(round(h));
    lcd.print(" %"); 
    delay(200);   
  }

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(h, 1);
  Serial.print("\t\t");
  Serial.print(t, 1);
  Serial.print("\t\t");

  
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);


}
void setup()
{
    pinMode(congtac1, OUTPUT);
        pinMode(congtac2, OUTPUT);
   pinMode(congtac3, OUTPUT);
        pinMode(congtac4, OUTPUT);
    lcd.init(); 
lcd.backlight(); // use to turn on and turn off LCD back light
  // Debug console
  Serial.begin(115200);
   dht.setup(DHTpin, DHTesp::DHT22); //for DHT22 Connect DHT sensor to GPIO 17


//   dht.setup(DHTpin, DHTesp::DHT22); //for DHT22 Connect DHT sensor to GPIO 17


  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
  timer.setInterval(1000L, AnalogPinRead);  // Run sensor scan 4 times a second

  //clean FS, for testing
  //SPIFFS.format();

  //read configuration from FS json
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");

          //strcpy(mqtt_server, json["mqtt_server"]);
         //strcpy(mqtt_port, json["mqtt_port"]);
          strcpy(blynk_token, json["blynk_token"]);

        } else {
          Serial.println("failed to load json config");
        }
        configFile.close();
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read



  // The extra parameters to be configured (can be either global or just in the setup)
  // After connecting, parameter.getValue() will get you the configured value
  // id/name placeholder/prompt default length
  //WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
  //WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port, 6);
  WiFiManagerParameter custom_blynk_token("blynk", "blynk token", blynk_token, 34);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  //set static ip
  //wifiManager.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
  
  //add all your parameters here
  //wifiManager.addParameter(&custom_mqtt_server);
  //wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_blynk_token);

  //reset settings - for testing
  //wifiManager.resetSettings();

  //set minimu quality of signal so it ignores AP's under that quality
  //defaults to 8%
  //wifiManager.setMinimumSignalQuality();
  
  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  //wifiManager.setTimeout(120);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("AutoConnectAP", "password")) {
    Serial.println("failed to connect and hit timeout");
        lcd.clear(); 
lcd.setCursor(0, 0);
lcd.print("failed to connect");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    //ESP.reset();
       ESP.restart();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  //read updated parameters
  //strcpy(mqtt_server, custom_mqtt_server.getValue());
  //strcpy(mqtt_port, custom_mqtt_port.getValue());
  strcpy(blynk_token, custom_blynk_token.getValue());

  //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("saving config");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    //json["mqtt_server"] = mqtt_server;
   // json["mqtt_port"] = mqtt_port;
    json["blynk_token"] = blynk_token;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
    //end save
  }
Serial.print("SSID: ");
Serial.println(WiFi.SSID());
Serial.print("psk: ");
Serial.println(WiFi.psk());
 while (WiFi.status() != WL_CONNECTED) {
       int mytimeout = 5;
       delay(500);
       Serial.print(".");
       if((millis() / 1000) > mytimeout ){ // try for less than 6 seconds to connect to WiFi router
         Serial.println(" ");
          break;
        }
   }

      if(WiFi.status() == WL_CONNECTED){  
    //if you get here you have connected to the WiFi
   Serial.println(" ");
   Serial.println("connected to WiFi!! yay :)");
   Serial.println("IP address: ");
   Serial.println(WiFi.localIP()); 
 lcd.clear(); 
lcd.setCursor(0, 0);
lcd.print(WiFi.localIP());
delay(1000);
 
  Blynk.config(blynk_token);
  bool result = Blynk.connect();

  if (result != true)
  {
  Serial.println("BLYNK Connection Fail");
  Serial.println(blynk_token);
  wifiManager.resetSettings();
     ESP.restart();
  //ESP.reset();
  //delay (5000);
  }
  else
  {
  Serial.println("BLYNK Connected!! Yay again");
  }
 }
 else if (WiFi.status() != WL_CONNECTED){
  Serial.println("WiFi Connection Fail");
 } 

 
  //Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  //timer.setInterval(1000L, myTimerEvent);
}
void AnalogPinRead() {
int  sensorValue = analogRead(analogPin);  // Read the analog in value:
  Serial.print("sensor = ");  // Print the results...
  Serial.println(sensorValue);  // ...to the serial monitor:
  Blynk.virtualWrite(V7, sensorValue);  // Send the results to Gauge Widget
  if (  sensorValue > 2001 ){
    Blynk.notify("Yaaay... FIRE FIRE FIRE!!!!");
  }

}


void loop()
{
  Blynk.run();
  timer.run();
  /*
    delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.print(dht.toFahrenheit(temperature), 1);
  Serial.print("\t\t");
  Serial.print(dht.computeHeatIndex(temperature, humidity, false), 1);
  Serial.print("\t\t");
  Serial.println(dht.computeHeatIndex(dht.toFahrenheit(temperature), humidity, true), 1);
*/
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}
