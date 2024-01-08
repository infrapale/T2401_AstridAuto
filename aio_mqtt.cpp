/**

  https://github.com/adafruit/Adafruit_MQTT_Library

 */

#include "aio_mqtt.h"

#include <stdint.h>
#include "stdio.h"
#include "pico/stdlib.h"
//#include "hardware/uart.h"
#include "main.h"
#include "secrets.h"
#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "time.h"
#include "log.h"
#include "dashboard.h"

// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    IO_USERNAME
#define AIO_KEY         IO_KEY
#define AIO_PUBLISH_INTERVAL_ms     60000
#define AIO_MIN_SUBSCRIBE_IVAL_ms   10000


typedef struct 
{
  int8_t  connected;
  uint8_t state;
  uint16_t conn_faults;
  uint8_t at_home;

} aio_mqtt_ctrl_st;

aio_mqtt_ctrl_st aio_mqtt_ctrl =
{
  .connected = false,
  .state = 0,
  .conn_faults = 0,
  .at_home = 0,
};


extern WiFiClient client;
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



// extern Adafruit_MQTT_Subscribe *aio_subs[];
// extern Adafruit_MQTT_Publish *aio_publ[];
// extern Adafruit_MQTT_Client aio_mqtt;

// extern value_st subs_data[];

Adafruit_MQTT_Subscribe *aio_subscription;



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




void aio_mqtt_stm(void *param)
{
    (void) param;
    uint16_t v_delay_ms = 5000 ;
    String time_str; 
    String value_str;
    float  value;
    uint32_t unix_time;

    // Serial.println(F("aio_mqtt_stm - init"));
    while(true)
    {
      unix_time = time_get_epoc_time();
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
          time_to_string(&time_str);
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
      vTaskDelay( v_delay_ms / portTICK_PERIOD_MS );
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