
/*						Smart Door System MC1 main HMI
 ******************************************************************************************
 ******************************************************************************************
 Written by: Hisham Yakan AbuBakr
 Project: Final Project
 File type: source file for MC1
  */


/************************ Setting the system frequency to 8 MHz ******************************/
# define F_CPU 8000000

/*
 ************************************** Included files******************************************
 */
#include "AppMC1.h"
#include "lcd.h"
#include "keypad.h"
#include  "util/delay.h"
#include "UART.h"
#include "avr/interrupt.h"


#define true  1
#define false 0

/*
 ************************************* Static Configurations **********************************************
 */
#define PASSWORD_SIZE 5

#define ENABLE_INTERRUPT() SREG |= (1<<7)  // Macro to enable global interrupt flag

int main()
{

	ENABLE_INTERRUPT(); // enable interrupts

	/* variable to store how many times user failed to enter correct password
	if user failed to enter 3 time system is blocked and variable is set to 0 again
	 * */
	uint8 password_mismatch_counter= 0;

	// Variable to store received messages from MC2
	uint8 received_message;

	// Array to store the new password
	uint8 newPassword[PASSWORD_SIZE];

	//Array to store the re-entered password
	uint8 reEnteredPassword[PASSWORD_SIZE];


/*********************************UART initialization***************************************/
	UART_Configuration myUart;
	myUart.UARTparity = disabled;
	myUart.numberOfTransBits = bits_8;
	myUart.UARTstopBit = stop_bit_1;
	myUart.baudRate = 9600;
	myUart.clk_edge = rising_edge;
	UART_init(& myUart);
/*********************************LCD initialization***************************************/
	LCD_init();



	// Set the new password for the first time and send it to MC2
	AppMC1_setNewPassword(newPassword,reEnteredPassword,PASSWORD_SIZE);

	// clear screen to display new commands
	LCD_clearScreen();
	LCD_moveCursor(0,0);

	// Main Loop

	while(1)
	{
		// Variable to receive user order
		uint8 userOrder;

		// Array to receive user input password and send it MC2 in order to compare
		uint8 password[PASSWORD_SIZE];

		//receive order from user
		userOrder = AppMC1_getOrderFromUser();
		do {
			received_message = AppMC1_getPasswordFromUser(password,PASSWORD_SIZE);//asking for the password
			if(received_message == passwordMisMatch)
			{
				// In case password mismatch increment counter
				password_mismatch_counter++;
			}
			else
			{
				// Display Correct password message and break
				LCD_clearScreen();
				LCD_displayString("Correct Password");
				_delay_ms(900);
				break;
			}

		} while (password_mismatch_counter<3);


		// if mismatch is 3 times Block the system
		if(password_mismatch_counter >2)
		{
			// Send the mismatch to  MC2
			AppMC1_send(inputMismatch_3_times);

			//Receive order from MC2
			uint8 recMessage = AppMC1_recieve() ;

			//If MC2 Asks to block the system block it
			if(recMessage == blockSystem)
			{
				AppM1_systemBlock();
			}
			password_mismatch_counter = 0; // restore initial value
			continue;
		}

		// If user request to open door
		if(userOrder == '+')
		{
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,4,"Welcome");
			LCD_displayStringRowColumn(1,0,"Door is opening");

			// Ask MC2 to open door
			AppMC1_send(openDoor);
			uint8 recMessage = AppMC1_recieve() ;

			//wait until the door is opened
			while(recMessage!= doorIsOpen)
			{

			}

		}
		// If user request to change password
		else if (userOrder == '-')
		{
			//Ask for the new password
			AppMC1_setNewPassword(newPassword,reEnteredPassword,PASSWORD_SIZE);

		}
		password_mismatch_counter = 0; // resotre original value

	}


	return 0;
}





