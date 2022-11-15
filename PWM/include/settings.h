#ifndef SETTINGS_H
#define SETTINGS_H

#include "main.h"

/************************************* ��������� *******************************/

#define OUTPUT_PWM				PB1 //����� ��� �����������
#define OUTPUT_LED				PB0	 //����� ��� LED�
#define INPUT_SIGNAL			ADC1 //���������� ���� ��� �������
#define TIME_CONTROL_INPUT		1000//��, �������� ����� ����������� ������� �� ����� INPUT_SIGNAL
#define PERIOD					10000ul//��, ����� ����� ���

/************************************* ��������� ADC ******************************************/

#define ADC_CLK				ADC_CLK128 //����� �������� ��� ADC

#endif //SETTINGS_H