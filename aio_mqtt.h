#ifndef __AIO_MQTT_H__
#define __AIO_MQTT_H__
#include "main.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

typedef enum 
{
  AIO_SUBS_TIME_ISO_8601 = 0,
  AIO_SUBS_VA_OD_TEMP,
  AIO_SUBS_VA_OD_HUM,
  AIO_SUBS_VA_TUPA_TEMP,
  AIO_SUBS_NBR_OF
} aio_subs_et;

typedef enum 
{
  AIO_PUBL_VA_HOME_MODE = 0,
  AIO_PUBL_VA_AC_TEMP,
  AIO_PUBL_NBR_OF
} aio_publ_et;


typedef struct 
{
    Adafruit_MQTT_Subscribe *feed;
    char      label[LABEL_LEN];
    unit_et   unit_index;
    char      value_str[40];
    bool      updated;
    uint32_t  next_update_ms;
    uint8_t   show_iter;
    uint8_t   show_counter;
} aio_subs_st;




void aio_mqtt_stm(void);

void aio_mqtt_print_feeds(void);

#endif