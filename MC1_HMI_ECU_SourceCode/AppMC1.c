
/*						Smart Door System MC1 Application Driver
 ******************************************************************************************
 ******************************************************************************************
 Written by: Hisham Yakan AbuBakr
 Project: Final Project
 File type: Module source file
  */





/************************ Setting the system frequency to 8 MHz ******************************/
# define F_CPU 8000000


/*
 ************************************** Included files******************************************
 */

#include "AppMC1.h"
#include "UART.h"
#include "lcd.h"
#include "keypad.h"
#include "string.h"
#include "std_types.h"
#include "util/delay.h"
#include "Timer.h"

/*****************************************Definitions ***************************************/

#define true 1
#define false 0

/******************************** Global Variables **********************************/
uint8 stateFlag = 0;  // This flag is set when the time required to be counted is finished

uint16 volatile MC1_counter = 0; // This counter counts the number of ties an ISR of timer0 is executed


uint16 volatile numberOfInterrupts = 0; /*This variable is used to have the number of interrupts
required to count the time
*/

uint16 volatile seconds = 0;

/*
 ********************************Static  Functions *****************************************
 */

static void AppMC1_getPasswordFromKeyPad(uint8 * arr,uint8 size);

static uint8 checkEqualPasswords(uint8 *newPassword,uint8 *reEnteredPassword,uint8 size);




/*
 ************************************* Functions **********************************************
 */
/*
 **************** 1.Application 1 setting new password from user function ***********************
 Function Description: This function is responsible of receiving a new password from the user by:

 1-Asking the user to enter new password
 2-Asking the user to re-enter the password
 3-Storing the password if both match or asking the user to re-enter because both password
 don't match
 4- Display on LCD a confirmation message that the password was stored successfully

  Function Inputs:
1. Array to store the first input password
2. Array to store the second input password
3. Array size (Password size)

Function return: No return(void)

 */

void AppMC1_setNewPassword(uint8 *newPassword,uint8 *reEnteredPassword,uint8 size)
{
	uint8 notEqual = true;  //Flag is true when password mismatch and fasle when password match
	uint8 counter = 0;


	LCD_clearScreen(); // clear the screen to be ready for writing
	LCD_moveCursor(0,0);
	do
	{
		LCD_clearScreen();
		LCD_moveCursor(0,0);
		/* if this is the first time to enter the loop skip this conditional
		 else display no match
		 * */
		if(counter>0)
		{
			LCD_displayString("No matching"); // display that both passwords didn't match
			_delay_ms(1500);

			LCD_clearScreen();
		}

		LCD_displayString("Enter new Pass");
		// get new password and store in first array
		AppMC1_getPasswordFromKeyPad(newPassword,size);
		_delay_ms(100);
		LCD_clearScreen();
		LCD_moveCursor(0,0);
		LCD_displayString("RE-enter new Pass");
		// get re entered password and store in second array
		AppMC1_getPasswordFromKeyPad(reEnteredPassword,size);
		// check if both are equal
		notEqual= !(checkEqualPasswords(newPassword,reEnteredPassword,size));
		LCD_clearScreen();

		counter++;

	}while(notEqual);


	// Ask MC2 To receive the password
	AppMC1_send(recievePassword);

	//Wait until MC2 Ask you to send
	while(AppMC1_recieve() != sendPassword);

	// send the password
	AppMC1_sendPassword(newPassword,size);

	// Ask MC2 To store the sent password
	AppMC1_send(storeNewPassword);

	//wait until password is stored
	while( AppMC1_recieve() != passwordIsStored);

    // Infrom the user that the password is stored through the LCD
	LCD_displayString("Password is ");
	LCD_moveCursor(1,0);
	LCD_displayString("Stored");
	_delay_ms(1000);

}


/*
 **************** 2.Application 1 getting password form user function ***********************
 Function Description: This function is responsible of receiving the password from the user
 and check whether this password is the stored one by:

 1-Asking the user to enter the  password
 2-Sending the password to the Second micro-Controller


  Function Inputs:
1. Array to get password
2. Array size (Password size)

Function return: uint8 ,returns either of two choices:
1.Password mismatch (if entered password is different from stored password)
2.Password match (if entered password is same as stored password)

 */



