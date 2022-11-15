#ifndef SETTINGS_H
#define SETTINGS_H

#include "main.h"

/************************************* настройки *******************************/

#define OUTPUT_PWM				PB1 //выход для транзистора
#define OUTPUT_LED				PB0	 //выход для LEDа
#define INPUT_SIGNAL			ADC1 //аналоговый вход для сигнала
#define TIME_CONTROL_INPUT		1000//мс, интервал между измерениями сигнала на входе INPUT_SIGNAL
#define PERIOD					10000ul//мс, время цикла ШИМ

/************************************* настройки ADC ******************************************/

#define ADC_CLK				ADC_CLK128 //выбор делителя для ADC

#endif //SETTINGS_H