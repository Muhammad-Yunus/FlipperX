void display_wifi_scan(){
  
  if(!wifi_scanned){
    display.clearDisplay();
    display.setTextColor(WHITE);
    wifi_set_header("Wifi Scanner", 1);
    display.setCursor(0, 12);
    display.print("scanning...");
    display.display();
    
    wifi_set_mode("sta");  
    
    wifi_scan();
    wifi_set_mode("off");

    wifi_scanned = true;
  }
  
  display_navigation(false, true, false);
  
  display.display(); 
}

void wifi_scan(){
  scanResult = WiFi.scanNetworks(/*async=*/false, /*hidden=*/true);

  display.clearDisplay();
  wifi_set_header("Wifi Scanner", 1);
  display.setCursor(1, 12);
  
  if (scanResult == 0) {
    display.print("No networks found");
  } else if (scanResult > 0) {
    display.setTextColor(BLACK, WHITE);
    display.print("  ");
    display.print(scanResult);
    display.print(" networks found");
    display.print("   ");
    
    display.setTextColor(WHITE);
    int default_wifi_y = 22;
    for (int8_t i = 0; i < scanResult; i++) {
      WiFi.getNetworkInfo(i, ssid, encryptionType, rssi, bssid, channel, hidden);
      
      sprintf (rssi_str, "%4d", rssi);
      
      display.setCursor(0, default_wifi_y + (i*10));
      display.print(i + 1);
      display.print(" ");
      display.print(ssid);
      wifi_draw_signal(rssi, 115, default_wifi_y + (i*10));
    }
  } else {
    display.print("WiFi scan error ");
    display.print(scanResult);
  }
}

void wifi_set_mode(String kind){
  if(kind == "sta"){
     WiFi.mode(WIFI_STA);
     WiFi.disconnect(); 
  }
  else if (kind == "ap"){
    WiFi.mode(WIFI_AP);
    WiFi.begin("Flipper-X", "yunusggg");
  }
  else if (kind == "off"){
    WiFi.mode(WIFI_OFF);
  }
}

void wifi_set_header(String label, int text_size){
  display.setTextSize(text_size);
  display.setCursor(28, 0);
  display.print(label);
  display.drawLine(0, 10, 127, 10, WHITE);
}

void wifi_draw_signal(int32_t rssi, int x, int y){
  int quality = 0;
  if(rssi <= -100)
    quality = 0;
  else if(rssi >= -50)
    quality = 100;
  else 
    quality = 2 * (rssi + 100); // rssi in dBm

  quality  = quality / 25;
  for (int i = 0; i < quality; i++){
    display.drawRect(x + (i*3), y + (3 - i)*2, 2, (i+1)*2, WHITE);
  }
}

void wifi_connect(){
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(34, 20);
  display.print("connecting");
  display.drawRect(39, 30, 50, 8, WHITE);
  display.display();
  
  int retry = 1;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    display.fillRect(39, 30, retry * 5, 8, WHITE);
    display.display();
    if (retry == 10)
      retry = 1;
    else 
      retry++;
  }

  display.clearDisplay();
  display.setCursor(24, 20);
  display.print("connected to ntp");
}
