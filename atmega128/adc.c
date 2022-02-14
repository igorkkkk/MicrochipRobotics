#include <stdbool.h>
#include <mega128.h>
unsigned int readAdc(unsigned char channel)
{
	unsigned char lowBitValue;
	ADMUX = (1 << REFS0) | channel;                     // выбор опорного напряжения и порта
	ADCSRA |= (1 << ADSC);                              // начинаем преобразовывать аналоговый сигнал в
	цифровую форму while ((ADCSRA & (1 << ADIF)) == 0); // ждем, пока не завершится
	преобразование
	ADCSRA |= (1 << ADIF);              // процесс преобразования (один шаг)
	lowBitValue = ADCL;                 // младшие биты
	return ((ADCH << 8) | lowBitValue); //объединяем старшие биты с младшими
}
void main(void)
{
	DDRD = 0b11111111;

	ADCSRA = (1 << ADEN); //включаем питание ацп
	while (true)
	{
		PORTD = (0b11111111 >> (7 - readAdc(0) / 128)); // делим результат
преобразования на 128 (должно получиться число от 0 
до 7,
 // так как переменная
целочисленная, остаток не сохранится
 // определяем, сколько должно
остаться выключенных светодиодов
	}
}