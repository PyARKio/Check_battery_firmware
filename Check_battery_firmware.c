/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
//////////  Stend for OAmp 1 phase
//////////
//////////

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#include "define.h"
#include "hardware.h"


char t[] = "T\r\n";
char bt[] = "B\r\n";
char mystr[]="\rAtmega8A UART ready!\r\n";
char flag_plus = 1;
int count = 0;

ISR(TIMER0_OVF_vect)
{
	func_timer0_ovf();
}

ISR(ADC_vect)
{
	func_ADC();
}

ISR(USART_RX_vect)
{
	//func_rx_commands(UDR);
}



int main(void)
{
	DDRB = 0b00011111	;
	PORTB = 0b11100000	;

	DDRD = 0b11111111;
	PORTD = 0b00000100;

	TCCR0 = (1<<CS02)|(0<<CS01)|(1<<CS00); // 1024 -> 256 -> 64 (101->100->011)
	TIMSK = (1<<TOIE0);
	TCCR1B = (0<<CS12)|(0<<CS11)|(1<<CS10)|(1<<WGM12); // 8 -> 1 (010->001)
	TCCR1A = (1<<COM1A1)|(1<<COM1B1)|(1<<WGM10)|(1<<WGM11); // використовуємо ножку OC0B(PD5) для виводу ШІМ-сигналу

	OCR1AH = 0;
	OCR1AL = 0;
	OCR1BH = 0;
	OCR1BL = 0;

//	TIMSK1 =(1<<TOIE1);
//	EICRA |=(1<<ISC11)|(1<<ISC10)|(1<<ISC01)|(1<<ISC00); // лише на зростаючий фронт
//	EIMSK |=(1<<INT1)|(1<<INT0);
	
	//UBRRH = 0;
	UBRRL = 12;

	//UCSRA = 0;
	UCSRB = 0b10011000;
	//UCSRC = 0b00000110; // UMSELn1/0 -> 00 -> Asynchronous USART

		// Опорна напруга - внутрішнє джерело 2,56 В
		// Перетворення виконувати з ADC5 вивода
	ADMUX=(1<<REFS0)|(0<<REFS1)|(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(1<<MUX0);  // |(1<<MUX2);
		// Вмикаємо АЦП
		// Дозволяємо переривання по закінченню перетворення
		// Робоча частота АЦП = Робоча частота МК / 16
	ADCSRA=(1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);

	for (char number_symbol=0; number_symbol<23; number_symbol++)
	{
		while (!(UCSRA&(1<<UDRE))) {};
		UDR = mystr[number_symbol];
	}

	clrbf(flag_end_ADC_conversion);
	clrbf(flag_repite);
	// VAR = 1024;

	sei();
	OCR1A = 0;
	OCR1B = 0;

	_delay_ms(100);
	ADMUX &= ~((1<<MUX0)|(1<<MUX1)|(1<<MUX2)|(1<<MUX3));
	_delay_ms(3);
	//ADMUX |= (1<<MUX2);
	ADCSRA |= (1<<ADSC); // ADC 0

	

	while(1)
	{
		_delay_ms(175);
		//for (char number_symbol=0; number_symbol<3; number_symbol++)
		//{
			//while (!(UCSRA&(1<<UDRE))) {};
			//UDR = t[number_symbol];
		//}

		if (is_bitf(flag_end_ADC_conversion))
		{
			//func_send_ADC_result();
			//clrbf(flag_end_ADC_conversion);
			// OCR1B = 1024 - VAR;
			setbf(flag_repite);
			clrbf(flag_end_ADC_conversion);
			func_send_ADC_result();

			for (char number_symbol=0; number_symbol<3; number_symbol++)
			{
				while (!(UCSRA&(1<<UDRE))) {};
				UDR = bt[number_symbol];
			}

		}


		if (is_bitf(flag_repite))
		{
			ADCSRA |= (1<<ADSC);
			clrbf(flag_repite);
		}

/*		if (count == 1000)
		{
			count = 0;
			if (OCR1B < 1020 && flag_plus == 1)
			{ 
				OCR1B += 1; 
			}

			else  
			{
				flag_plus = 0;
				if (OCR1B > 0 && flag_plus == 0)
				{ 
					OCR1B -= 1; 
				}
				else
				{ 
					flag_plus = 1;
				}
			}
		}
		else
		{ count++; }*/
	}
}
