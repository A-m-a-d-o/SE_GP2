/*
 * string_handler.h
 *
 *  Created on: 04/01/2023
 *      Author: Pedro Amado
 */

#ifndef STRING_HANDLER_H_
#define STRING_HANDLER_H_

void float_to_string( char * out, float in);
void char_to_float(float * temp, char * in);
void char_to_int(int * date, char * in);
void int_to_char(int * in, char * date);
uint32_t string_to_seconds( char * in);
void seconds_to_string( uint32_t in, char * time);

#endif /* STRING_HANDLER_H_ */
