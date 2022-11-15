/*
 * PWM.cpp
 *
 * Created: 23.01.2022 9:53:56
 * Author : Kichenko Alexander
 */ 


#define F_CPU 9600000

#include <avr/io.h>
#include <avr/wdt.h> //подключаем библиотеку для работы со сторожевым таймером
#include <avr/interrupt.h> //подключаем библиотеку для работы с прерываниями
#include <stdlib.h> //подключаем библиотеку математических функций
#include <util/delay.h> //подключаем библиотеку задержек

#include "include/main.h"
#include "include/settings.h"
#include "timers.h"


/**********************************************************************/

//Timer TimerControlInput;
Timer TimerPWM;
uint8_t inputValue;
TimerEvent PWM_event;
void setupIO(void); //объявляем функцию инициализации выходов
uint8_t getADC(void); //объявляем функцию возвращающую результат однократного преобразования на входе  analogInput
void switchOnInput(void); //объявление функции включения выходов
void switchOffInput(void); //объявление функции выключения выходов

int main(void)
{
	wdt_enable(WDTO_8S); //включаем сторожевой таймер со сбросом через 8 секунды
	setupIO(); //инициализации выходов
	init_TIMER(); //инициализации таймера
	_delay_ms(100); //задержка перед выполнением программы
	sei();	
	while(1) //основной цикл
	{
		wdt_reset(); //сброс сторожевого таймера		
		switch (PWM_event)
		{
			case NO_EVENT:
			inputValue = getADC();
			break;
			case EVENT_1:
			switchOnInput();
			break;
			case EVENT_2:
			switchOffInput();
			break;
			
		}
		uint32_t timeOn = PERIOD * (inputValue + (inputValue >> 7)) >> 8; //вычисление времени включенного состояния выходов			
		uint32_t timeOff = PERIOD - timeOn; //вычисление времени выключенного состояния выходов			
		PWM_event = TimerPWM.countTwoPeriod(timeOn, timeOff);
	}
}

void setupIO(void) //функция инициализации входов, выходов
{
	DDRB |= 1 << OUTPUT_LED; // назначаем OUTPUT_LED выходом
	PORTB |= 1 << OUTPUT_LED;
	DDRB |= 1 << OUTPUT_PWM; // назначаем OUTPUT_PWM выходом
	ADCSRA |= ADC_CLK << ADPS0; //устанавливаем предделитель частоты ADC
	ADCSRA |= 1 << ADEN; //разрешение работы АЦП, ADEN=1 - АЦП включен
	ADMUX |= INPUT_SIGNAL << MUX0; //назначаем ADC0 аналоговым входом
	ADMUX |= 1 << ADLAR; //порядок следования ADCL и ADCH
}
uint8_t getADC(void) //функция возвращающая результат однократного преобразования на входе analogInput
{
	cli();
	_delay_ms(10); //задержка для стабилизации напряжения
	sei();	
	ADCSRA |= 1 << ADSC; //старт преобразований АЦП, ADSC=1 - старт
	while ((ADCSRA >> ADSC) & 1); //ждем пока закончится преобразование	
	return ADCH; //возвращаем результат преобразования
}
void switchOnInput(void)
{
	PORTB |= 1 << OUTPUT_PWM;
	PORTB &= ~(1 << OUTPUT_LED);
}
	
void switchOffInput(void)
{
	PORTB &= ~(1 << OUTPUT_PWM);
	PORTB |= 1 << OUTPUT_LED;
}
