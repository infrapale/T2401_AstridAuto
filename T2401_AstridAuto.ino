/**
@title T2312_PicoConsol.ino
@git  	https://github.com/infrapale/T2312_PicoConsol

https://circuitdigest.com/microcontroller-projects/arduino-freertos-tutorial1-creating-freertos-task-to-blink-led-in-arduino-uno
https://circuitdigest.com/microcontroller-projects/arduino-freertos-tutorial-using-semaphore-and-mutex-in-freertos-with-arduino

 
This sketch uses the GLCD (font 1) and fonts 2, 4, 6, 7, 8 

  #########################################################################
  ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
  #########################################################################
*/
#define WAIT 1000
#define PIRPANA
//#define LILLA_ASTRID
//#define VILLA_ASTRID

#define PIN_SERIAL1_TX      (0u)
#define PIN_SERIAL1_RX      (1u)
#define PIN_WIRE_SDA        (12u)
#define PIN_WIRE_SCL        (13u)

#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include <WiFi.h>
#include <Wire.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "secrets.h"
#include "aio_mqtt.h"
#include "time.h"
#include <TaHa.h> 
#include "RTClib.h"
#include "time.h"
//#include "aio_mqtt.h"
#include "menu.h"
// #include "log.h"
#include "dashboard.h"


#include "RTClib.h"

// Adafruit IO
#define WLAN_SSID           WIFI_SSID
#define WLAN_PASS           WIFI_PASS

#define AIO_SERVER          "io.adafruit.com"
#define AIO_SERVERPORT      1883
#define AIO_USERNAME        IO_USERNAME
#define AIO_KEY             IO_KEY
#define AIO_PUBLISH_INTERVAL_ms     60000
#define AIO_MIN_SUBSCRIBE_IVAL_ms   10000

#define TFT_GREY        0x5AEB
#define LOOP_PERIOD     35 // Display updates every 35 ms
#define SCAN_BUTTON_MS  10


typedef struct 
{
  int8_t  connected;
  uint8_t state;
  uint16_t conn_faults;
  uint8_t at_home;
  uint32_t    next_run;
} aio_mqtt_ctrl_st;


aio_mqtt_ctrl_st aio_mqtt_ctrl =
{
  .connected = false,
  .state = 0,
  .conn_faults = 0,
  .at_home = 0,
  .next_run = 0
};

// Core 0 Data
module_data_st  me = {'X','1'};

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

WiFiClient client;

Adafruit_MQTT_Client aio_mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish villa_astrid_home_mode    = Adafruit_MQTT_Publish(&aio_mqtt, AIO_USERNAME "/feeds/villaastrid.astrid-mode");
Adafruit_MQTT_Subscribe villa_astrid_od_temp    = Adafruit_MQTT_Subscribe(&aio_mqtt, AIO_USERNAME "/feeds/villaastrid.ulko-temp");
Adafruit_MQTT_Subscribe villa_astrid_od_hum     = Adafruit_MQTT_Subscribe(&aio_mqtt, AIO_USERNAME "/feeds/villaastrid.ulko-hum");
Adafruit_MQTT_Subscribe villa_astrid_tupa_temp  = Adafruit_MQTT_Subscribe(&aio_mqtt, AIO_USERNAME "/feeds/villaastrid.tupa-temp");
//Adafruit_MQTT_Subscribe time_iso_8601  = Adafruit_MQTT_Subscribe(&aio_mqtt, AIO_USERNAME "/feeds/time/ISO-8601");
Adafruit_MQTT_Subscribe time_iso_8601  = Adafruit_MQTT_Subscribe(&aio_mqtt, "time/ISO-8601");


aio_subs_st io_subs[AIO_SUBS_NBR_OF] =
{
  [AIO_SUBS_TIME_ISO_8601]  = {&time_iso_8601,            "Date Time", UNIT_TEMPERATURE, "", false, 0, 2, 0},
  [AIO_SUBS_VA_OD_TEMP]     = {&villa_astrid_od_temp,     "VA OD Temp", UNIT_TEMPERATURE,"", false, 0, 4, 0},
  [AIO_SUBS_VA_OD_HUM]      = {&villa_astrid_od_hum,      "VA OD Hum", UNIT_TEMPERATURE, "", false, 0, 1, 0},
  [AIO_SUBS_VA_TUPA_TEMP]   = {&villa_astrid_tupa_temp,   "VA Tupa Temp", UNIT_TEMPERATURE,"", false, 0, 4, 0},
};


