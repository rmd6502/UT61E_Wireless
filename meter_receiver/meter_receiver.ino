#include <esp_now.h>
#include <WiFi.h>
#include <U8g2lib.h>
#include <Wire.h>

#include "Parser.h"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 16, /* clock=*/ 15, /* data=*/ 4);   // ESP32 Thing, HW I2C with pin remapping


#define CHANNEL 0

// Init ESP Now with fallback
void InitESPNow() {
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
    Serial.println(F("ESPNow Init Success"));
  }
  else {
    Serial.println(F("ESPNow Init Failed"));
    // Retry InitESPNow, add a counte and then restart?
    // InitESPNow();
    // or Simply Restart
    ESP.restart();
  }
}

// config AP SSID
void configDeviceAP() {
  const char *SSID = "Slave_1";
  bool result = WiFi.softAP(SSID, "Slave_1_Password", CHANNEL, 0);
  if (!result) {
    Serial.println(F("AP Config failed."));
  } else {
    Serial.println("AP Config Success. Broadcasting with AP: " + String(SSID));
  }
}

void u8g2_prepare(void) {
  u8g2.setFont(u8g2_font_helvR10_tr);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("ESPNow UT61e receiver"));
  //Set device in AP mode to begin with
  WiFi.mode(WIFI_AP);
  // configure device AP mode
  configDeviceAP();
  // This is the mac address of the Slave in AP Mode
  Serial.print(F("AP MAC: ")); Serial.println(WiFi.softAPmacAddress());
  // Init ESPNow with a fallback logic
  InitESPNow();
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info.
  esp_now_register_recv_cb(OnDataRecv);

  u8g2.begin();
  u8g2_prepare();
  u8g2.clearBuffer();
  u8g2.drawStr(0, 0, "Waiting for data");
  u8g2.sendBuffer();
}

// callback when data is recv from Master
String lastBuf;
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println();
  Serial.print("Last Packet Recv from: "); Serial.println(macStr);
  char buf[32];
  memcpy(buf, data, data_len);
  buf[data_len] = 0;
  lastBuf = buf;
}

void loop() {
  if (lastBuf.length() > 0) {
    String result = Parser::parse(lastBuf);
    lastBuf = "";
    u8g2_prepare();
    u8g2.clearBuffer();
    int sep = result.indexOf(' ');
    String meas = result.substring(0,sep);
    String status = result.substring(sep+1);
    Serial.println(status);
    Serial.println(meas);
    int xPos = (u8g2.getDisplayWidth() - u8g2.getStrWidth(status.c_str()))/2;
    if (xPos < 0) { xPos = 0; }
    u8g2.setCursor(xPos,4);
    u8g2.print(status);
    u8g2.setFont(u8g2_font_fub20_tr);
    xPos = (u8g2.getDisplayWidth() - u8g2.getStrWidth(meas.c_str()))/2;
    if (xPos < 0) { xPos = 0; }
    u8g2.setCursor(xPos,28);
    u8g2.print(meas);
    u8g2.sendBuffer();
  }
}
