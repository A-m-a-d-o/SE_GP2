/*
 * global_types.h
 *
 *  Created on: 04/01/2023
 *      Author: Pedro Amado
 */

#ifndef GLOBAL_TYPES_H_
#define GLOBAL_TYPES_H_

#define TEMP_READING 0
#define SPEED_READING 1
#define DATE_MODE 2
#define TIME_MODE 3
#define MENU_MODE 4
#define MIN_THR_MODE 5
#define MAX_THR_MODE 6
#define DATE_TIME_SCREEN 7
#define STOP_ALARM 8




xQueueHandle LCD_write_queue, Menu_queue, PWM_queue, Alert_queue, Menu_lcd_queue;

xSemaphoreHandle temp_mutex;


typedef struct lcd_msg_
{

    uint8_t order_id;
    float data;
    char ch;
    char pt_time[9], pt_date[11];

}lcd_msg;

typedef struct pwm_msg_
{

    uint8_t order_id;
    float temp;

}pwm_msg;



#endif /* GLOBAL_TYPES_H_ */
