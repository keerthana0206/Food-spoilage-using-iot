#include <LiquidCrystal.h>
#include <DHT.h>  // Including library for dht
 
#include <ESP8266WiFi.h>
LiquidCrystal lcd(D4,D5,D0,D1,D2,D3);

String apiKey = "HK70XTFMFPQXQQOW";     //  Enter your Write API key from ThingSpeak
 
const char *ssid =  "Gravity Gimbals";     // replace with your wifi ssid and wpa2 key
const char *pass =  "8778222387";
const char* server = "api.thingspeak.com";
 
#define DHTPIN D6          //pin where the dht11 is connected
const int gas =A0;
const int red =D7;
const int green =D8; 
DHT dht(DHTPIN, DHT11);

int gasval;

WiFiClient client;
 
void setup() 
{
       lcd.begin(16, 2);
       Serial.begin(115200);
       delay(10);
       dht.begin();
       lcd.print(" IOT BASED FOOD ");
       lcd.setCursor(0,1);
       lcd.print("MONITORING SYSTEM");
       delay(2000);
       lcd.clear();
       pinMode(gas,INPUT);
       pinMode(red,OUTPUT);
       pinMode(green,OUTPUT);
     Serial.println("Connecting to ");
       Serial.println(ssid);
 
 
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
 
}
 
void loop() 
{
  
      //float h = dht.readHumidity();
      float t = dht.readTemperature();
      //Serial.println("TEMP :");
      //Serial.println(t);

      gasval=analogRead(gas);
      
      lcd.clear();
      lcd.print("Temperature:");
      lcd.print(t);
      lcd.setCursor(0,1);
      lcd.print("Gas:");
      lcd.print(gasval);
      
              if (isnan(t)) 
                 {
                     Serial.println("Failed to read from DHT sensor!");
                      return;
                 }
 if(gasval>400)
 {
  digitalWrite(red,HIGH);
  digitalWrite(green,LOW);
  lcd.clear();
  lcd.print(" SPOILED FOOD");
  
 }
 else
 {
  digitalWrite(red,LOW);
  digitalWrite(green,HIGH);

 }
                      if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(t);
                             postStr +="&field2=";
                             postStr += String(gasval);
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
                             Serial.print("Temperature: ");
                             Serial.print(t);
                             Serial.print(" degrees Celcius, Humidity: ");
                             Serial.print(gasval);
                             Serial.println("%. Send to Thingspeak.");
                        }
          client.stop();
 
          Serial.println("Waiting...");
  
  // thingspeak needs minimum 15 sec delay between updates
  delay(1000);
}
