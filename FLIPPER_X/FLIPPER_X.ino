#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_HMC5883_U.h>

#include "I2Cdev.h" 
#include "RTClib.h"
#include "LOGO.h"
#include "WIFI.h"
#include "MPU6050.h" 
#include "COMMON.h"
#include "PITCHES.h"
#include "GAME_T-REX.h"
#include "TOOL_ORIENTATION.h"

void setup() {
  Wire.begin(SDA, SCL);
  Wire.setClock(400000);

  Serial.begin(115200);
  while (!Serial)
    delay(10); 
    
  setup_rtc();
  setup_display();
  setup_mpu();
  setup_hmc();
  setup_button();

  display_logo();
}

void loop() {
  navigation_on_change(navigation_check());
  if(current_menu_show_idx == menu_get_idx("menu") || current_menu_show_idx == menu_get_idx("back")) 
    display_menu();
  else if(current_menu_show_idx == menu_get_idx("watch")) 
    display_watch();
  else if(current_menu_show_idx == menu_get_idx("t-rex game")) 
    display_game_trex();
  else if(current_menu_show_idx == menu_get_idx("compass"))
    display_compas();
  else if(current_menu_show_idx == menu_get_idx("gyroscope"))
    display_gyroscope();
  else if(current_menu_show_idx == menu_get_idx("wifi scan"))
    display_wifi_scan();
}