uint8 AppMC1_getPasswordFromUser(uint8 * password,uint8 size)
{
	//Ask the user to enter the password through the LCD
	LCD_clearScreen();
	LCD_moveCursor(0,0);
	LCD_displayString("Enter Password");

	// get password and store in array
	AppMC1_getPasswordFromKeyPad(password,size);

	// Ask MC2 To receive the password
	AppMC1_send(recievePassword);

	//Wait until MC2 Ask you to send
	while( AppMC1_recieve() != sendPassword);

	// send the password
	AppMC1_sendPassword(password,size);

	//Ask the MC2 to check if this password is the same as the stored one
	AppMC1_send(checkPassword);

	//Receive the result
	uint8 recMessage = AppMC1_recieve();

	return  recMessage;
}


/*
 *******************  3.MicroController 1 orders send function  **************************
 Function Description: This function is responsible of sending the orders form microcontroller1
 to microcontroller2 by

 1-Sending a ready byte
 2-wait for microcontroller2 to send ready byte
 3-sending the order to microconttroller2


  Function Inputs:
1. message(order)


Function return: No return(void)

 */

void AppMC1_send(uint8 message)
{

	//Sent the ready byte to MC2 in order Synchronize
	UART_sendByte(MC1_ready);

	//Wait until MC2 is ready
	while(UART_recieveByte() != MC2_ready)

		{
		// if there is a delay inform the user
			LCD_displayStringRowColumn(0,0,"WAITING SEND");
		}

	// When MC2 is ready send the message
	UART_sendByte(message);
}


/*
 *******************  4.MicroController 1 receive order function  **************************
 Function Description: This function is responsible of receiving orders form microcontroller2
 and achieving synchronization by:

 1-waiting for microcontroller2 to send ready byte
 2-sending a ready byte to microcontroller2
 3-receive the order and return it


  Function Inputs: no input(void)


Function return: 1.uint8 returns microcontroller2 message (microcontroller2 order)

 */

uint8 AppMC1_recieve(void)
{
	//Wait until MC2 is ready
	while(UART_recieveByte() != MC2_ready)
	{
		// if there is a delay inform the user
		LCD_displayStringRowColumn(0,0,"WAITING REC");
	}

	//Sent the ready byte to MC2 in order Synchronize
	UART_sendByte(MC1_ready);

	return UART_recieveByte();
}


/*
 *******************  5.MicroController 1 send password function  **************************
 Function Description: This function is responsible of sending the password(array of numbers)
 to microcontroller2

 Function Inputs:
1. Array to get password
2. Array size (Password size)

Function return: No return(void)

 */

void AppMC1_sendPassword(uint8 * password,uint8 size)
{
	int i = 0;
	for(i =0 ; i<size;i++)
	{
		UART_sendByte(password[i]);
	}
}




/*
 *******************  6.MicroController 1 receive order form user function  **************************
 Function Description: This function is responsible of receiving an order from the user whether
 to:
 +: Open the door
 -: Set new Password

 if any other input is entered the function would discard this input

 Function Inputs:void(no input)

Function return: uint8 order (ascii of '+' or '-')

 */


uint8  AppMC1_getOrderFromUser(void)
{
	uint8  key; // variable to store the pressed key
	uint8 wrongInput = true; // flag is set when wrong input and cleared in case of correct input

	// Ask the user for an order
	LCD_clearScreen();
	LCD_moveCursor(0,0);
	LCD_displayString("+: Open Door");
	LCD_moveCursor(1,0);
	LCD_displayString("-: ChangePassword");

	do {

		key = KEYPAD_getPressedKey();
		_delay_ms(300); // wait press Time
		if(key == '+' || key == '-')
		{
			return key;
		}


	} while (wrongInput); // stay in loop until you get correct input


	return 0;

}


/*
 *******************  7.MicroController 1 system block function  **************************
 Function Description: This function is responsible of blocking the system for 1 minute in case
 of password mismatch for three times by:
 1- setting the set  call back function
 2-starting count according to the time need
 3- waiting until the count is over and displaying an error message

 Function Inputs:void(no input)

Function return: No return(void)

 */

