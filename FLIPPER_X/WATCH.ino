void display_watch(){
  if(rtc_available()){
    DateTime now = rtc.now();
    curr_day = now.day();
    curr_month = now.month();
    curr_year = now.year();
    curr_hour = now.hour();
    curr_minute = now.minute();
    wifi_set_mode("off");
    ntp_started = false;
  }
  else {
    if(!ntp_started) {
      ntp_time_setup();
      ntp_started = true;
    }
    ntp_update_time();
  }
  
  draw_watch();
}

void draw_watch(){
  if (curr_year > 2022) {
    boolean tick = check_refresh_state();
    display.clearDisplay();
    display.setTextColor(WHITE);
  
    // time part    
    display.setCursor(20,8);
    display.setTextSize(3);
    display_print_w_pad(curr_hour);
    if(tick == true)
      display.write(':');
    else
      display.write(' ');
    display_print_w_pad(curr_minute);
  
    // date part
    display.setCursor(35,35);
    display.setTextSize(1);
    display_print_w_pad(curr_day);
    display.write('.');
    display_print_w_pad(curr_month);
    display.write('.');
    display.print(curr_year);
    
    display_navigation(false, true, true);
    
    display.display();
    delay(500);  
  }
}

void display_print_w_pad(int number) {
    if (number >= 0 && number < 10) {
      display.write('0');
    }
    display.print(number);
}

boolean rtc_available(){
  Wire.beginTransmission (104); // 0x98 I2C address for RTC
  return Wire.endTransmission () == 0;
}
