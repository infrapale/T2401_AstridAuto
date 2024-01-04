  
#include "relay.h"
#include "main.h"
 

relay_addr_st relay_addr[VA_RELAY_NBR_OF] =
{
    [VA_RELAY_UNDEF] =      {"XXX", "XXXXX"},
    [VA_RELAY_TK] =         {"TK1", "RTK__"},
    [VA_RELAY_ET] =         {"TK1", "RET_1"},
    [VA_RELAY_WC_1] =       {"TK1", "RWC_1"},
    [VA_RELAY_WC_2] =       {"MH2", "RWC_2"},
    [VA_RELAY_TUPA_1] =     {"TK1", "RTUP1"},
    [VA_RELAY_TUPA_2] =     {"TK1", "RTUP2"},
    [VA_RELAY_MH1_1] =      {"MH1", "RMH11"},
    [VA_RELAY_MH1_2] =      {"MH1", "RMH12"},
    [VA_RELAY_MH1_3] =      {"MH1", "RMH13"},
    [VA_RELAY_MH1_4] =      {"MH1", "RMH14"},
    [VA_RELAY_MH1_5] =      {"MH1", "RMH15"},
    [VA_RELAY_MH2_1] =      {"MH2", "RMH21"},
    [VA_RELAY_MH2_2] =      {"MH2", "RMH22"},
    [VA_RELAY_KEITTIO_1] =  {"MH1", "RKOK1"},
    [VA_RELAY_KEITTIO_2] =  {"MH2", "RKOK2"},
    [VA_RELAY_KEITTIO_3] =  {"MH2", "RKOK3"},
    [VA_RELAY_KEITTIO_4] =  {"MH2", "RKOK4"},
    [VA_RELAY_KEITTIO_5] =  {"MH2", "RKOK5"},
    [VA_RELAY_KHH_1] =      {"TK1", "RKHH1"},
    [VA_RELAY_KHH_2] =      {"MH2", "RKHH2"},
    [VA_RELAY_PSH] =        {"MH2", "RPSH_"},
    [VA_RELAY_SAUNA] =      {"TK1", "RSAUN"},
    [VA_RELAY_PARVI] =      {"TK1", "RPARV"},
    [VA_RELAY_PIHA] =       {"TK1", "RPIHA"},
    [VA_RELAY_TERASSI] =    {"TK1", "RTERA"},
    [VA_RELAY_POLKU] =      {"TK1", "RPOLK"},
};

const va_relays_et relay_group[VA_RELAY_GROUP_NBR_OF][RELAYS_PER_GROUP]=
{
  [VA_RELAY_GROUP_MH1]  =     {VA_RELAY_MH1_1, VA_RELAY_MH1_2, VA_RELAY_MH1_3, VA_RELAY_MH1_4},
  [VA_RELAY_GROUP_MH2]  =     {VA_RELAY_MH2_1, VA_RELAY_MH2_2, VA_RELAY_UNDEF, VA_RELAY_UNDEF},
  [VA_RELAY_GROUP_TUPA] =     {VA_RELAY_TUPA_1, VA_RELAY_TUPA_1, VA_RELAY_UNDEF, VA_RELAY_UNDEF},
  [VA_RELAY_GROUP_KEITTIO] =  {VA_RELAY_KEITTIO_1,VA_RELAY_KEITTIO_2,VA_RELAY_UNDEF, VA_RELAY_UNDEF},
  [VA_RELAY_GROUP_KHH_PSH] =  {VA_RELAY_KHH_1, VA_RELAY_KHH_2, VA_RELAY_PSH, VA_RELAY_SAUNA},
  [VA_RELAY_GROUP_ULKO]    =  {VA_RELAY_PIHA, VA_RELAY_TERASSI, VA_RELAY_POLKU, VA_RELAY_UNDEF}

};

const relay_addr_st *relay_get_addr_ptr( va_relays_et relay_id)
{
    return &relay_addr[relay_id];
}

const va_relays_et relay_get_group_item(va_relay_group_et gindx, uint8_t rindx)
{
    return relay_group[gindx][rindx];
}