void AppM1_systemBlock(void)
{
	LCD_clearScreen();
	Timer0_setCallBack(AppMC1_setCallBackFunction); // send the set call back function to timer

	// Count for 1 minute
	AppMC1_startCounting(60);

	// wait until time over in blocked state and display error message
	while(stateFlag == 0)
	{
		LCD_moveCursor(0,0);
		LCD_displayString("System Blocked");
		LCD_moveCursor(1,0);
		LCD_displayString("Time left:");
		LCD_moveCursor(1,11);
		if(seconds >9)
		{
			LCD_intgerToString(seconds);
		}
		else
		{

			LCD_intgerToString(seconds);
			LCD_moveCursor(1,12);
			LCD_displayCharacter(' ');
		}
	}

	// return flag to original state
	stateFlag = 0;

}

/*
 *******************  8.MicroController 1 set Call Back Function   **************************
 Function Description: This function is the set call back function executed when an interrupt
 occurs.
 It has counter as a global variable and this counters increments every ISR until it exceeds
 certain limit then time is over

 Function Inputs:void(no input)

Function return: No return(void)

 */



void AppMC1_setCallBackFunction(void)
{
	// increment by 1 every interrupt
		MC1_counter++;

		if(MC1_counter%40 == 0)
		{
			seconds --;
		}

		// if you exceed the number of interrupts required set the flag and deInit Timer0
		if(MC1_counter>numberOfInterrupts)
		{
			stateFlag =1;
			MC1_counter = 0;
			numberOfInterrupts = 0;
			Timer0_deInit();

		}
}


/*
 *******************  9.MicroController 1 start counting function   **************************
 Function Description: This function is responsible of :
 1-Initializing the timer in the compare mode.
 2-Calculate the number of interrupts required to achieve the time entered in seconds
 3- Storing this value in global variable


 Function Inputs:uint8 time in seconds

Function return: No return(void)

 */
void AppMC1_startCounting(uint8 timeSeconds)
{
/*					Timer0 Configured in compare mode
 *
 *	set Compare mode
 *	preScaller = 1024
 *	initial value = 0
 *	compare value = 194S
 * */


	Timer0_Configuration myTimer;
	myTimer.mode = compareMode;
	myTimer.preScaller= clk_1024;
	myTimer.intial_value = 0;
	myTimer.compare_value = 194;


	Timer0_init(&myTimer);

	seconds = timeSeconds;
	// Number of interrupts = time in ms / time of one interrupt in ms
	numberOfInterrupts = (uint16)(timeSeconds * 1000)/25;
}



/*
 *******************  10.MicroController 1 checking equal passwords function   **************************
 Function Description: This is a static function
 This function is responsible of checking if tow passwords are the same

 Function Inputs:
 1. Array to store the first input password
2. Array to store the second input password
3. Array size (Password size)

Function return:return true if passwords match and false if password mismatch

 */


static uint8 checkEqualPasswords(uint8 *newPassword,uint8 *reEnteredPassword,uint8 size)
{
	uint8 i;
	for(i = 0; i<size;i++)
	{
		if (newPassword[i] != reEnteredPassword[i])
		{
			return false;
		}
	}

	return true;

}

/*
 **********  11.MicroController 1 get password from keypad function   ********************
 Function Description: This is a static function
 This function is responsible of getting the password form keypad

password : Number between 0 & 9

***********************important note************************


The enter is considered '=' Equal
 Function Inputs:
 1. Array to store the  password
2. Array size (Password size)

Function return:no return(void)

 */

static void AppMC1_getPasswordFromKeyPad(uint8 * arr,uint8 size)
{
	uint8  key; // variable to store the pressed key
	uint8 flag = 0; // count the number of correct inputs and break after password size is reached

	while(flag<size)
	{
		key = KEYPAD_getPressedKey();
		_delay_ms(300);// wait press Time
		if((key <= 9) && (key >= 0))
		{
			arr[flag] = key;

			LCD_moveCursor(1,flag);
			LCD_displayCharacter('*');

			flag++;

		}
	}

	// wait until user press enter key ('=')
	key = KEYPAD_getPressedKey();
	while(key != '=')
	{
		key = KEYPAD_getPressedKey();
		_delay_ms(300);// wait press Time
	}


}


