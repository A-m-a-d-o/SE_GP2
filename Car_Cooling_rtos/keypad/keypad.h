#ifndef KEYPAD_H_
#define KEYPAD_H_


char key_pressed(uint8_t col, uint8_t row);
void keypad_init(void);
char detect_key(void);
void PortEIntHandler(void);
void int_tick_ms_handler(void);
void keypad_processing(void);


#endif /* KEYPAD_H_ */
