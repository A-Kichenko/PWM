#ifndef TIMERS_H
#define TIMERS_H

#include <avr/interrupt.h>

enum clkTimer
{
NO_CLOCK,			//таймер выключен
CLK_1,				//без предделителя CLK/
CLK_8,				//с предделителем CLK/8
CLK_64,				//с предделителем CLK/64
CLK_256,			//с предделителем CLK/256
CLK_1024,			//с предделителем CLK/1024
CLK_EXT_FALLING,	//внешнее тактирование на входе Т0 или Т1 по спадающему фронту 
CLK_EXT_RISING,		//внешнее тактирование на входе Т0 или Т1 по восходящему фронту
};

enum TimerEvent {
	NO_EVENT,
	EVENT_1,
	EVENT_2,
	};

void init_TIMER(void) //конфигурирование таймера T1
{
	TCCR0B |= CLK_64; //устанавливаем предделитель	
	OCR0A = 150; //устанавливаем значение регистра сравнения A при F_CPU=9600000 и CLK_64, совпадение происходит каждую 1мс
	TIMSK0 |= 1 << OCIE0A; //разрешаем прерывания по совпадению с регистром OCR1A
}

volatile uint32_t counter; //переменная - счетчик количества прерываний таймера 0
		
class Timer
{
	private:
		
		uint32_t memCounter; //свойство для сохранения значений счетчика прерываний для отсчета интервалов
		uint32_t prevCounter;
		TimerEvent event;

	public:		

		Timer (void) //конструктор объектов класса
		{
			memCounter = counter; //счетчик количества созданных объектов класса Timer_0
			prevCounter = 0;
			event = NO_EVENT;			
		}		
		
		static uint32_t getCounter(void) //метод выводит количество прерываний таймера 0 с момента старта
		{
			TIMSK0 &= ~(1 << OCIE0A); //запрещаем прерывания по совпадению с регистром OCR1A
			uint32_t t = counter;
			TIMSK0 |= 1 << OCIE0A; //разрешаем прерывания по совпадению с регистром OCR1A
			return t;
		}
		
		TimerEvent countTwoPeriod(const uint32_t time1, const uint32_t time2)
		{
			if (counter != prevCounter)			
			{
				TIMSK0 &= ~(1 << OCIE0A); //запрещаем прерывания по совпадению с регистром OCR1A
				prevCounter = counter;
				if (counter - memCounter < time1)
				{					
					TIMSK0 |= 1 << OCIE0A; //разрешаем прерывания по совпадению с регистром OCR1A
					event = EVENT_1;					
				}
				else if ((counter - memCounter) < (time2 + time1)) //если счетчик времени равен time2					
				{
					TIMSK0 |= 1 << OCIE0A; //разрешаем прерывания по совпадению с регистром OCR1A
					event = EVENT_2;	//если time1 > 0						 
				}
				else 
				{
					memCounter = counter;
					TIMSK0 |= 1 << OCIE0A; //разрешаем прерывания по совпадению с регистром OCR1A
					event = NO_EVENT;					
				}				
			}
			return event;			
		}
		
		uint8_t countInterval(uint8_t (*funcCallBack)(void), uint32_t time_) //метод вызывает функция обратного вызова funcCallBack каждые time_tact мс
																		//и возрвращает значение функции funcCallBack
		{
			if (counter != prevCounter)
			{				
				TIMSK0 &= ~(1 << OCIE0A); //запрещаем прерывания по совпадению с регистром OCR1A
				prevCounter = counter;
				if ((counter - memCounter) < time_)
				{
					TIMSK0 |= 1 << OCIE0A; //разрешаем прерывания по совпадению с регистром OCR1A
				}
				else
				{
					memCounter = counter;					
					TIMSK0 |= 1 << OCIE0A; //разрешаем прерывания по совпадению с регистром OCR1A
					return funcCallBack();
				}				
			}			
		}
		
		bool dellay_on(uint32_t time_delay, bool input = true) //метод задержки на включение
		{
			if (input) //если на входе true
			{
				TIMSK0 &= ~(1 << OCIE0A); //запрещаем прерывания по совпадению с регистром OCR1A
				if ((counter - memCounter) < time_delay) //если значение счетчика меньше time_delay
				{
					TIMSK0 |= 1 << OCIE0A; //разрешаем прерывания по совпадению с регистром OCR1A
					return false; //возвращаем false
				}
				else //если значение счетчика не меньше time_delay
				{
					TIMSK0 |= 1 << OCIE0A; //разрешаем прерывания по совпадению с регистром OCR1A
					return true; //возвращаем true					
				}
			}
			else
			{
				TIMSK0 &= ~(1 << OCIE0A); //запрещаем прерывания по совпадению с регистром OCR1A
				memCounter = counter;
				TIMSK0 |= 1 << OCIE0A; //разрешаем прерывания по совпадению с регистром OCR1A
				return false; //возвращаем false
			}
		}
		
		uint32_t getTimeInterval(void) //метод возвращает интервал времени между вызовами функции
		{
			uint32_t timeInterval_ = counter - memCounter;
			memCounter = counter;
			return timeInterval_;
		}
};
ISR(TIM0_COMPA_vect) // обработка прерываний таймера0 по совпадению с A
{
	TCNT0 = 0; //обнуляем счетный регистра таймера	
	counter ++; //увеличиваем значение counter на 1 
}
#endif
