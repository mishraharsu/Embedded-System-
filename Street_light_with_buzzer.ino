#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <TimeLib.h>

// Replace these with your WiFi credentials
const char* ssid = "SSID";
const char* password = "PASSWORD";

const int led = 2;
const int buzzer = 5;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800); 

unsigned long previousMillis = 0; 
const long interval = 1000;

const int onHour = 4;
const int onMinute = 32;
const int offHour = 4;
const int offMinute = 35;

bool ledState = false;
bool buzzerBeep = false;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Initialize the NTP client
  timeClient.begin();

  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);

  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
}

void loop() {
  unsigned long currentMillis = millis(); // Get the current time
  
 
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; 
    
  // Update the time from the NTP server
  timeClient.update();

  // Get the current Unix timestamp (epoch time)
  unsigned long currentEpochTime = timeClient.getEpochTime();


  int currentHour = (hour(currentEpochTime) % 12 == 0) ? 12 : (hour(currentEpochTime) % 12);
  int currentMinute = minute(currentEpochTime);
  int currentSecond = second(currentEpochTime);
  String meridian = hour(currentEpochTime) < 12 ? "AM" : "PM";


  Serial.print("Current Time (India): ");
  Serial.print(currentHour);
  Serial.print(":");
  if (currentMinute < 10) {
    Serial.print("0"); 
  }
  Serial.print(currentMinute);
  Serial.print(":");
   if (currentSecond < 10) {
      Serial.print("0"); 
  }
    Serial.print(currentSecond);
    Serial.print(" ");
    Serial.println(meridian);

    if(currentHour == onHour && currentMinute == onMinute && !ledState && !buzzerBeep){
      digitalWrite(led, LOW);
      Serial.println("on");
      digitalWrite(buzzer, HIGH);
      delay(100);
      digitalWrite(buzzer, LOW);
      buzzerBeep = true;
      ledState = true;
    }
    if(currentHour == offHour && currentMinute == offMinute){
      digitalWrite(led, HIGH);   
    }
      if(currentHour == offHour && currentMinute == offMinute && ledState){
      Serial.println("Off");
      digitalWrite(buzzer, HIGH);
      delay(100);
      digitalWrite(buzzer, LOW);
      ledState = false;

  }  
}
}
