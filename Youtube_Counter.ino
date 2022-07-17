#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include <YoutubeApi.h>   //Setup API Key here https://github.com/witnessmenow/arduino-youtube-api
#include <ArduinoJson.h>
#include <TM1637TinyDisplay.h>
#include <Arduino.h>  

char ssid[] = "YOUR_SSID";       // your network SSID (name)
char password[] = "PASSWORD";  // your network key

#define API_KEY "ENTER_KEY_HERE"  // your google apps API Token
#define CHANNEL_ID "YOUR CHANNEL ID" // makes up the url of channel
//------- ---------------------- ------
String newHostname = "ENTER_HOSTNAME_HERE";
WiFiClientSecure client;
YoutubeApi api(API_KEY, client);

unsigned long timeBetweenRequests = 60000;
unsigned long nextRunTime;

long subs = 0;
#define CLK 5
#define DIO 16
TM1637TinyDisplay display(CLK, DIO);

void setup() {

  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
 WiFi.hostname(newHostname.c_str());
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  // Required if you are using ESP8266 V2.5 or above
  client.setInsecure();

  // If you want to enable some extra debugging
  api._debug = true;
}

void loop() {
display.setBrightness(BRIGHT_7);
  if (millis() > nextRunTime)  {
    if(api.getChannelStatistics(CHANNEL_ID))
    {
      Serial.println("---------Stats---------");
      Serial.print("Subscriber Count: ");
      float subCount=api.channelStats.subscriberCount;
      Serial.println(subCount/1000);
      Serial.print("View Count: ");
      Serial.println(api.channelStats.viewCount);
      Serial.print("Comment Count: ");
      Serial.println(api.channelStats.commentCount);
      Serial.print("Video Count: ");
      Serial.println(api.channelStats.videoCount);
      // Probably not needed :)
      //Serial.print("hiddenSubscriberCount: ");
      Serial.println(api.channelStats.hiddenSubscriberCount);
      Serial.println("------------------------");
      display.showNumber(subCount/1000, 2);

    }
    nextRunTime = millis() + timeBetweenRequests;
  }
}
