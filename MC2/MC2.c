/*
 * MC2.c
 *
 *  Created on: 24 Oct 2021
 *  Author: Ahmed Osama AbdulMaksoud
 */

#include "timer.h"
#include "uart.h"
#include "motor.h"
#include "buzzer.h"
#include "twi.h"
#include "external_eeprom.h"
#include <avr/io.h>
#include <util/delay.h>
/* [Description]: header file to store the definitions used in MC1 and MC2*/
#include "mc2_constants.h"


/* [Description]: Saving the received password from MC1*/
void Save_data(char *data);

/*[Description]: comparing the password received
 * from MC1 with the one saved in the EEPROM*/
uint8 check_data(char *compared_data);

/* [Description]: Turning the buzzer ON for a period then OFF*/
void alarm();

/* [Description]: Activates Timer1 for a specific period*/
void ActivateTimer1(uint16 period, void (*func_ptr)(void));

/* [Description]: will be used as the call back function for the timer
 * 				  opens the door by rotating the motor left and right*/
void UnlockDoor();

char val[6];
/* [usage]: will be used as the counter for the timer
 * 				  used in both call back functions*/
volatile uint8 ticks;
/* [usage]: will be used to store the characters that will
 * 			resemble the requests from the UART*/
volatile uint8 recievedValue;



int main(void)
{
	uint8 failed = 0;
	uint8 operation;

	/*Initializing The UART with :
	 * Baud rate : 9600
	 * One stop bit
	 * 8 bit data
	*/

	/*Enabling Interrupts*/
	SREG |= (1<<7);


	UART_Config config_u = {ONE_BIT_STOP, EIGHT_BIT_MODE, PARITY_OFF, 9600};
	UART_init(&config_u);




	/*Initializing the motor*/
	DcMotor_init();

	/*Initializing the Buzzer*/
	BUZZER_init();

	/*Initializing the I2C with
	 * bit rate of 100k
	 * address 1
	 * */
	twi_config I2C_config = {Btr_400k,0b00000010};
	TWI_init(&I2C_config);




	/*Receive the value of the password from MC1*/
	UART_receiveString(val);

	/*Save the Password received from MC1*/
	Save_data(val);

	for(;;)
	{
		/*Receives the password from MC1*/
		UART_receiveString(val);

		/*Checks if the password matches the one stored*/
		if(check_data(val))
		{
			/*Reset the number of failed attempts*/
			failed = 0;

			/*Sends to MC1 that the password was a match*/
			UART_sendByte(SUCCESS);
			/*Receives from MC1 the desired operation
			 * either to unlock the door or to change the password*/
			operation = UART_receiveByte();

			/*If the desired operation is to unlock the door*/
			if(operation == DOOR)
			{
				/*Reset the operation variable for the next iteration*/
				operation = 0;

				/*Starts timer1 with UnlockDoor as call back function
				 * and 8000 as the timer compare value which is 1 second*/
				ActivateTimer1(8000, UnlockDoor);

				/*After 17 seconds send a signal to MC1 that the door is unlocked
				 * and wait for the mentioned time before doing so*/
				while(ticks < 17);
				UART_sendByte(COMPLETE);

				/*After 33 seconds send a signal to MC1 that the process has finished */
				while(ticks != 33);
				UART_sendByte(COMPLETE);
			}
			else if(operation == CHANGE_PASS )
			{
				/*Reset the operation variable for the next iteration*/
				operation = 0;
				/*Receives the password from MC1*/
				UART_receiveString(val);
				/*Save the Password received from MC1*/
				Save_data(val);
				/*send a signal to MC1 that the process has finished*/
				UART_sendByte(COMPLETE);
			}
		}
		/*if the password doesn't match inform MC1 and increment the number of failed attempts */
		else
		{
			UART_sendByte(FAILURE);
			failed++;
		}

		/*if the number of failed attempts reaches 3 activate the alarm and reset the number*/
		if(failed == 3)
		{
			failed = 0;
			ActivateTimer1(8000, alarm);
			while(ticks != 10);
			/*Inform MC1 that the buzzer is OFF now*/
			UART_sendByte(COMPLETE);
		}
	}
}

void Save_data(char *data)
{
	uint8 i ;
	for(i = 0; i < 5; i++ )
	{
		EEPROM_writeByte(ADDRESS + i, data[i]);
		/*Delay required by the eeprom to store data*/
		_delay_ms(10);
	}
}

uint8 check_data(char *compared_data)
{

	uint8 rx_data[5];
	uint8 i ;
	/*read the stored password from the eeprom and compare it to the received one*/
	for(i = 0; i < 5; i++ )
	{
		EEPROM_readByte(ADDRESS + i, &rx_data[i]);
		if(rx_data[i] != compared_data[i])
			return 0;
	}
	return 1;
}


void ActivateTimer1(uint16 period, void (*func_ptr)(void))
{
	/*Reset the ticks every time the function is called*/
	ticks = 0;
	/*Initializing the timer with:
	 * timer: TIMER1
	 * mode: compare mode
	 * initial value: 0
	 * compare value: will be passed
	 * frequency: FCPU_1024
	 * */
	timer_config config = {TIMER1, CTC, 0, period, FCPU_1024 };
	/*set timer1's call back function*/
	TIMER_setCallBack(TIMER1, CTC, func_ptr);
	/*initialize timer1*/
	TIMER_init(&config);
}

void UnlockDoor()
{
	/*rotate the motor to the left for the first 15 seconds*/
	if(ticks == 0)
		DcMotor_rotateLeft();
	/*after 15 seconds turn the motor off for 3 seconds*/
	else if((ticks == 15))
		 DcMotor_off();
	/*rotate the motor right for 18 seconds*/
	else if(ticks == 18)
		 DcMotor_rotateRight();
	/*turn the motor off and de-initialize tiemr1*/
	else if (ticks == 33)
	{
		DcMotor_off();
		TIMER_deInit(TIMER1);
	}
	ticks++;
}

void alarm()
{
	/*turn the buzzer ON for 10 seconds*/
	if(ticks == 0)
		BUZZER_ON();
	/*turn the buzzer OFF and de-initialaize timer1*/
	if(ticks == 9)
	{
		BUZZER_OFF();
		TIMER_deInit(TIMER1);
	}
	ticks++;

}

