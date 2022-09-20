/*
 * buzzer.c
 *
 *  Created on: 24 Oct 2021
 *      Author: ahmed
 */

#include "buzzer.h"
#include "gpio.h"

void BUZZER_init()
{
	/*Setup the Pin direction as output and make the initial output zero so the buzzer is'nt fired on*/
	GPIO_setupPinDirection(BUZ_PORT_ID, BUZ_PIN_ID, PIN_OUTPUT);
	GPIO_writePin(BUZ_PORT_ID, BUZ_PIN_ID, LOGIC_LOW);
}

void BUZZER_ON()
{
	/*The buzzer's pin is logic high so the buzzer fired on*/
	GPIO_writePin(BUZ_PORT_ID, BUZ_PIN_ID, LOGIC_HIGH);
}

void BUZZER_OFF()
{
	/*The buzzer's pin is logic low so the buzzer turned off*/
	GPIO_writePin(BUZ_PORT_ID, BUZ_PIN_ID, LOGIC_LOW);
}
