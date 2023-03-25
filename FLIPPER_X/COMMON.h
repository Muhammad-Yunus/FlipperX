#define BUZZER_PIN D7
#define BUTTON_PIN D5

#define REFRESH_DURATION 500 //in milisecond

#define DETECTION_THRESHOLD 4
#define DETECTION_DURATION 20  //in milisecond

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

#define CHAR_UP     30
#define CHAR_DOWN   31
#define CHAR_RIGHT  16
#define CHAR_LEFT   17


int button_last_state = HIGH;
int button_current_state;
unsigned long prev_button_milis = 0;
  
unsigned long previous_millis = 0;
boolean refresh_state = true; 
int current_navigation = 0;

char menus[7][15] = {"watch", "compass", "gyroscope", "wifi scan", "t-rex game", "back", "menu"};
int current_menu_idx = 6;
int current_menu_show_idx = 6;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

RTC_DS1307 rtc;

Adafruit_MPU6050 mpu;
int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;
int16_t LAcX, LAcY,LGyX, LGyY;

MPU6050 mpu_o;

Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
int last_dx, last_dy, dx, dy, angle;
const int centreX = 65;
const int centreY = 30;
const int radius  = 30; 
