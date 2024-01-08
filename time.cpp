// https://github.com/adafruit/RTClib/blob/master/src/RTClib.h
// https://adafruit.github.io/RTClib/html/class_date_time.html
// https://arduinogetstarted.com/tutorials/arduino-rtc


#include <Wire.h>
#include "time.h"
#include "aio_mqtt.h"

#define TIME_ZONE_ADD   7200
extern aio_subs_st io_subs[AIO_SUBS_NBR_OF];
RTC_PCF8563 rtc;

typedef enum
{
    TIME_COMPILED   = 0,
    TIME_RTC,
    TIME_FEED,
    TIME_NBR_OF
} time_index_et;

typedef struct
{
    DateTime date_time;
    bool updated;
} time_st;

time_st my_time[TIME_NBR_OF];

char week_day[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

DateTime time_now;


bool time_begin(void)
{
  bool ret = false;
  if (! rtc.begin()) 
  {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    //while (1) delay(10);
  } 
  else 
  {
    ret = true;
    time_lost_power(true);
    rtc.start();
  }
  my_time[TIME_COMPILED].date_time = DateTime(F(__DATE__), F(__TIME__));
  my_time[TIME_COMPILED].updated = true;
  my_time[TIME_RTC].date_time  = rtc.now();
  my_time[TIME_RTC].updated = true;
  my_time[TIME_FEED].date_time  = rtc.now();
  my_time[TIME_FEED].updated = false;
  
  Serial.print("TIME_COMPILED:");
  time_print( my_time[TIME_COMPILED].date_time);
  Serial.print("TIME_RTC:");
  time_print(my_time[TIME_RTC].date_time);
  Serial.flush();
  return ret;
}

void time_lost_power(bool force_new_time)
{
  if (rtc.lostPower() || force_new_time) 
  {
      Serial.println("RTC is NOT initialized, let's set the time!");
      // When time needs to be set on a new device, or after a power loss, the
      // following line sets the RTC to the date & time this sketch was compiled
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      // This line sets the RTC with an explicit date & time, for example to set
      // January 21, 2014 at 3am you would call:
      //rtc.adjust(DateTime(2023, 12, 13, 3, 0, 0));
      //
      // Note: allow 2 seconds after inserting battery or applying external power
      // without battery before calling adjust(). This gives the PCF8523's
      // crystal oscillator time to stabilize. If you call adjust() very quickly
      // after the RTC is powered, lostPower() may still return true.


    // When the RTC was stopped and stays connected to the battery, it has
    // to be restarted by clearing the STOP bit. Let's do this to ensure
    // the RTC is running.
  }

}

DateTime *time_get_time_now(void)
{
    time_now = rtc.now();
    return &time_now;
}


void time_to_string(String *Str)
{
  DateTime now = rtc.now();
  char s[4];

  *Str = String("");
  *Str += now.year();
  *Str += String("-");
  *Str += now.month();
  *Str += String("-");
  *Str += now.day();
  *Str += String(" ");

  sprintf(s,"%02d",now.hour());
  *Str += s;
  *Str += ":";
  sprintf(s,"%02d",now.minute());
  *Str += s;
  *Str += ":";
  sprintf(s,"%02d",now.second());
  *Str += s;
}

uint32_t time_get_epoc_time(void)
{
    DateTime now = rtc.now();
    return now.unixtime();
}

void time_set_aio_feed(void)
{
    DateTime date_time = DateTime(io_subs[AIO_SUBS_TIME_ISO_8601].value_str);
    if (date_time.isValid()) 
    {
        TimeSpan tz_adjust(TIME_ZONE_ADD);
        date_time = date_time + tz_adjust;
        rtc.adjust(date_time);
    }
}

void time_set_compiled(void)
{
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void time_print(DateTime dt)
{
    //DateTime now = rtc.now();

    Serial.print(dt.year(), DEC);
    Serial.print('/');
    Serial.print(dt.month(), DEC);
    Serial.print('/');
    Serial.print(dt.day(), DEC);
    Serial.print(" (");
    Serial.print(week_day[dt.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(dt.hour(), DEC);
    Serial.print(':');
    Serial.print(dt.minute(), DEC);
    Serial.print(':');
    Serial.print(dt.second(), DEC);
    Serial.println();

    // Serial.print(" since midnight 1/1/1970 = ");
    // Serial.print(dt.unixtime());
    // Serial.print("s = ");
    // Serial.print(dt.unixtime() / 86400L);
    // Serial.println("d");
}

void time_test_iso_8601(void)
{
    char iso8601[] = "2024-01-05T13:52:51.107Z";
    DateTime date_time(iso8601);
    //uint32_t sec_time;
    TimeSpan tz_adjust(TIME_ZONE_ADD);
    //sec_time = date_time1.secondstime();
    // DateTime date_time(sec_time + TIME_ZONE_ADD); 
    date_time = date_time + tz_adjust;
    Serial.println(iso8601);
    if (date_time.isValid()) 
    {
        Serial.println("Is valid!"); 
        Serial.print(date_time.year()); Serial.print("-");
        Serial.print(date_time.month()); Serial.print("-");
        Serial.print(date_time.day()); Serial.print(" ");
        Serial.print(date_time.hour()); Serial.print(":");
        Serial.print(date_time.minute()); Serial.print("\n");

    }
    else
    {
        Serial.println("Is NOT valid!"); 
    }

}