Adafruit_MQTT_Publish *aio_publ[AIO_PUBL_NBR_OF] =
{
  [AIO_PUBL_VA_HOME_MODE] = &villa_astrid_home_mode,
  [AIO_PUBL_VA_AC_TEMP]  = &villa_astrid_home_mode
};

Adafruit_MQTT_Subscribe *aio_subscription;

// Task Handler Declarations
TaHa TaHa_aio;
TaHa TaHa_dashboard;
TaHa TaHa_read_button;

// Core 1 Data
uint32_t key_scan_time = 0;  //

void setup(void) {
  pinMode(TFT_BL, OUTPUT);  
  digitalWrite(TFT_BL, HIGH);

  // Serial1.setTX(PIN_SERIAL1_TX);
  // Serial1.setRX(PIN_SERIAL1_RX);

  tft.init();
  tft.setRotation(3);
  tft.setTextSize(1);

  delay(3000);
  Serial.begin(9600); // For debug
  // Serial1.begin(9600); // For something new

  Serial.println("TFT Meters");
  Serial.println(F(APP_NAME));
  Serial.print(F("Compiled: "));Serial.print(__DATE__);
  Serial.println(F(" "));Serial.println(__TIME__);

  Wire.setSDA(PIN_WIRE_SDA);
  Wire.setSCL(PIN_WIRE_SCL);
  Wire.begin();

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  Serial.println(F("setup - wifi begin .. "));
  while (WiFi.status() != WL_CONNECTED) 
  {
      delay( 100 );
      Serial.print(F("."));
      Serial.flush();
      //Watchdog.reset();
  }
  
  TaHa_aio.set_interval(100, RUN_RECURRING, aio_mqtt_stm);
  TaHa_aio.set_interval(100, RUN_RECURRING, dashboard_update_task);
  TaHa_read_button.set_interval(100, RUN_RECURRING, read_button_task);

  time_begin();
  // dashboard_draw_box(0);
  tft.fillScreen(TFT_BLACK);
  menu_draw();
}

void setup1()
{
  menu_initialize();
  key_scan_time = millis() + 10;
}

void loop() 
{
  TaHa_aio.run();
  TaHa_dashboard.run();
  TaHa_read_button.run();
}

void loop1()
{   
  if (millis() > key_scan_time)
  {
    key_scan_time = millis() + 10;
    menu_button_scan();
  }
}

void read_button_task(void)
{
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
}

int8_t aio_mqtt_connect() {
    Serial.print(F("Connecting to Adafruit IO… "));
    int8_t ret;
    if ((ret = aio_mqtt.connect()) != 0) 
    {
        switch (ret) 
        {
            case 1: Serial.println(F("Wrong protocol")); break;
            case 2: Serial.println(F("ID rejected")); break;
            case 3: Serial.println(F("Server unavail")); break;
            case 4: Serial.println(F("Bad user/pass")); break;
            case 5: Serial.println(F("Not authed")); break;
            case 6: Serial.println(F("Failed to subscribe")); break;
            default: Serial.println(F("Connection failed")); break;
        }
        Serial.println(aio_mqtt.connectErrorString(ret));
        if(ret >= 0) aio_mqtt.disconnect();
        Serial.println(F("Retrying connection…"));
        aio_mqtt_ctrl.conn_faults++;
        // Watchdog.reset();
    }
    else 
    {
      Serial.println(F("Adafruit IO Connected!"));
    }
    
    return ret;
}


