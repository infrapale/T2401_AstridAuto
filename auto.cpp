#include "main.h"
#include "auto.h"
#include "relay.h"

#define AUTO_NBR_OF_GROUPS      8
#define AUTO_NBR_OF_RELAYS      4
#define AUTO_NBR_OF_TIME_GROUPS 4


relay_group_t auto_group[AUTO_NBR_OF_GROUPS] =
{
    [0] = {VA_RELAY_UNDEF,  VA_RELAY_UNDEF, VA_RELAY_UNDEF,   VA_RELAY_UNDEF},
    [1] = {VA_RELAY_PARVI,  VA_RELAY_UNDEF, VA_RELAY_UNDEF,   VA_RELAY_UNDEF},
    [2] = {VA_RELAY_TK,     VA_RELAY_PARVI, VA_RELAY_UNDEF,   VA_RELAY_UNDEF},
    [3] = {VA_RELAY_PARVI,  VA_RELAY_TUPA_1, VA_RELAY_UNDEF,   VA_RELAY_UNDEF},
    [4] = {VA_RELAY_PARVI,  VA_RELAY_TUPA_2, VA_RELAY_KEITTIO_1, VA_RELAY_UNDEF},
    [5] = {VA_RELAY_TK,     VA_RELAY_PARVI, VA_RELAY_PIHA,    VA_RELAY_UNDEF},
    [6] = {VA_RELAY_PIHA,   VA_RELAY_PARVI, VA_RELAY_UNDEF,   VA_RELAY_UNDEF},
    [7] = {VA_RELAY_PIHA,  VA_RELAY_UNDEF, VA_RELAY_UNDEF,   VA_RELAY_UNDEF}
};


uint8_t time_group[24][AUTO_NBR_OF_TIME_GROUPS] =
{
    [0] =  {1, 0, 0, 0},
    [1] =  {1, 2, 0, 0},
    [2] =  {1, 0, 0, 0},
    [3] =  {1, 0, 0, 0},
    [4] =  {1, 0, 0, 0},
    [5] =  {1, 2, 0, 0},
    [6] =  {1, 3, 0, 0},
    [7] =  {1, 3, 0, 0},
    [8] =  {1, 0, 0, 0},
    [9] =  {1, 0, 0, 0},
    [10] = {1, 0, 0, 0},
    [11] = {1, 0, 0, 0},
    [12] = {1, 0, 0, 0},
    [13] = {1, 0, 0, 0},
    [14] = {1, 0, 0, 0},
    [15] = {1, 3, 0, 0},
    [16] = {1, 4, 0, 0},
    [17] = {1, 3, 5, 0},
    [18] = {6, 0, 0, 0},
    [19] = {7, 0, 0, 0},
    [20] = {5, 0, 0, 0},
    [21] = {4, 0, 0, 0},
    [22] = {3, 0, 0, 0},
    [23] = {2, 0, 0, 0},
};


relay_group_t *auto_get_auto_group(uint8_t hour)
{
  return &auto_group[time_group[hour][0]];
}
