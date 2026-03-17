#include <LiquidCrystal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const int rs = 13, en = 12, d4 =14 , d5 = 27, d6 = 26, d7 = 25;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
const char* ssid = "Robot123";
const char* password = "**********";
int IN1=4;
int IN2=5;
int IN3=18;
int IN4=19;
#include<dht.h>  
#define dht_dpin 15
dht DHT;
int Relay=21;
int Metal=22;
int Buzzer=32;
int Fire=23;
WiFiClient  client;
#define BOT_TOKEN "XXXXXXXXXX" // Telegram Bot Token
#define CHAT_ID "XXXXXXXXXX"  // Telegram Chat ID
WiFiClientSecure secured_client;  // Initialize Telegram BOT
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long lastTime;
unsigned long botRequestDelay = 1000;  // Check for new messages every second
int str_len;
char textmessage[20];
void Connect_wifi();
float tempc,Humidity;
int x=0;
int A_val;
int L_val;
void setup() 
{
  pinMode(Fire,INPUT);
  pinMode(Metal,INPUT);
  pinMode(Relay,OUTPUT);
  pinMode(Buzzer,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
  digitalWrite(Relay,LOW);
  digitalWrite(Buzzer,LOW);
  Serial.begin(9600);
  Serial2.begin(9600);
  lcd.begin(16,2);                      
  lcd.clear();
  lcd.print("Under Mining");
  lcd.setCursor(0,1);
  lcd.print("   Robot...");
  Serial.println("ESP32 ROBOT");
  delay(1000); 
  WiFi.mode(WIFI_STA); 
  Connect_wifi();
  // Send a startup message to Telegram
  bot.sendMessage(CHAT_ID, "Under Mining Robot ...");
  delay(1000); 
}
void Connect_wifi()
{  
  Serial.print("Connecting Wifi: ") ;// Attempt to connect to Wifi network:
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
} 
void loop() 
{
  Sensor_Check();
} 
void Sensor_Check()
{
  while(1)
  {
     serial_check(); 
     Humidity_Check();
     Gas_Check();
     Fire_Check();
     LDR_Check();
     Metal_Check();  
  } 
}
void serial_check()
{
  if(Serial2.available()>0)
     {
        char ch2=Serial2.read();
        Serial.println("Received:"+String(ch2));
        delay(100);
        if(ch2=='F')
        {
          FORWARD();
        }
        if(ch2=='B')
        {
          REVERSE();
        }
         if(ch2=='L')
        {
          LEFT();
        }
         if(ch2=='R')
        {
          RIGHT();
        }
         if(ch2=='S')
        {
          STOP();
        }
      }
}
void Fire_Check()
{
  if(digitalRead(Fire)==LOW)
  {
    lcd.clear();
    lcd.print("Fire Detected..");
    Serial.println("Fire Detected..");
    digitalWrite(Buzzer,HIGH);
    digitalWrite(Relay,HIGH);
    STOP();
    delay(1000);
    digitalWrite(Buzzer,LOW);
    digitalWrite(Relay,LOW);
    bot.sendMessage(CHAT_ID, "Fire Detected..");
    delay(1000);
  }
serial_check();
}
void Metal_Check()
{
  if(digitalRead(Metal)==LOW)
  {
    lcd.clear();
    lcd.print("Metal Detected..");
    Serial.println("Metal Detected..");
    digitalWrite(Buzzer,HIGH);
    STOP();
    delay(1000);
    digitalWrite(Buzzer,LOW);
    bot.sendMessage(CHAT_ID, "Metal Detected..");
    delay(1000);   
  }
serial_check();
}
void Humidity_Check(void)
{
      DHT.read11(dht_dpin);
      Humidity=DHT.humidity;
      tempc=DHT.temperature;
      Serial.print("$Humidity: ");
      Serial.print(DHT.humidity);   // printing Humidity on LCD
      Serial.print(" %");
      Serial.print(" # ");
  
      Serial.print("$Temperature:");
      Serial.print(DHT.temperature);   // Printing temperature on LCD
      Serial.print(" C");
      Serial.println(" # ");
      lcd.clear();
      lcd.print("Humidity:"+String(DHT.humidity));
      lcd.setCursor(0,1);
      lcd.print("Temp:"+String(DHT.temperature));
      delay(1000);
      serial_check();
 }
void Gas_Check()
{
  A_val=analogRead(36);
  Serial.println("CO:"+String(A_val));
  lcd.clear();
  lcd.print("CO:"+String(A_val));
  delay(1000);
  if(A_val>2000)
  {
    lcd.clear();
    lcd.print("CO Detected");
    Serial.println("CO Detected");
    Serial2.println("CO Detected");
    digitalWrite(Buzzer,HIGH);
    delay(1000);
    STOP();
    delay(1000);
    digitalWrite(Buzzer,LOW);
    bot.sendMessage(CHAT_ID, "CO Gas Detected");
    delay(2000); 
   }
   serial_check();
}
 void LDR_Check()
{
  L_val=analogRead(39);
  L_val=4095-L_val;
  Serial.println("LDR:"+String(L_val));
  lcd.clear();
  lcd.print("LDR:"+String(L_val));
  delay(1000);
  if(L_val<1000)
  {
    lcd.clear();
    lcd.print("Darkeness Detected");
    Serial.println("Darkness Detected");
    Serial2.println("Darkness Detected");
    digitalWrite(Relay,HIGH);
    delay(1000);
    bot.sendMessage(CHAT_ID, "Darkness Detected");
    delay(1000); 
   }
   if(L_val>3000)
  {
      digitalWrite(Relay,LOW);
      delay(100);
  }
  serial_check();
}
void FORWARD()
{
  Serial.println("Forward...");
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
} 
void REVERSE()
{
  Serial.println("Reverse...");
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
} 
void LEFT()
{
  Serial.println("Left...");
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
} 
void RIGHT()
{
  Serial.println("Right...");
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
} 
void STOP()
{
  Serial.println("Stop...");
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
}
