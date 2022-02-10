
/*						Smart Door System MC2 main Control ECU
 ******************************************************************************************
 ******************************************************************************************
 Written by: Hisham Yakan AbuBakr
 Project: Final Project
 File type: source file for MC2
  */

/************************ Setting the system frequency to 8 MHz ******************************/
# define F_CPU 8000000


/*
 ************************************* Static Configurations **********************************************
 */
#define PASSWORD_SIZE 	 5


/*
 ************************************** Included files******************************************
 */
#include "AppMC2.h"
#include "UART.h"
#include "external_eeprom.h"
#include "timer.h"
#include "avr/interrupt.h"
#include "Buzzer.h"
#include "DCmotor.h"

#define ENABLE_INTERRUPT() SREG |= (1<<7)  // Macro to enable global interrupt flag


int main()
{

	ENABLE_INTERRUPT();// enable interrupt

	/*********************************UART initialization***************************************/

	UART_Configuration myUart2;
	myUart2.UARTparity = disabled;
	myUart2.numberOfTransBits = bits_8;
	myUart2.UARTstopBit = stop_bit_1;
	myUart2.baudRate = 9600;
	myUart2.clk_edge = rising_edge;
	UART_init(& myUart2);


// Initialize EEPROM
	EEPROM_init();
	// Initialize buzzer
	Buzzer_init();
	// Initialize DC motor
	DcMotor_Init();

	// array to receive password from MC1
	uint8 recievedPassword[5];

	// array to receive password from EEPROM
	uint8 storedPassword[5];

	//Variable to receive orders from MC1
	uint8 order = noOrder;


	while(1)
	{


		/* Set the system state to normal state
		 * buzzer is off
		 * Motor is off
		 * */
		AppMC2_systemAtNormalMode();

		//Receive order from MC1
		order = AppMC2_recieve();

		//If the requested order to receive a password
		if(order == recievePassword)
		{
			//Ask MC1 to send password
			AppMC2_send(sendPassword);

			//Receive password
			AppMC2_recievePassword(recievedPassword,PASSWORD_SIZE);

			// Wait for MC1 command whether to store the password as new password or check with old
			order = AppMC2_recieve();

			// if new password store it
			if(order == storeNewPassword)
			{
				//store new password

				AppMC2_storePassword(recievedPassword);

				// send confirmation
				AppMC2_send(passwordIsStored);

			}
			// if check password compare it with stored and send match or mismatch
			else if(order == checkPassword)
			{
				//Retrieve stored password
				AppMC2_returnStoredPasswrod(storedPassword);

				//check sent password with retrieved one
				if(AppMC2_checkEqualPasswords(storedPassword,recievedPassword,PASSWORD_SIZE))
				{
					//Send password match
					AppMC2_send(passwordMatch);

				}

				else
				{
					//Send password mismatch
					AppMC2_send(passwordMisMatch);

				}

			}
		}

		//If the requested order to open the door
		else if (order == openDoor)
		{
			// open door
			AppMC2_openDoor();
			//send confirmation after finishing
			AppMC2_send(doorIsOpen);
		}

		//If the message is password mismatched 3 times
		else if (order == inputMismatch_3_times)
		{
			// Order MC1 to block system
			AppMC2_send(blockSystem);

			// Block system
			AppMC2_systemBlock();

		}


	}


	return 0;
}



