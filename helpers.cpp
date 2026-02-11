#include "helpers.h"
#include "mainpage.h" 

String userTimezone = "UTC0"; 
String geoTimezone = "";      

unsigned char h2int(char c) {
  if (c >= '0' && c <= '9') { return ((unsigned char)c - '0'); }
  if (c >= 'a' && c <= 'f') { return ((unsigned char)c - 'a' + 10); }
  if (c >= 'A' && c <= 'F') { return ((unsigned char)c - 'A' + 10); }
  return (0);
}

String urlDecode(String str) {
  String encodedString = "";
  char c;
  char code0;
  char code1;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (c == '+') {
      encodedString += ' ';
    } else if (c == '%') {
      i++;
      code0 = str.charAt(i);
      i++;
      code1 = str.charAt(i);
      c = (h2int(code0) << 4) | h2int(code1);
      encodedString += c;
    } else {
      encodedString += c;
    }
  }
  return encodedString;
}

String getPosixRule(String region) {
  if (region == "Europe/Lisbon") return "WET0WEST,M3.5.0/1,M10.5.0";
  if (region == "Europe/London") return "GMT0BST,M3.5.0/1,M10.5.0";
  if (region == "Europe/Madrid") return "CET-1CEST,M3.5.0,M10.5.0/3";
  if (region == "Europe/Paris") return "CET-1CEST,M3.5.0,M10.5.0/3";
  if (region == "Europe/Berlin") return "CET-1CEST,M3.5.0,M10.5.0/3";
  if (region == "Europe/Rome") return "CET-1CEST,M3.5.0,M10.5.0/3";
  if (region == "Europe/Brussels") return "CET-1CEST,M3.5.0,M10.5.0/3";
  if (region == "Europe/Amsterdam") return "CET-1CEST,M3.5.0,M10.5.0/3";
  if (region == "Europe/Athens") return "EET-2EEST,M3.5.0/3,M10.5.0/4";
  if (region == "Europe/Moscow") return "MSK-3";
  if (region == "America/New_York") return "EST5EDT,M3.2.0,M11.1.0";
  if (region == "America/Chicago") return "CST6CDT,M3.2.0,M11.1.0";
  if (region == "America/Sao_Paulo") return "<-03>3";
  if (region == "Asia/Dubai") return "<+04>-4";
  if (region == "Asia/Tokyo") return "JST-9";
  if (region == "Asia/Shanghai") return "CST-8";
  
  return "UTC0";
}


