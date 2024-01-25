#include "main.h"
#include "dashboard.h"
#include "task.h"


task_st task[TASK_NBR_OF] = 
{
    [TASK_AIO] =          {"AIO            ", 1000,0, 0, 255, aio_mqtt_stm },
    [TASK_DASHBOARD] =    {"Dashboard      ", 1000,0, 0, 255, dashboard_update_task },
    [TASK_READ_BUTTON] =  {"Read Button    ", 1000,0, 0, 255, read_button_task },
};


void task_run(void)
{
    static uint8_t task_indx = 0;

    uint32_t now_ms = millis();
    if (task_indx < TASK_NBR_OF)
    {
        if (now_ms >= task[task_indx].next_run )
        {
            task[task_indx].prev_state = task[task_indx].state;
            task[task_indx].next_run = now_ms + task[task_indx].interval;
            (*task[task_indx].cb)();
        }
    } 
    if (++task_indx >= TASK_NBR_OF) task_indx = 0;
}

void task_print_status(void)
{   
    // bool do_print = false;
    // Serial.printf("now = %d\n", millis());
    for (uint8_t i = TASK_AIO; i < TASK_NBR_OF; i++)
    {
        if (task[i].prev_state != task[i].state)
        {
            Serial.printf("%s: %d -> %d @ %d # %d\n",task[i].name, task[i].prev_state, task[i].state, millis(),task[i].next_run);
        }
    }
}

