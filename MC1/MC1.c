/*
 * MC1.c
 *
 *  Created on: 24 Oct 2021
 *  Author: Ahmed Osama AbdulMaksoud
 */


#include "timer.h"
#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include <avr/io.h>
#include <util/delay.h>
/* [Description]: header file to store the definitions used in MC1 and MC2*/
#include "mc1_constants.h"

char pass[7] = {0};


/* [Description]: Display the Options for the user on the LCD */
void DisplayMenue(void);

/* [Description]: Display on the LCD the door opening process*/
void OpenDoor();



/* [Description]: Take the password from the user as input
 * 				  Works in either ways; for a new entry and for
 * 				  password confirmation
 * 				  NEW_ENTRY : for a new password
 * 				  PASS_CONF : for */
void GetPass(uint8 loop);

/* [Description]: manages the password changing process by sending the signals
 * 				  and displaying confirmation message*/
void ChangePass();

/* [Description]: Checks if the password entered by the user matches the
 * 				  original one, if so return 1 or else return 0*/
uint8 CheckPass();

/* [Description]: waits for the user after entering a password
 * 				  to confirm by pressing ON*/
void WaitForEnter();


int main(void)
{
	/*Stores the user's input*/
	uint8 choice = 0;

	/*enabling interrupts*/
	SREG |= (1<<7);

	/*Initialize the LCD*/
	LCD_init();

	/*Initializing The UART with :
	 * Baud rate : 9600
	 * One stop bit
	 * 8 bit data
	*/
	UART_Config config = {ONE_BIT_STOP, EIGHT_BIT_MODE, PARITY_OFF, 9600};
	UART_init(&config);

	/*get a new password entry*/
	GetPass(NEW_ENTRY);
	/*send the password to MC2 to be saved in the eeprom*/
	UART_sendString(pass);

	for(;;)
	{
		DisplayMenue();
		/*wait for a choice to be made
		 * whether to change password '-'
		 * or to unlock the door '+'*/
		do
		{
			choice = KEYPAD_getPressedKey();
		}while(choice != '+' && choice != '-');
		if(choice == '+')
		{
			/*Reset the choice for the next entry*/
			choice = 0;
			if(CheckPass())
			{
				OpenDoor();
			}

		}
		else if(choice == '-')
		{
			/*Reset the choice for the next entry*/
			choice = 0;
			if(CheckPass())
			{
				ChangePass();
			}

		}
	}
}

void DisplayMenue(void)
{
	LCD_displayOnScreen("+ : Open Door","- : Change Pass");
}

void OpenDoor()
{
	/*Send to MC2 a macro for the desired process*/
	UART_sendByte(DOOR);
	LCD_displayOnScreen("Correct Password","Door opening..");
	/*Wait for MC2 to send that it's done with the process*/
	while(UART_receiveByte() != COMPLETE);
	LCD_displayOnScreen("Correct Password","Door Closing..");
	/*Wait for MC2 to send that it's done with the process*/
	while(UART_receiveByte() != COMPLETE);
}


uint8 CheckPass()
{
	uint8 failure = 0;
	uint8 reply;
	for(;;)
	{
		GetPass(PASS_CONF);
		LCD_displayOnScreen("Checking Password...","");
		/*Delay for the message to be displayed*/
		_delay_ms(1000);
		/*Send to MC2 the password*/
		UART_sendString(pass);
		/*Receive a reply from the MC2*/
		reply = UART_receiveByte();

		if(reply== SUCCESS)
		{
			return 1;
		}

		else if(reply == FAILURE)
		{
			failure++;
			if(failure == 3)
			{
				failure = 0;
				/*display error message whilst waiting for MC2 to send when the
				 * alarm is turned off*/
				LCD_displayOnScreen("ALARM ACTIVATED","status: CALAMITOUS");
				while(UART_receiveByte() != COMPLETE);
				return 0;
			}
			else
			{
				LCD_displayOnScreen("Incorrect Password!","Try again");
				_delay_ms(1000);
			}

		}
		else
		{
			LCD_displayOnScreen("UART didn't send","Try again");
			_delay_ms(1000);
		}


	}
	return 0;
}



void ChangePass()
{
	/*Send to MC2 a macro for the desired process: change the password*/
	UART_sendByte(CHANGE_PASS);
	/*Ask for a new password entry*/
	GetPass(NEW_ENTRY);
	/*send the password to the eeprom to be saved*/
	UART_sendString(pass);

	LCD_displayOnScreen("Changing Password..","");
	_delay_ms(1000);
	if(UART_receiveByte() == COMPLETE)
		LCD_displayOnScreen("Your password has been changed","");
	_delay_ms(2000);
}
void GetPass(uint8 loop)
{
	uint8 correct = 0;
	/*Add to the string '#' so the UART knows where to cut the message*/
	pass[5] = '#';
	pass[6] = '\0';
	LCD_displayOnScreen("Enter a password of 5 characters","");
	/*the loop wil go on until the user enters the password right twice*/
	do
	{
		/*Reset the number of correct entries each time*/
		correct = 0;
		/*ask the user to enter the password*/

		for(uint8 i = 0; i < loop  ; i++)
		{
			/*Display message according to the process*/
			if(i == 0 && loop == NEW_ENTRY)
				LCD_displayOnScreen("Enter a password of 5 characters","");

			if(i == 0 && loop == PASS_CONF)
				LCD_displayOnScreen("Enter your Password to Continue","");

			else if(i == SIZE )
				LCD_displayOnScreen("Confirm your password","");



			uint8 button =  KEYPAD_getPressedKey();

			/*first take the password from the user as input*/
			if(i < SIZE && (button >=0 && button <=9 ) )
			{
				pass[i] = button;
				LCD_displayCharacter('*');
				/*After the user Enter the password, wait for ON*/
				if(i + 1 == SIZE)
					WaitForEnter();
			}


			/*check if the password in the second entry matches the first*/
			else if(i >= SIZE && (button >=0 && button <=9 ))
			{
				if(pass[i - SIZE ] == button)
					correct++;
				LCD_displayCharacter('*');
				/*After the user Enter the password, wait for ON*/
				if(i + 1 == 10)
					WaitForEnter();
			}

			else
				i--;

			/*A delay so the button is only pressed once*/
			_delay_ms(500);
		}
	}while(correct != SIZE && loop == NEW_ENTRY);
}

void WaitForEnter()
{
	LCD_moveCursor(1,0);

	LCD_displayString("					Press ON to continue!");
	uint8 enter =  KEYPAD_getPressedKey();
	/*Wait for the user to press ON*/
	while(enter != 13)
	{
		enter =  KEYPAD_getPressedKey();
	}
}

