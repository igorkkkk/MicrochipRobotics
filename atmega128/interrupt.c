#include <mega128.h>
#include <delay.h>
interrupt[EXT_INT0] void NameIsr(void)
{
	if ((PORTE & 0b00000001) == 0)
	{ //если светодиод не горит, включаем его
		PORTE = 0b00000001;
	}
	else
	{
		PORTE = 0b00000000;
	}
}
void main(void)
{
	DDRE = 0b00000001;
	PORTE = 0b00000001;

#asm("sei")				// глобальное разрешение прерываний
	EIMSK = 0b00000001; // разрешение прерывания
	EICRA = 0b00000010; // прерывание по срезу (когда напряжение падает, поэтому
кнопка методом pull-up (сигнал на входе - 5В)) 
while (1)
{
}
}