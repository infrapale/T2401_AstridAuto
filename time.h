#ifndef __TIME_H__
#define __TIME_H__

#include "Arduino.h"
#include "RTClib.h"

bool time_begin(void);
void time_lost_power(bool force_new_time);
DateTime *time_get_time_now(void);
uint32_t time_get_epoc_time(void);
void time_to_string(String *Str);

void time_print(DateTime dt);

void time_test_iso_8601(void);

#endif