/*
 * motor.c
 *
 *  Created on: 8 Oct 2021
 *      Author: Ahmed Osama AbdulMaksoud
 */

#include "motor.h"
#include "common_macros.h"
#include "gpio.h"


void DcMotor_init(void)
{

	/*Configure the pins as output*/
	GPIO_setupPinDirection(DcMotor_inputPort_ID, DcMotor_inputPin1_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(DcMotor_inputPort_ID, DcMotor_inputPin2_ID, PIN_OUTPUT);


	/*Motor is at rest initially*/
	GPIO_writePin(DcMotor_inputPort_ID, DcMotor_inputPin1_ID, LOGIC_LOW);
	GPIO_writePin(DcMotor_inputPort_ID, DcMotor_inputPin2_ID, LOGIC_LOW);
}

void DcMotor_rotate(DcMotor_State state)
{
	if(state == DcMotor_LEFT)
		GPIO_writePin(DcMotor_inputPort_ID, DcMotor_inputPin1_ID, LOGIC_HIGH);
	else if(state == DcMotor_RIGHT)
		GPIO_writePin(DcMotor_inputPort_ID, DcMotor_inputPin2_ID, LOGIC_HIGH);
	else if(state == DcMotor_OFF)
	{
		GPIO_writePin(DcMotor_inputPort_ID, DcMotor_inputPin1_ID, LOGIC_LOW);
		GPIO_writePin(DcMotor_inputPort_ID, DcMotor_inputPin2_ID, LOGIC_LOW);
	}
}

void DcMotor_rotateLeft()
{
	GPIO_writePin(DcMotor_inputPort_ID, DcMotor_inputPin2_ID, LOGIC_LOW);
	GPIO_writePin(DcMotor_inputPort_ID, DcMotor_inputPin1_ID, LOGIC_HIGH);
}

void DcMotor_rotateRight()
{
	GPIO_writePin(DcMotor_inputPort_ID, DcMotor_inputPin1_ID, LOGIC_LOW);
	GPIO_writePin(DcMotor_inputPort_ID, DcMotor_inputPin2_ID, LOGIC_HIGH);
}

void DcMotor_off()
{
	GPIO_writePin(DcMotor_inputPort_ID, DcMotor_inputPin1_ID, LOGIC_LOW);
	GPIO_writePin(DcMotor_inputPort_ID, DcMotor_inputPin2_ID, LOGIC_LOW);
}
