/*
 * string_handler.c
 *
 *  Created on: 04/01/2023
 *      Author: Pedro Amado
 */

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void float_to_string( char * out, float in)
{
    int inteira= 0, decimal = 0, i, contas[3];

    inteira = (int) in;
    decimal = (int) ( (in - inteira) * 100 );

    contas[0] = inteira / 100;
    contas[1] = (inteira - contas[0]*100) /10;
    contas[2] = inteira - contas[0]*100 - contas[1]*10;

    for(i = 0; i < 3; i++)
    {
        out[i] = contas[i] + '0';
    }
    out[3] = '.';

    contas[0] = decimal / 10;
    contas[1] = (decimal - contas[0]*10);

    for(i = 0; i < 2; i++)
    {
        out[i+4] = contas[i] + '0';
    }

    out[6] = '\0';


    return;

}

void char_to_int(int * date, char * in)
{

    date[0] = (in[0] - '0')*10;

    date[0] += (in[1] - '0');

    date[1] = (in[2] - '0')*10;

    date[1] += (in[3] - '0');

    date[2] = (in[4] - '0') * 1000;

    date[2] += (in[5] - '0') * 100;

    date[2] += (in[6] - '0') * 10;

    date[2] += (in[7] - '0');

}

void int_to_char(int * in, char * date)
{

    date[0] = (in[0]/10) +'0' ;
    date[1] = (in[0]%10) +'0' ;
    date[2] = '/';
    date[3] = (in[1]/10) +'0' ;
    date[4] = (in[1]%10) +'0' ;
    date[5] = '/';
    date[6] = (in[2]/1000) +'0' ;
    date[7] = ((int)((in[2]%1000)/100)) +'0' ;
    date[8] = ((int)((in[2]%100)/10)) +'0' ;
    date[9] = ((int)(in[2]%10)) +'0' ;
    date[10] = '\0' ;


}

void char_to_float(float * temp, char * in)
{

    * temp = in[0]*100;
    * temp += in[1]*10;
    * temp += in[2];
    * temp += in[3]/10;
    * temp += in[4]/100;


}

uint32_t string_to_seconds( char * in)
{
    int time =0;

    time = ((in[0] - '0')*10 + (in[1] - '0'))*3600;

    time += ((in[2] - '0')*10 + (in[3] - '0'))*60;

    time += ((in[4] - '0')*10 + (in[5] - '0'));

    if(time <=0 || time > 86400)
        return 0;




    return time;

}


void seconds_to_string( uint32_t in, char * time)
{

    uint32_t hours, minutes, seconds;

    hours =(uint32_t) in/3600;

    minutes = (uint32_t) (in/60)%60;

    seconds = (uint32_t) in%60;

    time[0] = (hours / 10) + '0';

    time[1] = (hours % 10) + '0';

    time[2] = ':';

    time[3] = (minutes / 10) + '0';

    time[4] = (minutes % 10) + '0';

    time[5] = ':';

    time[6] = (seconds / 10) + '0';

    time[7] = (seconds % 10) + '0';

    time[8] = '\0';

}



