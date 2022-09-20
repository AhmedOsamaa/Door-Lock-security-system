/*
 * buzzer.h
 *
 *  Created on: 24 Oct 2021
 *      Author: ahmed
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include "std_types.h"

/*Definitions*/
#define BUZ_PORT_ID	PORTD_ID
#define BUZ_PIN_ID	PIN4_ID

/*Function Prototypes*/
void BUZZER_init(); /*initialize the buzzer pins/ports*/
void BUZZER_ON();	/*turn the buzzer on*/
void BUZZER_OFF();	/*turn the buzzer off*/



#endif /* BUZZER_H_ */
