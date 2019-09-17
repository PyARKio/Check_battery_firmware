
#include <avr/io.h>
#include <util/delay.h>
#include <util/atomic.h>
#include "define.h"
#include "hardware.h"


char at[] = "A\r\n";


char number_symbol = 0;
char ADC_0[]="Command 0 recive!\r\n";
char ADC_1[]="Command 1 recive!\r\n";
char ADC_2[]="Command 2 recive!\r\n";
char ADC_3[]="Command 3 recive!\r\n";
char ADC_4[]="Command 4 recive!\r\n";
char ADC_5[]="Command 5 recive!\r\n";
char ADC_6[]="Command 6 recive!\r\n";
char ADC_7[]="Command 7 recive!\r\n";
char ADC_default[]="Unknown command recive!\r";
char next_string = 13;
char new_line = 10;

int Num_Com = 0;

char digits[10] = {48, 49, 50, 51, 52, 53, 54, 55, 56, 57};
char point[1] = {86};

char dist_thouthend=0;
char dist_hundrets=0;
char dist_tens=0;
char dist_ones=0;
volatile unsigned int VAR = 0;
char k_OAmp = 2;
char Vi = 5;
int R2 = 10;
char i = 0;
char discharge = 20;
char end_of_discharge = 7;
char LED_threshold = 20;

int VAR_U = 0;
int VAR_R = 0;

volatile unsigned int count_time_LED =0;
volatile unsigned int count_time_ADC =0;
int Num_Conver =48;

static uint32_t flag0,flag1;


void func_timer0_ovf()
{
	// LED_status
	if (count_time_LED == LED_threshold)
	{
		if (((PORTD>>LED_status)&MASK_BUTTON) != 1)
		{	PORTD |=(1<<LED_status);	}
		else
		{	PORTD &= ~(1<<LED_status);	}

		count_time_LED = 0;
	}
	else
	{ count_time_LED++; }
}


void func_ADC(void)
{
	//for (char number_symbol=0; number_symbol<3; number_symbol++)
	//{
	//	while (!(UCSRA&(1<<UDRE))) {};
	//	UDR = at[number_symbol];
	//}

	if (i<10)
	{
		VAR += ADC;
		i++;
		setbf(flag_repite);
	}
	else
	{
		i = 0;
		VAR = VAR/10;
		setbf(flag_end_ADC_conversion);
	}
}

/*
void func_rx_commands(int Num_Com)
{
	switch (Num_Com)
	{
		case Symbol_0:
		{
			for (char number_symbol=0; number_symbol<20; number_symbol++)
			{
				while (!(UCSRA&(1<<UDRE))) {};
				UDR = ADC_0[number_symbol];
			}
			
			break;
		}
		default:
		{
			for (char number_symbol=0; number_symbol<24; number_symbol++)
			{
				while (!(UCSRA&(1<<UDRE))) {};
				UDR = ADC_default[number_symbol];
			}
			break;
		}
	}
}
*/

void func_send_ADC_result(void)
{

	func_convertion_data(VAR);

	while (!(UCSRA&(1<<UDRE))) {};
	UDR = digits[dist_thouthend];
	while (!(UCSRA&(1<<UDRE))) {};
	UDR = digits[dist_hundrets];
	while (!(UCSRA&(1<<UDRE))) {};
	UDR = digits[dist_tens];
	while (!(UCSRA&(1<<UDRE))) {};
	UDR = digits[dist_ones];
	while (!(UCSRA&(1<<UDRE))) {};
	UDR = next_string;
	while (!(UCSRA&(1<<UDRE))) {};
	UDR = new_line;

	VAR_U = (VAR*5120.0)/1024;	// 0.0025 for mega8, 0.001074 for mega328
	func_convertion_data(VAR_U);

	while (!(UCSRA&(1<<UDRE))) {};
	UDR = point[0];
	while (!(UCSRA&(1<<UDRE))) {};
	UDR = digits[dist_thouthend];
	while (!(UCSRA&(1<<UDRE))) {};
	UDR = digits[dist_hundrets];
	while (!(UCSRA&(1<<UDRE))) {};
	UDR = digits[dist_tens];
	while (!(UCSRA&(1<<UDRE))) {};
	UDR = digits[dist_ones];
	while (!(UCSRA&(1<<UDRE))) {};
	UDR = next_string;
	while (!(UCSRA&(1<<UDRE))) {};
	UDR = new_line;

	func_check_threshold(VAR_U);

	VAR = 0;
}

// in this function send different var and return mas
void func_convertion_data(VAR_VAR)
{
	int data = 0;
	dist_thouthend=0;
	dist_hundrets=0;
	dist_tens=0;
	dist_ones=0;

	data=VAR_VAR;
	while(data>999)
	{	data -=1000; dist_thouthend++;}
	while(data>99)	
	{	data -=100;	dist_hundrets++;}
	while(data>9)	
	{	data -=10;dist_tens++;}
	dist_ones=data;	
}

void func_check_threshold(VAR_VAR_U)
{
	if (VAR_VAR_U < 2450)
	{	
		LED_threshold = end_of_discharge;
		PORTD |=(1<<Bat_relay);		
	}
	else
	{	
		LED_threshold = discharge;
		PORTD &= ~(1<<Bat_relay);
	}
	
}

/********************************************
???????????? ??????????? ???? ?????
**********************************************/

inline char is_bitf(byte bit)
{
	byte res;// ?????????? ?????????? ???????	
	if (bit< 32) //???? ??? ?????? 32 ???????????? ????????? ?????????? ??????
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			if ((flag0 & (1UL<<bit)) !=0)			
			res = 1;			
			else			
			res = 0;
		}		
	}	
	else
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			bit-=32;			
			if ((flag1 & (1UL<<bit)) !=0)			
			res = 1;			
			else
			res = 0;
		}		
	}	
	return res;// ????????? ????????? ???????	
}

/**********************************************
???????????? ?????? ???? ????????? ?????
**********************************************/


inline void clrbf ( byte bit)
{
	if (bit <32) //???? ??? ?????? 32 ???????????? ????????? ?????????? ??????
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			flag0&=(~(1UL << bit));
		}
		
	}
	
	else
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			bit-=32;
			flag1&=(~(1UL << bit));
		}
		
	}
	
}
/************************************************
???????????? ????????? ???? ???????? ??????
*************************************************/

inline void setbf ( byte bit)
{
	
	if (bit<32) //???? ??? ?????? 32 ???????????? ????????? ?????????? ??????
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			flag0|=(1UL << bit);
		}
		
	}
	
	else
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			bit-=32;
			flag1|=(1UL << bit);
		}	
	}	
}
