#include <mega128a.h>
#include <delay.h>
#include <mega128a.h>
#include <delay.h>
// 8 бит
unsigned char readAdc(unsigned char channel)
{
	ADMUX = (1 << REFS0) | (1 << ADLAR) | channel;
	ADCSRA |= (1 << ADSC);
	while (!(ADCSRA & (1 << ADIF)));
	ADCSRA |= (1 << ADIF);

	return ADCH;
}
//переменные
int field;		   // «начение с датчика Холла
unsigned char ref; // Задание (потенциометр на третьей ноге)
int error = 0;	   // Ошибка управления
int control;	   // Сигнал управления
int integral;	   // интегральная составляющая
int diff = 0;
int prevField = 0;

void main()
{
	// Инициализация портов ввода/вывода
	DDRB = 0b00100000;												   //
	DDRG = 0b00001000;												   //
																	   // Инициализация ацп
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //предделитель на 128
																	   // Инициализация таймера 1
																	   // Быстрая ШИМ 8 бит
	TCCR1A = (1 << COM1A1) | (1 << WGM10);
	TCCR1B = (1 << WGM12) | (1 << CS10);
	while (1)
	{
		ref = (readAdc(3) + readAdc(3) + readAdc(3) + readAdc(3) + readAdc(3)) / 5;
		if (ref >= 45)
		{ //ограничение задани€
			ref = 45;
		}
		field = (readAdc(1) + readAdc(1) + readAdc(1) + readAdc(1) + readAdc(1)) / 5; //значение с
		датчика Холла
			diff = prevField - field; // находим разницу с предыдущим значением
		(Дифференциальная составляющая)
			prevField = field;
		error = ref - field; // ошибка управления для 8 бит ацп
		integral += error;	 // чем больше ки тем больше перерегулирование
							 //сигнал управления ПИД-регулятора
		control = integral / 600 + 200 + error * 2 + diff * 100;
		if (control > 255)
		{ //ограничение сигнала управления сверху
			control = 255;
		}
		if (control < -255)
		{ //ограничение сигнала управления снизу
			control = -255;
		}
		if (control >= 0)
		{						//установка сигнала управления
			PORTG &= ~(1 << 3); // устанавливаем направление магнитного поля
			OCR1AL = (unsigned char)control;
		}
		else
		{
			PORTG |= (1 << 3); // устанавливаем направление магнитного поля
			OCR1AL = (unsigned char)(-control);
		}
	}
}