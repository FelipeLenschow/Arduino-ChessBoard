#define F_CPU 16000000UL 
#include <avr/io.h> 
#include <util/delay.h> 

#define NOP __asm__ __volatile__ ("nop\n\t")
#define tst_bit(Y,bit_x)(Y&(1<<bit_x))
void setLed();
void clearLed();
void LedShow();
void corLed(int Led, int red, int green, int blue);

int Leds[192];   //grb


int main(void)
{

	DDRB = 0xFF; 
	corLed(0,255,0,0);
	corLed(1,0,255,0);
	corLed(2,0,0,255);
	corLed(3,0,255,255);
	corLed(4,255,0,255);
	LedShow();
	
	while (1)
	{
		
		
	}
}

void LedShow(){
	for(int i = 0; i < 192; i++)
		for(int j = 0; j < 8; j++)
		   if(tst_bit(Leds[i], j)) setLed(); 
		   else clearLed();
}

void corLed(int Led, int red, int green, int blue){
	int AUX = Led * 3;
	Leds[AUX] = green;
	Leds[AUX+1] = red;
	Leds[AUX+2] = blue;
}

void setLed(){
	PORTB = 0xFF;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	PORTB = 0x00;
	NOP;
	NOP;
	NOP;
	NOP;
}

void clearLed(){
	PORTB = 0xFF;
	NOP;
	NOP;
	NOP;
	NOP;
	PORTB = 0x00;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
}