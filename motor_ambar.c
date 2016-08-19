#define F_CPU 14745600ul

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "r.h"

#define baud 38400
#define ACKN 98

long int count=0,max_count=0;
volatile char data_rxd_flag=0;
volatile unsigned int data;
volatile int runflag = 0;
volatile int flag=0,button;

void init_servo(void)
{ 
	TCCR5A=(1<<COM5B1)|(1<<WGM50);
	TCCR5B=(1<<WGM52)|(1<<CS51)|(1<<CS50);
	OCR5B=128;
}
void servo(int direction,long int angle)
{
	max_count=angle;
	if(direction==1)
	PORTL|=0b01000000;
	else if(direction==0)
	PORTL &= ~0b01000000;
}
void init_bot(void)
{
	DDRB=255;
    TCCR1A|=(1<<COM1A1)|(1<<WGM10);
	TCCR1B|=(1<<WGM12)|(1<<CS10)|(1<<CS12);
	TCCR2A|=(1<<COM2A1)|(1<<WGM20)|(1<<WGM21);
	TCCR2B|=(1<<CS20)|(1<<CS22);
	OCR2A=255;
	OCR1A=255;	
}
void init_usart(void)
{
	UBRR1=23;
	UCSR1B|=(1<<RXEN1)|(1<<TXEN1)|(1<<RXCIE1);
	UCSR1C|=(1<<UCSZ11)|(1<<UCSZ10);
}
void servo_right()
{
	DDRL=255;
	servo(1,1);
}
void servo_left()
{
	DDRL=255;
	servo(0,1);
}
void servo_stop()
{
	DDRL=0;
	servo(0,0);
}
void Move_Front()
{
	PORTB|=0b11110000;//7,6
}
void Move_Right()
{
	PORTB|=0b10110011;//7,0
}
void Move_Left()
{
	PORTB|=0b01110100;//6,2
}
void Move_Back()
{
	PORTB|=0b00110101;//0,2
}
void Move_Stop()
{
	PORTB=0b00000000;//all
}
void init_actuators_gripper(void)
{
	DDRH = 0xff;
	PORTH=0;
	DDRJ =0xff;
	PORTJ &= ~(1<<0); //J0 off
	
	
}
void actuator1_up(void)
{
	PORTH &= ~(1<<1);		//I1  off
	PORTH |= (1<<0)|(1<<3);		// I2 on, EN on (3 is enable)
	
}
void actuator1_down(void)
{
	PORTH &= ~(1<<0);		//I2 off
	PORTH |= (1<<1)|(1<<3);	//I1 ON, EN on
}
void actuator1_stop(void)
{
	PORTH &= ~(1<<0)|(1<<1)|(1<<3) ;	//I1 off, I2 off

}
void actuator2_up(void)
{
	PORTJ |= (1<<0);
	PORTH &= ~(1<<2);
	PORTH |=(1<<4);
	
}
void actuator2_down(void)
{
	PORTH |= (1<<2);
	PORTJ &= ~(1<<0);
	PORTH |=(1<<4);
	
}
void actuator2_stop(void)
{
	PORTJ &= ~(1<<0);
	PORTH &= ~(1<<2);
	PORTH &=~(1<<4);

}

ISR(USART1_RX_vect)
{
	 while(((UCSR1A &(1<<RXC1))!=0) && ((UCSR1A &(1<<UDRE1))==0));
	 data=UDR1;
	if(data==ACKN && flag==0)
	{
		flag=1;
	}
	else if(flag==1)
	{
		button=data;
		flag=0;
		data_rxd_flag=1;
	}
}
ISR(TIMER5_OVF_vect)
{
	count++;
	if (count==(max_count*10))
	{
		DDRL=0;
	}	
}
int main()
{
	cli();
	LCD_INIT(LCD_DISP_ON_CURSOR);
	init_servo();
	init_bot();
	init_usart();
	sei();
	DDRC=255;
	while(1)
	{   /*
		Move_Front();
		_delay_ms(10000);
		Move_Stop();
		_delay_ms(2000);
		Move_Back();
		_delay_ms(10000);
		Move_Stop();
		_delay_ms(2000);
		*/
		if(data_rxd_flag)
		{
			switch (button)
			{
				case 0:
				{
					LCD_HOME_POS();
					LCD_PRINT("Ready yo!!");
					_delay_ms(100);
					Move_Stop();
					servo_stop();
					actuator1_stop();
					actuator2_stop();
					data_rxd_flag=0;
					break;
				}
				case 1:
				{
					LCD_HOME_POS();
					LCD_PRINT("forward");
					_delay_ms(100);
					Move_Front();
					data_rxd_flag=0;
					break;
				}
				case 2:
				{
					LCD_HOME_POS();
					LCD_PRINT("backward");
					_delay_ms(100);
					Move_Back();
					data_rxd_flag=0;
					break;
				}
				case 3: 
				{
					LCD_HOME_POS();
					LCD_PRINT("right");
					_delay_ms(100);
					Move_Right();
					data_rxd_flag=0;
					break;
				}
				case 4:
				{
					LCD_HOME_POS();
					LCD_PRINT("left");
					_delay_ms(100);
					Move_Left();
					data_rxd_flag=0;
					break;
				}
				case 5: 
				{
					LCD_HOME_POS();
					LCD_PRINT("servo left");
					_delay_ms(100);
					servo_left();
					data_rxd_flag=0;
					break;
				}
				case 6: 
				{ 
					LCD_HOME_POS();
					LCD_PRINT("servo right");
					_delay_ms(100);
					servo_right();
					data_rxd_flag=0;
					break;
				}
				case 7:
				{
					
					break;
				}
				case 8:
				{
					break;
				}
				case 9:
				{
					LCD_HOME_POS();
					LCD_PRINT("Actuator 1 up");
					_delay_ms(100);
					actuator1_up();
					data_rxd_flag=0;
					break;
				}
				case 10:
				{
					LCD_HOME_POS();
					LCD_PRINT("Actuator 1 down");
					_delay_ms(100);
					actuator1_down();
					data_rxd_flag=0;
					break;
				}
				case 11:
				{
					LCD_HOME_POS();
					LCD_PRINT("Actuator 2 up");
					_delay_ms(100);
					actuator2_up();
					data_rxd_flag=0;
					break;
				}
				case 12:
				{
					LCD_HOME_POS();
					LCD_PRINT("Actuator 2 down");
					_delay_ms(100);
					actuator2_down();
					data_rxd_flag=0;
					break;
				}
				case 13:
				{
					break;
				}
				case 14:
				{
					break;
				}
			}				
		}
	}
	return 0;
}