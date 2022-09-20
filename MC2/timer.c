/*
 * timer.c
 *
 *  Created on: 24 Oct 2021
 *      Author: Ahmed Osama AbdulMaksoud
 */

#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/*six interrupts, divided so
 * 					 ->Compare mode
 * 			Timer0 ->
 *		   | 		 ->OverFlow mode
 *		   |
 * 		   |		->Compare mode
 * Timers ->Timer1
 * 		   |		->OverFlow mode
 * 		   |
 * 		   |		->Compare mode
 * 			Timer2->
 * 					->OverFlow mode
 * */

static volatile void(*OVF0_ptr)(void) = NULL_PTR;
static volatile void(*CMP0_ptr)(void) = NULL_PTR;
static volatile void(*OVF1_ptr)(void) = NULL_PTR;
static volatile void(*CMP1_ptr)(void) = NULL_PTR;
static volatile void(*OVF2_ptr)(void) = NULL_PTR;
static volatile void(*CMP2_ptr)(void) = NULL_PTR;

void TIMER_init(timer_config* config)
{
	/* Configure the timer control register
	 * 1. choose the desired timer
	 * 2. set the initial value from TCNT register
	 * 3. set the compare value from OCR register
	 * 4. select the timer type from TCCR register
	 * 5. set the pre-scalers value from TCCR register
	 * 6. enable the timer's interrupt  whether the overflow or the compare one
	 */
	if(TIMER0 == config->timer)
	{
		TCNT0 = config->init_val;
		OCR0 = config->comp_val;
		TCCR0 = (1 << FOC0) | (config->mode <<3) | (config->prescaler);
		/*we add one to avoid adding any conditions to check
		 * OVF 00 + 1 = 01
		 * CMP 01 + 1 = 10 */
		TIMSK |= (config->mode + 1);
	}

	else if(TIMER1 == config->timer)
	{
		TCNT1 = config->init_val;
		OCR1A = config->comp_val;
		TCCR1A = (1 << FOC1A);
		TCCR1B = (config->mode <<3) | (config->prescaler);
		//find a better way
		if(config->mode & 1)
			TIMSK |= (1 << OCIE1A);
		else
			TIMSK |= (1 << TOIE1);

	}
	else if(TIMER2 == config->timer)
	{
		TCNT2 = config->init_val;
		OCR2 = config->comp_val;
		TCCR2 |= (FOC2) | (config->mode << 3) | (config->prescaler);
		/*we add one to avoid adding any conditions to check
		 * OVF 00 + 1 = 01
		 * CMP 01 + 1 = 10
		 * shifted by 6 as the TIMSK register is different in timer2*/
		TIMSK |= ((config->mode + 1) << 6);
	}
}

/*Function used to set the call back function that will be used later by the ISRS*/
void TIMER_setCallBack(timer_id timer,timer_mode mode ,void (*func_ptr)(void))
{
	/*Check for each timer then check the mode to assign the correct call back function for each*/
	switch(timer)
	{
	case 0:
		if(NORM == mode)
			OVF0_ptr = func_ptr;
		else
			CMP0_ptr = func_ptr;
		break;
	case 1:
		if(NORM == mode)
			OVF1_ptr = func_ptr;
		else
			CMP1_ptr = func_ptr;
		break;
	case 2:
		if(NORM == mode)
			OVF2_ptr = func_ptr;
		else
			CMP2_ptr = func_ptr;
		break;
	}
}

void TIMER_deInit(timer_id timer)
{
	if(TIMER0 == timer)
		TIMSK &= 0xFC;

	else if(TIMER1 == timer)
		TIMSK &= 0xC3;

	else if(TIMER2 == timer)
		TIMSK &= 0x3F;
}

/*six ISRs as each timer has two ISRs for overflow and compare modes*/

ISR(TIMER0_COMP_vect)
{
	if(CMP0_ptr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*CMP0_ptr)();
	}
}

ISR(TIMER0_OVF_vect)
{
	if(OVF0_ptr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*OVF0_ptr)();
	}
}

ISR(TIMER1_COMPA_vect)
{
	if(CMP1_ptr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*CMP1_ptr)();
	}
}

ISR(TIMER1_OVF_vect)
{
	if(OVF1_ptr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*OVF1_ptr)();
	}
}

ISR(TIMER2_COMP_vect)
{
	if(CMP2_ptr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*CMP2_ptr)();
	}
}

ISR(TIMER2_OVF_vect)
{
	OVF2_ptr();
}
