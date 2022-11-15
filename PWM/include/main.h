#ifndef MAIN_H
#define MAIN_H



#include <avr/io.h>
#include <avr/wdt.h> //���������� ���������� ��� ������ �� ���������� ��������
#include <avr/interrupt.h>
#include <stdlib.h> //���������� ���������� �������������� �������
#include <util/delay.h> //���������� ���������� ��������


/*�������� ������� ��� ADC*/
#define	ADC_CLK2			0b001 //clk/2
#define	ADC_CLK4			0b010 //clk/4
#define	ADC_CLK8			0b011 //clk/8
#define	ADC_CLK16			0b100 //clk/16
#define	ADC_CLK32			0b101 //clk/32
#define	ADC_CLK64			0b110 //clk/64
#define	ADC_CLK128			0b111 //clk/128

#define ADC0				0 //���������� ���� 0
#define ADC1				1 //���������� ���� 1
#define ADC2				2 //���������� ���� 2
#define ADC3				3 //���������� ���� 3

/*�������� ������� ��� �������*/
#define	TIM_CLK1			0b001 //clk/1
#define	TIM_CLK8			0b010 //clk/8
#define	TIM_CLK64			0b011 //clk/64
#define	TIM_CLK256			0b100 //clk/256
#define	TIM_CLK1024			0b101 //clk/1024

#endif //MAIN_H