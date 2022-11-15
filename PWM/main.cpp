/*
 * PWM.cpp
 *
 * Created: 23.01.2022 9:53:56
 * Author : Kichenko Alexander
 */ 


#define F_CPU 9600000

#include <avr/io.h>
#include <avr/wdt.h> //���������� ���������� ��� ������ �� ���������� ��������
#include <avr/interrupt.h> //���������� ���������� ��� ������ � ������������
#include <stdlib.h> //���������� ���������� �������������� �������
#include <util/delay.h> //���������� ���������� ��������

#include "include/main.h"
#include "include/settings.h"
#include "timers.h"


/**********************************************************************/

//Timer TimerControlInput;
Timer TimerPWM;
uint8_t inputValue;
TimerEvent PWM_event;
void setupIO(void); //��������� ������� ������������� �������
uint8_t getADC(void); //��������� ������� ������������ ��������� ������������ �������������� �� �����  analogInput
void switchOnInput(void); //���������� ������� ��������� �������
void switchOffInput(void); //���������� ������� ���������� �������

int main(void)
{
	wdt_enable(WDTO_8S); //�������� ���������� ������ �� ������� ����� 8 �������
	setupIO(); //������������� �������
	init_TIMER(); //������������� �������
	_delay_ms(100); //�������� ����� ����������� ���������
	sei();	
	while(1) //�������� ����
	{
		wdt_reset(); //����� ����������� �������		
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
		uint32_t timeOn = PERIOD * (inputValue + (inputValue >> 7)) >> 8; //���������� ������� ����������� ��������� �������			
		uint32_t timeOff = PERIOD - timeOn; //���������� ������� ������������ ��������� �������			
		PWM_event = TimerPWM.countTwoPeriod(timeOn, timeOff);
	}
}

void setupIO(void) //������� ������������� ������, �������
{
	DDRB |= 1 << OUTPUT_LED; // ��������� OUTPUT_LED �������
	PORTB |= 1 << OUTPUT_LED;
	DDRB |= 1 << OUTPUT_PWM; // ��������� OUTPUT_PWM �������
	ADCSRA |= ADC_CLK << ADPS0; //������������� ������������ ������� ADC
	ADCSRA |= 1 << ADEN; //���������� ������ ���, ADEN=1 - ��� �������
	ADMUX |= INPUT_SIGNAL << MUX0; //��������� ADC0 ���������� ������
	ADMUX |= 1 << ADLAR; //������� ���������� ADCL � ADCH
}
uint8_t getADC(void) //������� ������������ ��������� ������������ �������������� �� ����� analogInput
{
	cli();
	_delay_ms(10); //�������� ��� ������������ ����������
	sei();	
	ADCSRA |= 1 << ADSC; //����� �������������� ���, ADSC=1 - �����
	while ((ADCSRA >> ADSC) & 1); //���� ���� ���������� ��������������	
	return ADCH; //���������� ��������� ��������������
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
