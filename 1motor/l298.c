#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRL=255;
	TCCR5A=(1<<COM5B1)|(1<<COM5A1)|(1<<WGM50);
	TCCR5B=(1<<WGM52)|(1<<CS51);
	OCR5B=0;
	OCR5A=0;
	PORTL=(1<<0);
	while(1)
	{
		
	}
}