/*
 * motor.h
 *
 *  Created on: 8 Oct 2021
 *      Author: ahmed
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "std_types.h"

/*==============================================================
 * 							Definitions
 * ==============================================================*/
#define DcMotor_inputPort_ID	PORTC_ID
#define DcMotor_inputPin1_ID	PIN2_ID
#define DcMotor_inputPin2_ID	PIN3_ID

typedef enum
{
	DcMotor_RIGHT, DcMotor_LEFT, DcMotor_OFF
}DcMotor_State;

/*==============================================================
 * 							Function Prototypes
 * ==============================================================*/
void DcMotor_init(void);

void DcMotor_rotate(DcMotor_State state);

void DcMotor_rotateLeft();

void DcMotor_rotateRight();

void DcMotor_off();

#endif /* MOTOR_H_ */
