// For AtMega8

//int VAR = 0;

#ifndef __define__
#define __define__

/*
PortB 8
*/
#define CD4066_2_D 0	//
#define CD4066_1_D 1	//
#define CD4066_1_A 2	//
//#define CD4051_C   3
//#define CD4066_1_D 4

/*
PortC 7
*/


/*
PortD 8
*/
#define LED_status 2	//
#define Bat_relay  3	//
#define CD4066_2_C 4	//
#define CD4066_1_C 5	//
#define CD4066_1_B 6	//
#define CD4066_2_A 7	//

/*
USART commands
*/
#define Symbol_0 48
#define Symbol_1 49
#define Symbol_2 50
#define Symbol_3 51
#define Symbol_4 52
#define Symbol_5 53
#define Symbol_6 54
#define Symbol_7 55
#define Symbol_8 56
#define Symbol_9 57
#define Symbol_10 58
#define Symbol_11 59


/*
#define Symbol_8 56
#define Symbol_9 57
#define Symbol_10 97	// a
#define Symbol_11 98	// b
#define Symbol_12 99	// c
#define Symbol_13 100	// c
#define Symbol_14 101	// c
#define Symbol_15 102	// c
*/

/*
Flags
*/
#define flag_open_CD4066_2_A_D  0
#define flag_open_CD4066_2_B_C  1
#define flag_end_ADC_conversion 2
#define flag_open_CD4066_1_A_D  3
#define flag_open_CD4066_1_B_C  4
#define flag_repite 5


#endif
