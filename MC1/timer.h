/*
 * timer.h
 *
 *  Created on: 24 Oct 2021
 *      Author: ahmed
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"



/*Definitions*/

typedef enum
{
	NORM, CTC, F_PWM
}timer_mode;
typedef enum
{
	NO_CLK, FCPU, FCPU_8, FCPU_64, FCPU_256, FCPU_1024
}timer_prescaler;
typedef enum
{
	TIMER0, TIMER1, TIMER2
}timer_id;

/*Configuration structure for the timers, contains:
 * 1. timer id: an enum which helps pick timer that will be used
 * 2. timer mode: an enum which helps select the mode
 * 3. initial value: the initial value that the counter will start counting from, made uint16 for timer1
 * 4. compare value: the value which represents the upper bound in case of compare mode (CTC)
 * 5. Timer pre-scaler: the frequency that'll be used by the clock*/
typedef struct
{
	timer_id timer;
	timer_mode mode;
	uint16 init_val;
	uint16 comp_val;
	timer_prescaler prescaler;
}timer_config;

/*Function Prototypes*/

/*function responsible for initializing a timer specified in the config structure with all its other details*/
void TIMER_init(timer_config* config);
/*function responsible for assigning the call back function of the timer*/
void TIMER_setCallBack(timer_id timer,timer_mode mode ,void (*func_ptr)(void));
/*function responsible for de-initializing a specific timer*/
void TIMER_deInit(timer_id timer);


#endif /* TIMER_H_ */
