/*
 * lcd.h
 *
 * Created: 3/17/22 9:52:13 AM
 *  Author: TuanNguyen
 */ 


#ifndef LCD_H_
#define LCD_H_

#define F_CPU 8000000UL /* Define CPU frequency here 8MHz */
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#define LCD_RS PD6
#define LCD_RW PD5
#define LCD_EN PD7
#define degree_sysmbol 0xdf
#define LCD_D4 PC4
#define LCD_D5 PC5
#define LCD_D6 PC6
#define LCD_D7 PC7

#define ADC_VREF_TYPE 0x00

char buff[5];
int adc_value;
float nhietdo;

void ADC_Init(){
	DDRA = 0x00; /* Make ADC port as input */
	ADCSRA = 0x87; /* Enable ADC, with freq/128 */
	ADMUX = 0x40; /* Vref: Avcc, ADC channel: 0 */
}

int ADC_Read(char channel)
{
	ADMUX = 0x40 | (channel & 0x07); /* set input channel to read */
	ADCSRA |= (1<<ADSC); /* Start ADC conversion */
	while (!(ADCSRA & (1<<ADIF))); /* Wait until end of conversion by polling
	ADC interrupt flag */
	ADCSRA |= (1<<ADIF); /* Clear interrupt flag */ /* set the ADIF
	bit of ADCSRA register */
	_delay_ms(1); /* Wait a little bit */
	return ADCW; /* Return ADC word */
}
	
//ham tao xung tren chan EN de LCD nhan du lieu
void LCD_Enable(void)
{
	PORTD |=(0x01<<LCD_EN);
	_delay_us(3);
	PORTD &=~(0x01<<LCD_EN);
	_delay_us(50);
}

//Ham Gui 4 Bit Du Lieu Ra LCD
void LCD_Send4Bit(unsigned char Data)
{
	( Data &1)?(PORTC |=(1<<LCD_D4)):(PORTC &=~(0x01<<LCD_D4));
	((Data>>1)&1)?(PORTC |=(1<<LCD_D5)):(PORTC &=~(0x01<<LCD_D5));
	((Data>>2)&1)?(PORTC |=(1<<LCD_D6)):(PORTC &=~(0x01<<LCD_D6));
	((Data>>3)&1)?(PORTC |=(1<<LCD_D7)):(PORTC &=~(0x01<<LCD_D7));
}
	
// Ham Gui 1 Lenh Cho LCD
void LCD_SendCommand(unsigned char command)
{
	LCD_Send4Bit((unsigned char)command >>4);/* Gui 4 bit cao */
	LCD_Enable();
	LCD_Send4Bit(command); /* Gui 4 bit thap*/
	LCD_Enable();
}
	
// Ham Xoa Man Hinh LCD
void LCD_Clear()
{
	LCD_SendCommand(0x01);
	_delay_ms(2);
}
	
//bat con tro, nhap nhay con tro
void LCD_BlinkPointer()
{
	LCD_SendCommand(0x0E);
}
	
//tat con tro, nhap nhay con tro
void LCD_BlinkPointerPlus()
{
	LCD_SendCommand(0x0F);
}
	
// Ham Khoi Tao LCD
void LCD_Init()
{
	LCD_Send4Bit(0x00); //turn on LCD
	_delay_ms(20);
	PORTD &=~(0x01<<LCD_RS); //ghi vao IR
	PORTD &=~(0x01<<LCD_RW);
	LCD_Send4Bit(0x03); //function setting
	LCD_Enable();
	_delay_ms(5);
	LCD_Enable();
	_delay_us(100);
	LCD_Enable();
	LCD_Send4Bit(0x02); //di chuyen con tro ve dau man hnh
	LCD_Enable();
	LCD_SendCommand( 0x28 ); //lua chon che do 4 bit
	LCD_SendCommand( 0x0c); // bat hien thi va tat con tro di
	LCD_SendCommand( 0x06 ); // tu dong di chuyen con tro
	LCD_Clear();
}
	
// set toa do hien thi
void LCD_GotoXY(unsigned char x, unsigned char y)
{
	unsigned char address;
	if(!y)address=(0x80+x);
	else address=(0xc0+x);
	LCD_SendCommand(address);
}

void LCD_PutChar(unsigned char Data)
{
	PORTD |=(0x01<<LCD_RS); //keo RS len 1 de ghi vao DR
	LCD_SendCommand(Data);
	PORTD &=~(0x01<<LCD_RS);
}

void LCD_Puts(char *s)
{
	while (*s)
	{
		LCD_PutChar(*s);
		s++;
	}
}




#endif /* LCD_H_ */