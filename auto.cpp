#include "auto.h"
#include "main.h"

extern task_st task[TASK_NBR_OF];
extern astrid_st astrid;


auto_key_st key_func[AUTO_KEY_NBR_OF] =
{
    [AUTO_KEY_ALL_OFF]      = {"All Off  ", '3', '1'},
    [AUTO_KEY_AT_HOME]      = {"Kotona   ", '3', '2'},
    [AUTO_KEY_HALO]         = {"Halogen  ", '3', '3'},
    [AUTO_KEY_4]            = {"Key 4    ", '3', '4'},
    [AUTO_KEY_TK]           = {"TK       ", '3', '5'},
    [AUTO_KEY_TUPA_1]       = {"Tupa 1   ", '3', '6'},
    [AUTO_KEY_TUPA_2]       = {"Tupa 2   ", '3', '7'},
    [AUTO_KEY_PARVI]        = {"Parvi    ", '3', '8'},
    [AUTO_KEY_KEITTIO]      = {"Keittio  ", '4', '1'},
    [AUTO_KEY_KHH]          = {"KHH      ", '4', '2'},
    [AUTO_KEY_PIHA]         = {"Piha     ", '4', '3'},
    [AUTO_KEY_12]           = {"Key 12   ", '4', '4'},
    [AUTO_KEY_13]           = {"Key 13   ", '4', '5'},
    [AUTO_KEY_14]           = {"Key 14   ", '4', '6'},
    [AUTO_KEY_15]           = {"Key 15   ", '4', '7'},
    [AUTO_KEY_16]           = {"Key 16   ", '4', '8'},
};



void auto_state_machine(void)
{
    switch(task[TASK_ASTRID_AUTO].state)
    {
      case ASTRID_UNDEFINED:
        task[TASK_ASTRID_AUTO].state = ASTRID_AT_HOME; 
        break;
      case ASTRID_AT_HOME:
        break;
      case ASTRID_GOING:
        break;
      case ASTRID_AWAY:
        break;
    }
    task_print_status()
}