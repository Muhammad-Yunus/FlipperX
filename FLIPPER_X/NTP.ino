void ntp_time_setup(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);

  wifi_connect();

  timeClient.begin();
}

void ntp_update_time(){
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime);
  curr_day = ptm->tm_mday;
  curr_month = ptm->tm_mon+1;
  curr_year = ptm->tm_year+1900;
  curr_hour = timeClient.getHours();
  curr_minute = timeClient.getMinutes();
}
  
