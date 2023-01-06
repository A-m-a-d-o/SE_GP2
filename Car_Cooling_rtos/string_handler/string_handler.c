/*
 * string_handler.c
 *
 *  Created on: 04/01/2023
 *      Author: Pedro Amado
 */

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


    return 0;

}
