/**
@title T2312_PicoConsol.ino
@git  	https://github.com/infrapale/T2312_PicoConsol

https://circuitdigest.com/microcontroller-projects/arduino-freertos-tutorial1-creating-freertos-task-to-blink-led-in-arduino-uno
https://circuitdigest.com/microcontroller-projects/arduino-freertos-tutorial-using-semaphore-and-mutex-in-freertos-with-arduino



 */

//  This sketch uses the GLCD (font 1) and fonts 2, 4, 6, 7, 8
 
 
// Pause in milliseconds between screens, change to 0 to time font rendering
#define WAIT 1000
#define PIRPANA
//#define LILLA_ASTRID
//#define VILLA_ASTRID

#define PIN_WIRE_SDA         (12u)
#define PIN_WIRE_SCL         (13u)

#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>
#include "main.h"
#include "menu.h"
#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "secrets.h"
#include <Wire.h>
#include "RTClib.h"
#include "time.h"
#include "aio_mqtt.h"
#include "log.h"
#include "dashboard.h"

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

// WiFi parameters
#define WLAN_SSID       WIFI_SSID
#define WLAN_PASS       WIFI_PASS
WiFiClient client;


module_data_st  me = {'X','1'};

// RTC_PCF8563 rtc;



unsigned long     targetTime = 0; // Used for testing draw times
SemaphoreHandle_t sema_v; 
SemaphoreHandle_t mutex_v;

void setup(void) {
  pinMode(TFT_BL, OUTPUT);  
  digitalWrite(TFT_BL, HIGH);
  delay(3000);
  while (!Serial);
  Serial.begin(115200); // For debug
  Serial.println(F(APP_NAME));
  Serial.print(F("Compiled: "));Serial.print(__DATE__);
  Serial.println(F(" "));Serial.println(__TIME__);


  Wire.setSDA(PIN_WIRE_SDA);
  Wire.setSCL(PIN_WIRE_SCL);
  Wire.begin();
  time_begin();

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  Serial.println(F("setup - wifi begin .. "));
  while (WiFi.status() != WL_CONNECTED) 
  {
      delay( 100 );
      Serial.print(F("."));
      Serial.flush();
      //Watchdog.reset();
  }
  //Watchdog.reset();


  mutex_v = xSemaphoreCreateMutex(); 
  if (mutex_v == NULL) 
  { 
    Serial.println("Mutex can not be created"); 
  } 
  
  xTaskCreate(aio_mqtt_stm,"AIO_MQTT",4*4096,nullptr,1,nullptr);
  xTaskCreate(dashboard_update_task,"Dashboard",1024,nullptr,1,nullptr);
  sema_v = xSemaphoreCreateBinary();

  tft.init();
  tft.setRotation(3);
  tft.setTextSize(1);
  tft.fillScreen(TFT_BLACK);

  menu_draw();

  dashboard_draw_box(0);  // clear dashboard
  log_initialize();
  // log_fill_test_data(30);   log_fill_test_data(70);
}

void setup1()
{
  menu_initialize();
  targetTime = millis() + 10;

}

void loop() {
  char c = menu_read_button();
  if (c != 0x00) 
  {
      if ((c & 0b10000000) == 0) 
          Serial.printf("On");
      else 
          Serial.printf("Off");
      Serial.printf(" %c\n",c & 0b01111111);
      c &=  0b01111111;
      menu_btn_pressed(c);
  }
  delay(100);
}

void loop1()
{
  if (millis() > targetTime)
  {
    targetTime = millis() + 10;
    menu_button_scan();
  }
}