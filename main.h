#ifndef __MAIN_H__
#define __MAIN_H__

#define   BOARD_PICO_TFT_4KEYS
#include <Arduino.h>
#include <board.h>
//#include <FreeRTOS.h>
//#include <semphr.h>
//#include "secrets.h"
//#include <WiFi.h>
//#include "Adafruit_MQTT.h"
// #include "Adafruit_MQTT_Client.h"
// #include "Adafruit_MQTT.h"
// #include <task.h>

#define LABEL_LEN   20
#define UNIT_LEN    6
#define TXT_LEN     40

#define APP_NAME   "T2401_AstridAuto"
#define MAIN_TITLE "Astrid Automatic Light"

#define SerialRfm  Serial1



typedef enum
{
    UNIT_UNDEFINED = 0,
    UNIT_TEMPERATURE,
    UNIT_HUMIDITY,
    UNIT_AIR_PRESSURE,
    UNIT_LIGHT,
    UNIT_LDR,
    UNIT_DATE_TIME,
    UNIT_DATE,
    UNIT_TIME,
    UNIT_NBR_OF
} unit_et;



typedef struct
{
    char            module;
    char            addr;         
} module_data_st;


typedef struct
{
    uint16_t x_pos;
    uint16_t y_pos;
    uint16_t width;
    uint16_t height;
    char     txt[TXT_LEN];
    uint8_t  font_indx;
    uint8_t  font_size;
    uint16_t fill_color;
    uint16_t border_color;
    uint16_t text_color;

} disp_box_st;


#endif