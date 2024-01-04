#include "SerialUSB.h"
#include "main.h"
#include "dashboard.h"
#include "time.h"

#define NBR_BOXES           7


extern value_st subs_data[];

extern TFT_eSPI tft;

disp_box_st db_box[NBR_BOXES] =
{
  {  0,   0, 319, 172, "Box 0", 4, 1, TFT_BLACK, TFT_LIGHTGREY, TFT_LIGHTGREY },
  {  0,  72, 319,  80, "Box 1", 8, 1, TFT_BLACK, TFT_GOLD, TFT_GOLD },
  {  0,   0, 319,  32, "Box 2", 4, 1, TFT_BLACK, TFT_GOLD, TFT_WHITE },
  {  0,  32, 319,  32, "Box 3", 4, 1, TFT_BLACK, TFT_GOLD, TFT_WHITE },
  {  0,  64, 319,  32, "Box 4", 4, 1, TFT_BLACK, TFT_GOLD, TFT_WHITE},
  {  0,  96, 319,  32, "Box 5", 4, 1, TFT_BLACK, TFT_GOLD, TFT_WHITE},
  {  0,  90, 319,  90, "Box 6", 8, 1, TFT_BLACK, TFT_VIOLET, TFT_GOLD },
};


char zone_main_label[NBR_MAIN_ZONES][MAIN_ZONE_LABEL_LEN] =
{ 
  // 01234567890123456789
    "Villa Astrid   ",
    "Lilla Astrid   ",
    "Laituri        ",
    "Tampere        "    
};


char unit_label[NBR_UNITS][UNIT_LABEL_LEN] =
{
  // 012345678
    "Celsius ",
    "%       ",
    "kPa     ",
    "Lux     ",
    "LDR     "
};

char measure_label[NBR_UNITS][MEASURE_LABEL_LEN] =
{
  // 0123456789012345
    "Temperature    ",
    "Humidity       ",
    "Air Pressure   ",
    "Light          ",
    "LDR Value      "
};

value_st subs_data[AIO_SUBS_NBR_OF]
{
  [AIO_SUBS_TIME_ISO_8601] = {ZONE_VILLA_ASTRID, "IO ",  UNIT_TEMPERATURE, 0.0},
  [AIO_SUBS_VA_OD_TEMP] = {ZONE_VILLA_ASTRID, "OD ",  UNIT_TEMPERATURE, 0.0},
  [AIO_SUBS_VA_OD_HUM]  = {ZONE_VILLA_ASTRID, "OD ",  UNIT_HUMIDITY, 0.0}
};


void dashboard_draw_box(uint8_t bindx)
{
    tft.setTextSize(db_box[bindx].font_size);
    tft.setTextColor(db_box[bindx].text_color, db_box[bindx].fill_color, false);
    tft.fillRect(db_box[bindx].x_pos, db_box[bindx].y_pos, db_box[bindx].width, db_box[bindx].height, db_box[bindx].fill_color);
    tft.drawString( db_box[bindx].txt , db_box[bindx].x_pos+4, db_box[bindx].y_pos+2, db_box[bindx].font_indx);
    Serial.print("Box: "); Serial.print(bindx); Serial.print(" = ");Serial.println(db_box[bindx].txt);
}

void dashboard_update_all(void)
{
    for (uint8_t i = 0; i < NBR_BOXES; i++)
    {
        dashboard_draw_box(i);
    }
}

void dashboard_set_text(uint8_t box_indx, char *txt_ptr)
{
    strcpy(db_box[box_indx].txt, txt_ptr);
}

void dashboard_big_time(void)
{
    DateTime *now = time_get_time_now();
    char s1[4];
    
    db_box[4].txt[0] = 0x00;
    dashboard_draw_box(4);
 
    sprintf(s1,"%02d",now->hour());
    String time_str = s1;
    time_str += ":";
    sprintf(s1,"%02d",now->minute());
    time_str += s1;
    time_str.toCharArray(db_box[1].txt, TXT_LEN);
    Serial.println(db_box[1].txt);
    dashboard_draw_box(1);
}

void dashboard_show_info(void)
{
    String Str_info = APP_NAME;
    Str_info += "\n";
    Str_info += __DATE__;
    Str_info += __TIME__;

    strcpy(db_box[0].txt, " ");
    dashboard_draw_box(0);
 
    strcpy(db_box[2].txt, APP_NAME);
    dashboard_draw_box(2);

    strcpy(db_box[3].txt, __DATE__);
    dashboard_draw_box(3);

    strcpy(db_box[4].txt, __TIME__);
    dashboard_draw_box(4);

    // Str_info.toCharArray(db_box[0].txt, TXT_LEN);

}

void dashboard_show_common(void)
{
    String time_str;
    strcpy(db_box[2].txt, MAIN_TITLE);
    dashboard_draw_box(2);
    time_to_string(&time_str);
    time_str.toCharArray(db_box[3].txt, TXT_LEN);
    dashboard_draw_box(3);
}

void dashboard_clear(void)
{

}
void dashboard_update_task(void *param)
{
    (void) param;
    uint8_t state = 0;
    uint16_t v_delay_ms = 1000;
    bool    update_box;
    String  Str;

    vTaskDelay( 5000 / portTICK_PERIOD_MS );
    for (;;)
    {
        Serial.print("dashboard_update_task state: "); Serial.println(state);
        switch (state)
        {
            case 0:
                dashboard_show_info();
                v_delay_ms = 10000;
                state++;
                break;
            case 1:                
                dashboard_show_common();
                dashboard_big_time();
                v_delay_ms = 5000;
                state++;
                break;
            case 2:
                update_box = false;
                for (uint8_t i = AIO_SUBS_TIME_ISO_8601; (i < AIO_SUBS_NBR_OF) && !update_box; i++ )
                {
                    Serial.print("aio index: "); Serial.println(i); 
                    if (subs_data[i].updated)
                    {
                        Serial.println("Updated ");
                        subs_data[i].updated = false;
                        switch(i)
                        {
                            case AIO_SUBS_TIME_ISO_8601:
                              Serial.println("Time updated");
                              break;
                            case AIO_SUBS_VA_OD_TEMP:
                              Str = zone_main_label[subs_data[i].main_zone_index];
                              Str += " ";
                              Str += subs_data[i].sub_zone;
                              Str.toCharArray(db_box[2].txt,40);

                              Str = measure_label[subs_data[i].unit_index];
                              Str += " ";
                              Str += unit_label[subs_data[i].unit_index];
                              Str.toCharArray(db_box[3].txt, TXT_LEN);

                              Str = String(subs_data[i].value);
                              Serial.println(Str);
                              Str.toCharArray(db_box[1].txt,6);
                              update_box = true;
                              break;
                            case AIO_SUBS_VA_OD_HUM:
                              break;
                        }
                        if (update_box)
                        {
                            dashboard_draw_box(0);
                            dashboard_draw_box(1);
                            dashboard_draw_box(2);
                            dashboard_draw_box(3);
                        }
                    }
                }
                v_delay_ms = 5000;
                state = 1;

                break;

        }
        vTaskDelay( v_delay_ms / portTICK_PERIOD_MS );
    }


}