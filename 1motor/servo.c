#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRL=255;
	TCCR5A=(1<<COM5B1)|(1<<WGM50);
	TCCR5B=(1<<WGM52)|(1<<CS51);
	OCR5B=128;
	PORTL|=0b01000000;
	while(1)
	{
		
	}
}