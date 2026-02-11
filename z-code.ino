#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <SPI.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <WebServer.h>
#include <EEPROM.h>
#include <ESPmDNS.h>
#include <time.h>
#include "Parola_Fonts_data.h"
#include "helpers.h" 

#define USE_UI_CONTROL 0

#if USE_UI_CONTROL
#include <MD_UISwitch.h>
#endif

#define DEBUG 0 

#if DEBUG
  #define DB_BEGIN(x)     Serial.begin(x)
  #define DB_PRINT(...)   Serial.print(__VA_ARGS__)
  #define DB_PRINTLN(...) Serial.println(__VA_ARGS__)
#else
  #define DB_BEGIN(x)
  #define DB_PRINT(...)
  #define DB_PRINTLN(...)
#endif


#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   4
#define DATA_PIN  6
#define CS_PIN    7

#define PUSHBUTTON 3
const uint8_t BUZZER = 2; 

MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

const uint8_t SPEED_IN = A5;
const uint8_t SPEED_DEADBAND = 5;

int clock_live = 900;
uint8_t scrollSpeed = 50; 
textEffect_t scrollEffect = PA_SCROLL_LEFT;
textPosition_t scrollAlign = PA_LEFT;
uint16_t scrollPause = 0; 
const MD_MAX72XX::fontType_t *pFont = myNumbers;

WiFiManager wm;
WebServer server(80);
struct tm timeinfo;

#define BUF_SIZE 95
char newMessage[BUF_SIZE] = { "" };
String message = "";
String alarmDays = "0000000";
String alarmTime = "00:00";
int brightness = 5;

String deviceName = "default"; 

MD_MAX72XX *mx;
boolean alarmOn = false; 
int minute = -1;
String lmsg = "";

void setup()
{
  DB_BEGIN(115200);

  WiFi.mode(WIFI_STA); 
  String mac = WiFi.macAddress();
  mac.replace(":", ""); 
  String suffix = mac.substring(8); 
  deviceName = "clock-" + suffix;
  DB_PRINT("Device Name: ");
  DB_PRINTLN(deviceName);
  
  pinMode(PUSHBUTTON, INPUT_PULLUP); 

  if (digitalRead(PUSHBUTTON) == LOW) {
    DB_PRINTLN("Resetting WiFi Settings...");
    wm.resetSettings();
    ledcAttach(BUZZER, 1000, 8);
    ledcWriteTone(BUZZER, 2000);
    delay(100);
    ledcWriteTone(BUZZER, 0);
  }

  if (!ledcAttach(BUZZER, 1000, 8)) {
    DB_PRINTLN("Buzzer init failed");
  }

  beepOk(); 
  
    if (!EEPROM.begin(128)) {
    DB_PRINTLN("Failed to initialise EEPROM");
    ESP.restart();
  }
  readEeprom(); 
  
  P.begin();
  mx = P.getGraphicObject();
  
  P.setIntensity(map(brightness, 1, 9, 0, 15));
  P.setFont(nullptr);

  wm.setConfigPortalBlocking(false);
  
  if(wm.autoConnect(deviceName.c_str())) {
      DB_PRINTLN("Connected immediately!");
  } else {
      DB_PRINTLN("Config Portal started...");
  }

  String msgSetup = "Open WiFi '" + deviceName + "' to setup";
  msgSetup.toCharArray(newMessage, msgSetup.length() + 1);
  
  P.displayText(newMessage, PA_CENTER, 50, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);

  while (WiFi.status() != WL_CONNECTED) {
      wm.process();       
      P.displayAnimate(); 
      if (P.getZoneStatus(0)) {
        P.displayReset();
      }
  }
  P.setFont(pFont);
  DB_PRINTLN("WiFi Connected!");

  if (MDNS.begin(deviceName.c_str())) {
    DB_PRINTLN("MDNS responder started");
  }
   
  server.on("/", []() {
   server.send(200, "text/html", parsePage());
  });
  server.on("/setAlarm", HTTP_POST, dataHandler); 
  server.begin();  
  
  DB_PRINTLN("Reporting wifi values");
  DB_PRINTLN(WiFi.SSID());
  DB_PRINTLN(ipToString()); 
  
  showDeviceName();
  setClock(); 
  
  beepOk();
}

void loop()
{
  if(digitalRead(PUSHBUTTON) == LOW) {
    alarmOn = false; 
    showDeviceName();
  }
  
  String msg = ""; 
  msg = getDate(true);
  if (alarmDays[timeinfo.tm_wday] == '1') {    
    msg += " #";
  }
  msg.toCharArray(newMessage, msg.length() + 1);
  
  if (minute != timeinfo.tm_min) {
    alarmOn = isAlarmTime();
    minute = timeinfo.tm_min;
    
    changeTimeDisplay(lmsg, msg);
    
  } else {
    P.displayText(newMessage, PA_CENTER, 150, clock_live, PA_PRINT, PA_NO_EFFECT);
    waitForIt();
    lmsg = msg;
  }  
  server.handleClient();

  while (alarmOn) {
      if(digitalRead(PUSHBUTTON) == LOW) {
        alarmOn = false;
        beepOk();
      }      
      
      msg = getDate(true);
      msg.toCharArray(newMessage, msg.length() + 1);  
      P.displayText(newMessage, PA_CENTER, 40, clock_live / 2 , PA_SCROLL_UP, PA_FADE);
      waitForIt();
      beepAlarm();
      server.handleClient();
      lmsg = msg;
  }
}