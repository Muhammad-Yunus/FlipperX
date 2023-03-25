#define STASSID "X Modem New"
#define STAPSK  "yunusggg"


// WIFI NTP
const long utcOffsetInSeconds = 3600*7;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
boolean ntp_started = false;
int curr_year, curr_month, curr_day, curr_hour, curr_minute;

// WIFI SCANNER
boolean wifi_scanned = false;

String ssid;
int32_t rssi;
uint8_t encryptionType;
uint8_t* bssid;
int32_t channel;
bool hidden;
int scanResult;
char rssi_str[7];