void animDissolve(String oldMsg, String newMsg) {
  newMsg.toCharArray(newMessage, newMsg.length() + 1);
  P.displayText(newMessage, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
  P.displayAnimate();
  int width = mx->getColumnCount();
  byte newBitmap[width];
  for(int i=0; i<width; i++) { newBitmap[i] = mx->getColumn(i); }
  oldMsg.toCharArray(newMessage, oldMsg.length() + 1);
  P.displayText(newMessage, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
  P.displayAnimate();
  int totalSteps = width * 8 * 3;
  mx->control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
  for (int i=0; i<totalSteps; i++) {
      int c = random(width);
      int r = random(8);
      bool targetState = (newBitmap[c] >> r) & 0x01;    
      mx->setPoint(r, c, targetState);
      if (i % 5 == 0) {
        mx->control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
        delay(10);
        mx->control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
      }
  }
  for(int i=0; i<width; i++) { mx->setColumn(i, newBitmap[i]); }
  mx->control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON); 
  delay(clock_live);
}

void animCursorReplaceRight(String oldMsg, String newMsg) {
  newMsg.toCharArray(newMessage, newMsg.length() + 1);
  P.displayText(newMessage, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
  P.displayAnimate(); 
  int width = mx->getColumnCount();
  byte newBitmap[width];
  for(int i=0; i<width; i++) { newBitmap[i] = mx->getColumn(i); }
  oldMsg.toCharArray(newMessage, oldMsg.length() + 1);
  P.displayText(newMessage, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
  P.displayAnimate();
  mx->control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
  for (int i=0; i<width; i++) {
      mx->setColumn(i, newBitmap[i]);
      if (i < width - 1) { mx->setColumn(i+1, 0xFF); }
      mx->control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
      delay(50); 
      mx->control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
  }
  mx->setColumn(width-1, newBitmap[width-1]);
  mx->control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
  delay(clock_live);
}

void animCursorReplaceLeft(String oldMsg, String newMsg) {
  newMsg.toCharArray(newMessage, newMsg.length() + 1);
  P.displayText(newMessage, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
  P.displayAnimate();
  int width = mx->getColumnCount();
  byte newBitmap[width];
  for(int i=0; i<width; i++) { newBitmap[i] = mx->getColumn(i); }
  oldMsg.toCharArray(newMessage, oldMsg.length() + 1);
  P.displayText(newMessage, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
  P.displayAnimate();
  mx->control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
  for (int i=width-1; i>=0; i--) {
      mx->setColumn(i, newBitmap[i]);
      if (i > 0) { mx->setColumn(i-1, 0xFF); }
      mx->control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
      delay(50);
      mx->control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
  }
  mx->setColumn(0, newBitmap[0]);
  mx->control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
  delay(clock_live);
}

void animJumpUp(String oldMsg, String newMsg) {
  oldMsg.toCharArray(newMessage, oldMsg.length() + 1);
  P.displayText(newMessage, PA_CENTER, 150, clock_live, PA_PRINT, PA_SCROLL_UP);
  waitForIt();
  newMsg.toCharArray(newMessage, newMsg.length() + 1);
  P.displayText(newMessage, PA_CENTER, 150, clock_live, PA_SCROLL_DOWN, PA_NO_EFFECT);
  waitForIt();
}

void animJumpDown(String oldMsg, String newMsg) {
  oldMsg.toCharArray(newMessage, oldMsg.length() + 1);
  P.displayText(newMessage, PA_CENTER, 150, clock_live, PA_PRINT, PA_SCROLL_DOWN);
  waitForIt();
  newMsg.toCharArray(newMessage, newMsg.length() + 1);
  P.displayText(newMessage, PA_CENTER, 150, clock_live, PA_SCROLL_UP, PA_NO_EFFECT);
  waitForIt();
}

void animScrollUp(String oldMsg, String newMsg) {
  newMsg.toCharArray(newMessage, newMsg.length() + 1);
  P.displayText(newMessage, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
  P.displayAnimate();
  int width = mx->getColumnCount();
  byte newBitmap[width];
  for(int i=0; i<width; i++) newBitmap[i] = mx->getColumn(i);
  oldMsg.toCharArray(newMessage, oldMsg.length() + 1);
  P.displayText(newMessage, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
  P.displayAnimate();
  byte oldBitmap[width];
  for(int i=0; i<width; i++) oldBitmap[i] = mx->getColumn(i);
  mx->control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
  for (int step = 0; step <= 8; step++) {
    for (int i = 0; i < width; i++) {
      byte col = (oldBitmap[i] >> step) | (newBitmap[i] << (8 - step));
      mx->setColumn(i, col);
    }
    mx->control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
    delay(50);
    mx->control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
  }
  for(int i=0; i<width; i++) mx->setColumn(i, newBitmap[i]);
  mx->control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
  delay(clock_live);
}

void animScrollDown(String oldMsg, String newMsg) {
  newMsg.toCharArray(newMessage, newMsg.length() + 1);
  P.displayText(newMessage, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
  P.displayAnimate();
  int width = mx->getColumnCount();
  byte newBitmap[width];
  for(int i=0; i<width; i++) newBitmap[i] = mx->getColumn(i);
  oldMsg.toCharArray(newMessage, oldMsg.length() + 1);
  P.displayText(newMessage, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
  P.displayAnimate();
  byte oldBitmap[width];
  for(int i=0; i<width; i++) oldBitmap[i] = mx->getColumn(i);
  mx->control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
  for (int step = 0; step <= 8; step++) {
    for (int i = 0; i < width; i++) {
      byte col = (oldBitmap[i] << step) | (newBitmap[i] >> (8 - step));
      mx->setColumn(i, col);
    }
    mx->control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
    delay(50); 
    mx->control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
  }
  for(int i=0; i<width; i++) mx->setColumn(i, newBitmap[i]);
  mx->control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
  delay(clock_live);
}

void changeTimeDisplay(String oldMsg, String newMsg) {
  int pick = random(0, 7);
  switch (pick) {
    case 0: animJumpUp(oldMsg, newMsg); break;
    case 1: animJumpDown(oldMsg, newMsg); break;
    case 2: animScrollUp(oldMsg, newMsg); break;
    case 3: animScrollDown(oldMsg, newMsg); break;
    case 4: animCursorReplaceRight(oldMsg, newMsg); break;
    case 5: animCursorReplaceLeft(oldMsg, newMsg); break;
    case 6: animDissolve(oldMsg, newMsg); break;
    default: animJumpUp(oldMsg, newMsg); break;
  }
}

void waitForIt() {
  while (!P.displayAnimate()) { }
}


void showDeviceName() {
  alarmOn = false; 
  beepOk();
  P.displayClear();
  P.setFont(nullptr);
  String m = "http://" + deviceName + ".local";
  m.toCharArray(newMessage, m.length() + 1);
  P.displayText(newMessage, PA_CENTER, 60, 2000 , PA_SCROLL_LEFT, PA_SCROLL_LEFT);    
  waitForIt();  
  P.setFont(pFont); 
}

String ipToString(){
  IPAddress ip = WiFi.localIP();
  return String(ip[0]) + String(".") + String(ip[1]) + String(".") + String(ip[2]) + String(".") + String(ip[3]); 
}

void beepOk(){
  ledcWriteTone(BUZZER, 440); delay(20); ledcWriteTone(BUZZER, 0); delay(200);  
  ledcWriteTone(BUZZER, 440); delay(20); ledcWriteTone(BUZZER, 0); 
}

void beepError(){
  ledcWriteTone(BUZZER, 880); delay(100); ledcWriteTone(BUZZER, 0); delay(100);
  ledcWriteTone(BUZZER, 880); delay(100); ledcWriteTone(BUZZER, 0); delay(100);
  ledcWriteTone(BUZZER, 880); delay(100); ledcWriteTone(BUZZER, 0);
}

void beepAlarm(){
  ledcWriteTone(BUZZER, 880); delay(400); ledcWriteTone(BUZZER, 1661); delay(400);
  ledcWriteTone(BUZZER, 2489); delay(400); ledcWriteTone(BUZZER, 3135); delay(1000);
  ledcWriteTone(BUZZER, 0); 
}

boolean isAlarmTime(){
  int h = alarmTime.substring(0,2).toInt();
  int m = alarmTime.substring(3,5).toInt();
  return m == timeinfo.tm_min && h == timeinfo.tm_hour && alarmDays[timeinfo.tm_wday] == '1';
}

String daysToString(String days[7]) {
  String ret = "0000000";
  for (int i = 6; i >= 0; i--) {
        ret[i] = days[i] == "on" ? '1' : '0';
  }
  return ret;
}

void detectTimezone() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://ip-api.com/json/?fields=timezone"); 
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      int firstQuote = payload.indexOf(':');
      if (firstQuote > 0) {
        String temp = payload.substring(firstQuote + 1); 
        temp.replace("\"", ""); temp.replace("}", "");  
        temp.replace("\n", ""); temp.trim();
        geoTimezone = temp;
        Serial.print("GeoIP Timezone Detected: "); Serial.println(geoTimezone);
      }
    }
    http.end();
  }
}

void setClock() {
  configTime(0, 0, "europe.pool.ntp.org", "pool.ntp.org", "time.nist.gov");
  detectTimezone();

  if ((userTimezone == "" || userTimezone == "UTC0") && geoTimezone != "") {
      String suggestedRule = getPosixRule(geoTimezone);
      if (suggestedRule != "UTC0") {
         userTimezone = suggestedRule;
      }
  }
  
  Serial.print("Applying Timezone Rule: "); Serial.println(userTimezone);
  setenv("TZ", userTimezone.c_str(), 1);
  tzset();

  Serial.print(F("Waiting for NTP time sync: "));
  time_t nowSecs = time(nullptr);
  int retry = 0;
  while (nowSecs < 8 * 3600 * 2 && retry < 20) { 
    delay(500); Serial.print(F(".")); yield();
    nowSecs = time(nullptr); retry++;
  }
  Serial.println(F("\nDone! "));
  getLocalTime(&timeinfo);
}

void readEeprom() {
  alarmTime =  EEPROM.readString(0).substring(0,5); 
  alarmDays =  EEPROM.readString(5).substring(0,7); 
  brightness = EEPROM.readInt(12);
  String storedTz = EEPROM.readString(20);
  if (storedTz.length() > 1 && storedTz.length() < 50) { userTimezone = storedTz; } 
  else { userTimezone = "UTC0"; }
  
  Serial.print("Stored alarm: "); Serial.println(alarmTime);
  Serial.print("Stored brightness: "); Serial.println(brightness);  
  Serial.print("Stored timezone: "); Serial.println(userTimezone);
}

// --- WEB SERVER ---
String parsePage() {
  String ret = page; 
  ret.replace("%ALARM%", alarmTime);
  ret.replace("%D1%",alarmDays[0] == '1' ? "checked" : "");
  ret.replace("%D2%",alarmDays[1] == '1' ? "checked" : "");
  ret.replace("%D3%",alarmDays[2] == '1' ? "checked" : "");  
  ret.replace("%D4%",alarmDays[3] == '1' ? "checked" : "");
  ret.replace("%D5%",alarmDays[4] == '1' ? "checked" : "");
  ret.replace("%D6%",alarmDays[5] == '1' ? "checked" : "");
  ret.replace("%D7%",alarmDays[6] == '1' ? "checked" : "");
  ret.replace("%BRIGHTNESS%", String(brightness));
  ret.replace("%CURRENT_TZ%", userTimezone);
  
  ret.replace("%MESSAGE%", ""); 

  String s = "";
  if (alarmOn) {
    s = " <div><input type='hidden' name='stop_alarm' value = '1' /><button class='button' type='submit' id='stopAlarm'>Stop alarm</button></div>\r\n ";    
  }
  ret.replace("%ALARM_SWITCH%", s);  
  
  return ret;
}

void dataHandler(){  
  String alarmStop = server.arg("stop_alarm");
  if (alarmStop == "1"){
    alarmOn = false;
    beepOk();
    server.sendHeader("Location","/");  
    server.send(303);    
    return;
  }
  
  String msg = server.arg("alarmTime");   
  String days[7] = {
    server.arg("d1"), server.arg("d2"), server.arg("d3"),
    server.arg("d4"), server.arg("d5"), server.arg("d6"), server.arg("d7")
  };
  String bright = server.arg("brightness");
  
  String newTzRaw = server.arg("timezone"); 
  String newTz = urlDecode(newTzRaw);       
  
  if (newTz.length() > 0) {
      userTimezone = newTz;
      setenv("TZ", userTimezone.c_str(), 1);
      tzset();
      getLocalTime(&timeinfo);
  }
  String matrixMsg = ""; 
  String expectedRule = getPosixRule(geoTimezone);
  if (geoTimezone.length() > 0 && userTimezone != expectedRule) {
     matrixMsg = "TZONE!!";
     beepError();
  } else {
     matrixMsg = "    OK    ";
     beepOk();
  }
  

  EEPROM.writeString(0, msg);  
  EEPROM.writeString(5, daysToString(days));
  EEPROM.writeInt(12, bright.toInt());
  EEPROM.writeString(20, userTimezone);
  EEPROM.commit();
  
  matrixMsg.toCharArray(newMessage, matrixMsg.length() + 1);
  P.displayClear();
  P.setFont(nullptr);
  P.displayText(newMessage, PA_CENTER, 50, 1000, PA_PRINT, PA_FADE);
  waitForIt();
  P.setFont(pFont);
  readEeprom();
  P.setIntensity(map(brightness, 1, 9, 0, 15));  
  
  server.sendHeader("Location","/");  
  server.send(303);                   
}

String getDate(boolean justTime) { 
  getLocalTime(&timeinfo);
  char day[3];
  char weekDay[10];
  char month[10];
  char year[5];
  char hour[3];
  char minute[3];
  strftime(weekDay, 10, "%A", &timeinfo);
  strftime(month, 10, "%m", &timeinfo);
  strftime(day, 3, "%d", &timeinfo);
  strftime(hour, 3, "%H", &timeinfo);
  strftime(minute, 3, "%M", &timeinfo);
  strftime(year, 5, "%Y", &timeinfo);
  char test [30];  
  char time_val [5];  
  sprintf(test, "%s, %s-%s-%s",weekDay, day, month, year);
  String line = test;
  sprintf(time_val, "%s:%s", hour, minute);
  if (justTime) {    
    return time_val;
  }  
  return line;
}