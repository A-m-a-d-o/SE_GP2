/*
 * LCD.h
 *
 *  Created on: 03/01/2023
 *      Author: mac
 */

#ifndef LCD_H_
#define LCD_H_

void LCD_Command(unsigned char c);
void LCD_Clear(void);
void LCD_init();
void LCD_Pulse();
void LCD_Write_c(unsigned char d);
void LCD_print(char * input);
void LCD_Cursor(uint8_t col);

#endif /* LCD_H_ */
