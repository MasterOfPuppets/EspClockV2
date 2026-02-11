#ifndef HELPERS_H
#define HELPERS_H

#include <Arduino.h>
#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <SPI.h>
#include <WiFi.h>
#include <WebServer.h>
#include <EEPROM.h>
#include <time.h>
#include <WiFiManager.h>
#include <HTTPClient.h>

extern MD_Parola P;
extern MD_MAX72XX *mx;
extern WebServer server;
extern struct tm timeinfo;
extern WiFiManager wm;

extern char newMessage[];
extern String message;
extern String alarmDays;
extern String alarmTime;
extern int brightness;
extern int clock_live;
extern const uint8_t BUZZER;
extern const MD_MAX72XX::fontType_t *pFont;
extern String deviceName;
extern boolean alarmOn; 
extern const String page; 
extern String userTimezone;
extern String geoTimezone;

void animDissolve(String oldMsg, String newMsg);
void animCursorReplaceRight(String oldMsg, String newMsg);
void animCursorReplaceLeft(String oldMsg, String newMsg);
void animJumpUp(String oldMsg, String newMsg);
void animJumpDown(String oldMsg, String newMsg);
void animScrollUp(String oldMsg, String newMsg);
void animScrollDown(String oldMsg, String newMsg);
void changeTimeDisplay(String oldMsg, String newMsg);
void waitForIt();

void showDeviceName();
String ipToString();
void setClock();
String getDate(boolean justTime);
void detectTimezone();

void beepOk();
void beepError();
void beepAlarm();
boolean isAlarmTime();

String parsePage();
void dataHandler();
void readEeprom();
String daysToString(String days[7]);
String urlDecode(String str); 
String getPosixRule(String region);

#endif