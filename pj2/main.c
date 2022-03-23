/*
 * pj2.c
 *
 * Created: 3/15/22 7:34:01 AM
 * Author : TuanNguyen
 */ 

#define FRE 8000000UL
#define F_CPU 8000000UL /* Define CPU frequency here 8MHz */
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#include <funcs.h>
#include <lcd.h>
#include <leddon.h>
#include <led7seg.h>

unsigned char button = 0;

int main(void)
{
	//Khai bao trang thai output cho 8 chan led det
	DDRD |= 0xFF;
	PORTD |= 0x00;
	//LED 7 thanh
	DDRC |= 0xFF;
	PORTC |= 0x00;
	//Khoi tao cho chan ADC
	DDRA = 0x00; /* Make ADC port as input */
	ADCSRA = 0x87; /* Enable ADC, with freq/128 */
	ADMUX = 0x40; /* Vref: Avcc, ADC channel: 0 */		

	while (1)
	{
		button = buttCheck();
		switch (button)
		{
			case 1:
				_leddon();	
			break;
			case 2:
				_7seg();
			break;
			case 3:
				LCD_Init();
				//_delay_ms(3500);
				ADC_Init();
				while (1)
				{
					adc_value = ADC_Read(7);
					nhietdo = adc_value*0.4888; // nhietdo = adc_value*500/1023
					if (nhietdo>30)
					{
						LCD_Clear();
						LCD_GotoXY(0,0);
						LCD_Puts("Temperature: ");
						LCD_GotoXY(12,0);
						sprintf(buff,"%d%cC ", (int)nhietdo, degree_sysmbol);
						LCD_Puts(buff);
						LCD_GotoXY(1,1);
						LCD_Puts("It is very hot");
					}
					if ((nhietdo<30)&&(nhietdo>20))
					{
						LCD_Clear();
						LCD_GotoXY(0,0);
						LCD_Puts("Temperature: ");
						LCD_GotoXY(12,0);
						sprintf(buff,"%d%cC ", (int)nhietdo, degree_sysmbol);
						LCD_Puts(buff);
						LCD_GotoXY(0,1);
						LCD_Puts("It's comfortable");
					}
					if (nhietdo<20)
					{
						LCD_Clear();
						LCD_GotoXY(0,0);
						LCD_Puts("Temperature: ");
						LCD_GotoXY(12,0);
						sprintf(buff,"%d%cC ", (int)nhietdo, degree_sysmbol);
						LCD_Puts(buff);
						LCD_GotoXY(1,1);
						LCD_Puts("It is cold now");
					}
					_delay_ms(2000);
				}				
			break;
			case 4:
			break;
			default:
				LCD_Init();
				LCD_GotoXY(1,0);
				LCD_Puts("NguyenDinhTuan");
				LCD_GotoXY(2,1);
				LCD_Puts("MSSV 20182861");
		}		
	}
	return 0;
}