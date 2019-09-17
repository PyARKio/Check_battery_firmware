

#ifndef __hardware__
#define __hardware__
typedef unsigned char byte;

#define MASK_BUTTON 0x01

void func_timer0_ovf();
void func_ADC(void);
void func_rx_commands(int Num_Com);
void func_send_ADC_result(void);
void func_convertion_data(int VAR_VAR);
void func_check_threshold(int VAR_VAR_U);
char is_bitf(unsigned char bit);
void clrbf(unsigned char bit);
void setbf(unsigned char bit);

#endif