void aio_mqtt_stm(void)
{
    static bool initial_run = true;
    static uint32_t next_run = 0;
    String time_str; 
    String value_str;
    float  value;
    uint32_t unix_time;

    if (initial_run)
    {
      Serial.println(F("aio_mqtt_stm - initial run"));
      aio_mqtt_ctrl.next_run = millis() + 1000;
      aio_mqtt_ctrl.state = 0;

    }
    else if (millis() > aio_mqtt_ctrl.next_run)
    {
      aio_mqtt_ctrl.next_run = millis() + 5000;
      unix_time = 0;  //time_get_epoc_time();
      // Serial.print(F("aio_mqtt_stm - while "));
      // Serial.println(state);
      switch(aio_mqtt_ctrl.state)
      {
        case 0:
          Serial.println(F("Initializing AIO MQTT"));
          // Serial.println(F("\nWiFi connected"));
          // Serial.println(F("IP address: "));
          // Serial.println(WiFi.localIP());
          Serial.println(F("Subscribe to feeds: "));
          for ( uint8_t findx = AIO_SUBS_TIME_ISO_8601; findx < AIO_SUBS_NBR_OF; findx++)
          {
              Serial.println(io_subs[findx].feed->topic);
              aio_mqtt.subscribe(io_subs[findx].feed);
          }
          aio_mqtt_ctrl.state++;
          break;
        case 1:
          aio_mqtt_ctrl.connected =  aio_mqtt_connect();
          if (aio_mqtt_ctrl.connected == 0) 
          {
            aio_mqtt_ctrl.state++;
            aio_mqtt_ctrl.conn_faults = 0;
          }
          break;
        case 2:   
          aio_mqtt_ctrl.state++;
          break;
        case 3:
          Serial.print(F("Read Subscription\n"));
          //time_to_string(&time_str);
          Serial.println(time_str);
          while ((aio_subscription = aio_mqtt.readSubscription(500))) 
          {
              //Serial.println(aio_subscription->topic);
              for (uint8_t sindx = AIO_SUBS_TIME_ISO_8601; sindx < AIO_SUBS_NBR_OF; sindx++ )
              {
                  if ((aio_subscription == io_subs[sindx].feed) &&
                      (millis() > io_subs[sindx].next_update_ms ))
                  {
                      io_subs[sindx].next_update_ms += AIO_MIN_SUBSCRIBE_IVAL_ms;
                      Serial.print(io_subs[sindx].feed->topic);
                      Serial.print(F(": "));
                      //value_str = (char*)io_subs[sindx].feed->lastread;
                      strcpy(io_subs[sindx].value_str, (char*)io_subs[sindx].feed->lastread);
                      Serial.println(io_subs[sindx].value_str);
                      io_subs[sindx].show_counter = io_subs[sindx].show_iter;
                      io_subs[sindx].updated = true;
                      //ctrl.set_temp = atoi((char *)set_temperature.lastread);
                      //Serial.println(ctrl.set_temp);
                  }                  
              }
          }
          aio_mqtt_ctrl.state++;
          break;
        case 4:
            if (! aio_publ[AIO_PUBL_VA_HOME_MODE]->publish((float)aio_mqtt_ctrl.at_home)) 
            {
              Serial.println(F("Publish Failed"));
            } else 
            {
              Serial.println(F("Publish OK!"));
            }
            if (aio_mqtt_ctrl.at_home > 0) aio_mqtt_ctrl.at_home = 0;
            else aio_mqtt_ctrl.at_home = 1;
            aio_mqtt_ctrl.state = 1;
          break;
        case 5:
          break;
        case 6:
          break;
        case 7:
          break;
        case 8:
          break;

      }
    }
}

void aio_mqtt_print_feeds(void)
{
    for ( uint8_t findx = AIO_SUBS_TIME_ISO_8601; findx < AIO_SUBS_NBR_OF; findx++)
    {
        Serial.printf("%20s  ",io_subs[findx].label);
        Serial.printf("%40s  ",io_subs[findx].feed->topic);
        Serial.printf("%6d  ",io_subs[findx].next_update_ms);
        if (io_subs[findx].updated) Serial.printf("Was updated  ");
        else Serial.printf("Not updated  ");
        Serial.printf("%20s",io_subs[findx].value_str);
        Serial.printf("\n");
    }    
}


