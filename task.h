#ifndef __TASK_H__
#define __TASK_H__

typedef void (*task_cb)(void);

typedef struct 
{
    char      name[16];
    uint32_t  interval;
    uint32_t  next_run;
    uint16_t  state;
    uint16_t  prev_state;
    task_cb   cb;
} task_st;

typedef enum
{
    TASK_AIO = 0,
    TASK_DASHBOARD,
    TASK_READ_BUTTON,
    TASK_NBR_OF
} tasks_et;


void task_run(void);

void task_print_status(void);



#endif