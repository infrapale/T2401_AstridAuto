  
#include "relay.h"
  
typedef struct
{
    char  unit[4];
    char  relay[6];
} relay_addr_et;


relay_addr_et relay[1] =
{
    [VA_RELAY_TK] =       {"TK1", "RTK__"},
    [VA_RELAY_ET] =       {"TK1", "RET_1"},
    [VA_RELAY_WC_1] =     {"TK1", "RWC_1"},
    [VA_RELAY_WC_2] =     {"MH2", "RWC_2"},
    [VA_RELAY_TUPA_1] =   {"TK1", "RTUP1"},
    [VA_RELAY_TUPA_2] =   {"TK1", "RTUP2"},
    [VA_RELAY_K_1] =      {"MH1", "RKOK1"},
    [VA_RELAY_K_2] =      {"MH2", "RKOK2"},
    [VA_RELAY_KHH_1] =    {"TK1", "KHH_1"},
    [VA_RELAY_KHH_2] =    {"MH2", "KHH_2"},
    [VA_RELAY_SAUNA] =    {"TK1", "RSAUN"},
    [VA_RELAY_PARVI] =    {"TK1", "RPARV"},
    [VA_RELAY_PIHA] =     {"TK1", "RPIHA"},
    [VA_RELAY_TERASSI] =  {"TK1", "RTERA"},
    [VA_RELAY_POLKU] =    {"TK1", "RPOLK"},

};
  /*
  
    switch(btn){
       case '1': add_code("MH2","RWC_2","T"); 
                 break;        
       case '2': add_code("MH2","RET_1","T"); 
                 break;
       case '3': add_code("TK1","RPOLK","T"); 
                 break;
       case '4': add_code("MH2","RMH21","T"); 
                 add_code("MH2","RMH22","T"); 
                 break;     
       case '5': add_code("TK1","RPARV","T"); 
                 break;   
       case '6': add_code("MH1","RMH11","T"); 
                 add_code("MH1","RMH12","T"); 
                 add_code("MH1","RMH13","T"); 
                 break;   
       case '7': add_code("MH2","RKHH2","T"); 
                 add_code("MH2","RPSH1","T");
                 break;   
       case '8': add_code("TK1","RTUP1","T"); 
                 add_code("TK1","RTUP2","T");
                 break;   
       case '9': add_code("MH1","RKOK1","T"); 
                 add_code("MH1","RKOK2","T"); 
                 add_code("MH1","RKOK3","T"); 
                 add_code("MH1","RKOK4","T"); 
                 add_code("MH1","RKOK5","T"); 
                  break;   
       case '*': add_code("MH1","xxxxx","T"); break;   
       case '0': add_code("MH1","*.OFF","0"); 
                 add_code("MH2","*.OFF","0");
                 add_code("TK1","*.OFF","0");
                 break;   
       case '#': add_code("MH1","RKOK3","T"); 
                 add_code("MH1","RKOK4","T"); 
                 add_code("MH1","RKOK5","T"); 
                 break;   
  */