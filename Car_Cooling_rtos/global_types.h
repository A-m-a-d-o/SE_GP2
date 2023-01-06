/*
 * global_types.h
 *
 *  Created on: 04/01/2023
 *      Author: Pedro Amado
 */

#ifndef GLOBAL_TYPES_H_
#define GLOBAL_TYPES_H_

#define TEMP_READING 0

xQueueHandle LCD_write_queue;

typedef struct lcd_msg_
{
    uint8_t order_id;
    float data;
}lcd_msg;

#endif /* GLOBAL_TYPES_H_ */
