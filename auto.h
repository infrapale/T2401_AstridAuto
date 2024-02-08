#ifndef __AUTO_H__
#define __AUTO_H__
#include "main.h"

#define AUTO_KEY_NAME_LEN  10

typedef enum
{
    AUTO_KEY_ALL_OFF = 0,
    AUTO_KEY_AT_HOME,
    AUTO_KEY_HALO,
    AUTO_KEY_4,
    AUTO_KEY_TK,
    AUTO_KEY_TUPA_1,
    AUTO_KEY_TUPA_2,
    AUTO_KEY_PARVI,
    AUTO_KEY_KEITTIO,
    AUTO_KEY_KHH,
    AUTO_KEY_PIHA,
    AUTO_KEY_12,
    AUTO_KEY_13,
    AUTO_KEY_14,
    AUTO_KEY_15,
    AUTO_KEY_16,
    AUTO_KEY_NBR_OF
} auto_key_et;



typedef struct 
{
    char  name[AUTO_KEY_NAME_LEN]; 
    char  group;
    char  key;
} auto_key_st;

void auto_state_machine(void);

#endif