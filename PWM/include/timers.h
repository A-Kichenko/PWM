#ifndef TIMERS_H
#define TIMERS_H

#include <avr/interrupt.h>

enum clkTimer
{
NO_CLOCK,			//������ ��������
CLK_1,				//��� ������������ CLK/
CLK_8,				//� ������������� CLK/8
CLK_64,				//� ������������� CLK/64
CLK_256,			//� ������������� CLK/256
CLK_1024,			//� ������������� CLK/1024
CLK_EXT_FALLING,	//������� ������������ �� ����� �0 ��� �1 �� ���������� ������ 
CLK_EXT_RISING,		//������� ������������ �� ����� �0 ��� �1 �� ����������� ������
};

enum TimerEvent {
	NO_EVENT,
	EVENT_1,
	EVENT_2,
	};

void init_TIMER(void) //���������������� ������� T1
{
	TCCR0B |= CLK_64; //������������� ������������	
	OCR0A = 150; //������������� �������� �������� ��������� A ��� F_CPU=9600000 � CLK_64, ���������� ���������� ������ 1��
	TIMSK0 |= 1 << OCIE0A; //��������� ���������� �� ���������� � ��������� OCR1A
}

volatile uint32_t counter; //���������� - ������� ���������� ���������� ������� 0
		
class Timer
{
	private:
		
		uint32_t memCounter; //�������� ��� ���������� �������� �������� ���������� ��� ������� ����������
		uint32_t prevCounter;
		TimerEvent event;

	public:		

		Timer (void) //����������� �������� ������
		{
			memCounter = counter; //������� ���������� ��������� �������� ������ Timer_0
			prevCounter = 0;
			event = NO_EVENT;			
		}		
		
		static uint32_t getCounter(void) //����� ������� ���������� ���������� ������� 0 � ������� ������
		{
			TIMSK0 &= ~(1 << OCIE0A); //��������� ���������� �� ���������� � ��������� OCR1A
			uint32_t t = counter;
			TIMSK0 |= 1 << OCIE0A; //��������� ���������� �� ���������� � ��������� OCR1A
			return t;
		}
		
		TimerEvent countTwoPeriod(const uint32_t time1, const uint32_t time2)
		{
			if (counter != prevCounter)			
			{
				TIMSK0 &= ~(1 << OCIE0A); //��������� ���������� �� ���������� � ��������� OCR1A
				prevCounter = counter;
				if (counter - memCounter < time1)
				{					
					TIMSK0 |= 1 << OCIE0A; //��������� ���������� �� ���������� � ��������� OCR1A
					event = EVENT_1;					
				}
				else if ((counter - memCounter) < (time2 + time1)) //���� ������� ������� ����� time2					
				{
					TIMSK0 |= 1 << OCIE0A; //��������� ���������� �� ���������� � ��������� OCR1A
					event = EVENT_2;	//���� time1 > 0						 
				}
				else 
				{
					memCounter = counter;
					TIMSK0 |= 1 << OCIE0A; //��������� ���������� �� ���������� � ��������� OCR1A
					event = NO_EVENT;					
				}				
			}
			return event;			
		}
		
		uint8_t countInterval(uint8_t (*funcCallBack)(void), uint32_t time_) //����� �������� ������� ��������� ������ funcCallBack ������ time_tact ��
																		//� ����������� �������� ������� funcCallBack
		{
			if (counter != prevCounter)
			{				
				TIMSK0 &= ~(1 << OCIE0A); //��������� ���������� �� ���������� � ��������� OCR1A
				prevCounter = counter;
				if ((counter - memCounter) < time_)
				{
					TIMSK0 |= 1 << OCIE0A; //��������� ���������� �� ���������� � ��������� OCR1A
				}
				else
				{
					memCounter = counter;					
					TIMSK0 |= 1 << OCIE0A; //��������� ���������� �� ���������� � ��������� OCR1A
					return funcCallBack();
				}				
			}			
		}
		
		bool dellay_on(uint32_t time_delay, bool input = true) //����� �������� �� ���������
		{
			if (input) //���� �� ����� true
			{
				TIMSK0 &= ~(1 << OCIE0A); //��������� ���������� �� ���������� � ��������� OCR1A
				if ((counter - memCounter) < time_delay) //���� �������� �������� ������ time_delay
				{
					TIMSK0 |= 1 << OCIE0A; //��������� ���������� �� ���������� � ��������� OCR1A
					return false; //���������� false
				}
				else //���� �������� �������� �� ������ time_delay
				{
					TIMSK0 |= 1 << OCIE0A; //��������� ���������� �� ���������� � ��������� OCR1A
					return true; //���������� true					
				}
			}
			else
			{
				TIMSK0 &= ~(1 << OCIE0A); //��������� ���������� �� ���������� � ��������� OCR1A
				memCounter = counter;
				TIMSK0 |= 1 << OCIE0A; //��������� ���������� �� ���������� � ��������� OCR1A
				return false; //���������� false
			}
		}
		
		uint32_t getTimeInterval(void) //����� ���������� �������� ������� ����� �������� �������
		{
			uint32_t timeInterval_ = counter - memCounter;
			memCounter = counter;
			return timeInterval_;
		}
};
ISR(TIM0_COMPA_vect) // ��������� ���������� �������0 �� ���������� � A
{
	TCNT0 = 0; //�������� ������� �������� �������	
	counter ++; //����������� �������� counter �� 1 
}
#endif
