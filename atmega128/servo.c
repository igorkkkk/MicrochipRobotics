#include <io.h>
#include <mega128a.h>
#include <delay.h>
#include <mega128a.h>
#include <delay.h>

// servo and encoder
unsigned char angle = 0;
interrupt[EXT_INT0] void int0Isr(void) // определение угля для энкодера
{
	// Если прерывание по фронту
	if ((PIND & (1 << 1)) != 0)
	{
		// Настраиваем прерывание на срез
		EICRA = (1 << ISC01);
		// Если на второй ножке высокий уровень
		if (((PIND & (1 << 0)) != 0))
		{
			if (angle < 255)
			{
				angle++;
			}
			else
			{
				angle = 255;
			}
		}
		else
		{
			if (angle > 0)
			{
				angle--;
			}
			else
			{
				angle = 0;
			}
		}
	}
	else
	{
		// Если прерывание по срезу
		// Настраиваем прерывание на фронт
		EICRA = (1 << ISC01) | (1 << ISC00);
		// Если на второй ножке высокий уровень
		if ((PIND & (1 << 0)) != 0)
		{
			if (angle > 0)
			{
				angle--;
			}
			else
			{
				angle = 0;
			}
		}
		else
		{
			if (angle < 255)
			{
				angle++;
			}
			else
			{
				angle = 255;
			}
		}
	}
}
unsigned char readADC(unsigned char channel) //ацп 8 бит
{
	ADMUX = (1 << REFS0) | (1 << ADLAR) | channel;
	ADCSRA |= (1 << ADSC);
	while (!(ADCSRA & (1 << ADIF)))
		;
	ADCSRA |= (1 << ADIF);

	return ADCH;
}
void main(void)
{
	unsigned char value1, value2;
	DDRA = 0xFF;
	DDRB = (1 << 5) | (1 << 6);
	DDRC = 0xFF;
	ADCSRA = (1 << ADEN); //Ацп включить
	EIMSK = (1 << INT0);  //прерывание по энкодеру
	EICRA = (1 << ISC11) | (1 << ISC10);
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1); //таймер 50Гц
	TCCR1A |= (1 << WGM11);
	TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS11);
	ICR1 = 27648;
#asm("sei")             // глобальное разрешение прерываний
	EIMSK = 0b00000001; // разрешение прерывания
	EICRA = 0b00000010; // прерывание по срезу //прерывания для энкодера
	while (1)
	{
		value1 = readADC(3); // значение ацп от 0 до 255
		value2 = angle;      // значение угла от 0 до 255
							 //изменение положения сервомашинок
		OCR1A = 1400 + (value1 * 5);
		OCR1B = 2800 - (value2 * 5);
	}
}
