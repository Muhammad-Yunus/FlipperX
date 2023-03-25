void display_menu(){
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);

  menu_item(2, 2, "watch", true);
  
  menu_item(2, 12, "compass", true);
  
  menu_item(2, 22, "gyroscope", true);
 
  menu_item(2, 32, "wifi scan", true);
  
  menu_item(65, 2, "t-rex game", true);
    
  display_navigation(true, true, true);
    
  display.display();
}

void display_navigation(boolean show_back, boolean show_menu, boolean show_line){
    display.setTextSize(1);
    if (show_line) 
      display.drawLine(0, 50, 127, 50, WHITE);

    if(current_menu_idx == menu_get_idx("menu"))
        display.fillTriangle(103-4,54+3, 103-1,54, 103-1,54+7, WHITE);
    else if(current_menu_idx == menu_get_idx("back"))
        display.fillTriangle(25+4,54+3, 25+1,54, 25+1,54+7, WHITE);
    
    menu_item(2, 54, "back", show_back);
    menu_item(103, 54, "menu", show_menu);
}

void menu_item(int x, int y, char* menu_name, boolean show){
  if(show){
    if(current_menu_idx == menu_get_idx(menu_name)){
      if(menu_name != "menu")
        display.drawRect(x-1, y, 1, 8, WHITE);
      menu_animate(false);
    }
    else {
      menu_animate(true);
    }

    display.setCursor(x,y);
    display.print(menu_name);
  }
}

int menu_get_idx(char* menu_name){
  int idx;
  for (int x = 0; x < sizeof(menus) / sizeof(menus[0]); x++){
    if(strcmp(menus[x], menu_name) == 0){
      idx = x;
    }
  }
  return idx;
}

void menu_set_next(){
  current_menu_idx = (current_menu_idx + 1) % (sizeof(menus) / sizeof(menus[0]));
  play_buzzer("next");
}

void menu_set_prev(){
  current_menu_idx = ((current_menu_idx - 1) > -1 ? (current_menu_idx - 1) : (sizeof(menus) / sizeof(menus[0])));
  play_buzzer("prev");
}

void menu_animate(boolean tick){
  tick ? display.setTextColor(WHITE) : display.setTextColor(BLACK, WHITE);
}

void navigation_on_change(boolean state_change){
  if(state_change && current_menu_show_idx == menu_get_idx("menu")){ // only navigate on the menu page
    if(current_navigation == CHAR_LEFT){
      menu_set_next();  
    }
  }
}

boolean navigation_check(){
    int nav_id = navigation_detect();
    boolean state_change = (nav_id != current_navigation && nav_id != 0);
    if(state_change) {
      current_navigation = nav_id;
    }
    return state_change;
}

int navigation_detect(){
   if(mpu.getMotionInterruptStatus()) {
      sensors_event_t a, g, temp;
      mpu.getEvent(&a, &g, &temp);
      AcX = a.acceleration.x;
      AcY = a.acceleration.y; 
      GyX = g.gyro.x;
      GyY = g.gyro.y;
      boolean above_thresh_X = abs(AcX - LAcX) > DETECTION_THRESHOLD;
      boolean above_thresh_Y = abs(AcY - LAcY) > DETECTION_THRESHOLD;
      boolean sign_X = (AcX - LAcX) > 0;
      boolean sign_Y = (AcY - LAcY) > 0;
      LAcX = AcX;
      LAcY = AcY; 
      LGyX = GyX;
      LGyY = GyY;
      
      if(above_thresh_X){
        return (sign_X > 0 ? CHAR_RIGHT : CHAR_LEFT);
      }
      else if(above_thresh_Y){
        return (sign_Y > 0 ? CHAR_UP : CHAR_DOWN);
      }
      else {
        return 0;
      }
   }
   else {
     return 0;
   }
}

boolean check_refresh_state(){
  unsigned long current_millis = millis();
  if((current_millis - previous_millis) >= REFRESH_DURATION){
    previous_millis = current_millis;
    refresh_state = !refresh_state;
  }
  return refresh_state;
}

void setup_mpu(){
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("mpu connection successful");
  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(DETECTION_THRESHOLD);
  mpu.setMotionDetectionDuration(DETECTION_DURATION);
  mpu.setInterruptPinLatch(true);  
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);

  mpu_o.initialize();
  // XAOffset, YAOffset, ZAOffset, XGOffset, YGOffset, ZGOffset  
  mpuOffsetConfig(-2166, -759, 1453, 27, -25, -40);
}

void setup_hmc(){
  mag.begin();
  last_dx = centreX;
  last_dy = centreY;
}

void setup_display(){
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.display();
}

void setup_rtc(){
  rtc.begin();
}

void setup_button(){
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  button_attach_interrupt();
}

void button_attach_interrupt(){
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), button_handler, RISING);
}

void button_detach_interrupt(){
  detachInterrupt(digitalPinToInterrupt(BUTTON_PIN));
}

void play_buzzer(String kind){

  if(kind == "next")
    tone(BUZZER_PIN, NOTE_C4, 125);
  else if(kind == "prev")
    tone(BUZZER_PIN, NOTE_G3, 125);
  else if(kind == "ok")
    tone(BUZZER_PIN, NOTE_B3, 125);
}

ICACHE_RAM_ATTR  void button_handler(){

  unsigned long current_button_milis = millis();
  if((current_button_milis - prev_button_milis) >= 500) { // avoid double click
    prev_button_milis = current_button_milis;
    int prev_menu_show_idx = current_menu_show_idx; 
    
    current_menu_show_idx = current_menu_idx; // change page

    //////////////////////////////////////////////////////////////////////
    if(current_menu_show_idx == menu_get_idx("menu"))
      current_menu_idx = prev_menu_show_idx; // set last selected menu as default
    else 
      current_menu_idx = menu_get_idx("menu"); // set menu button as default


    //////////////////////////////////////////////////////////////////////
    if(current_menu_show_idx == menu_get_idx("t-rex game")){
      button_detach_interrupt();// detach interrupt while playing t-rex game
    }
    else if(current_menu_show_idx == menu_get_idx("gyroscope")){
      display_gyroscope_init();
    }
    else if(current_menu_show_idx == menu_get_idx("wifi scan")){
      wifi_scanned = false;
    }
    
    //////////////////////////////////////////////////////////////////////    
    if(prev_menu_show_idx == menu_get_idx("watch") && ntp_started){
      wifi_set_mode("off");
      ntp_started = false;
    }
      
    play_buzzer("ok");
  }
}
