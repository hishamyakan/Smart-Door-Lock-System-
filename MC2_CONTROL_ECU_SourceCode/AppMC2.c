
/*						Smart Door System MC2 Application Driver
 ******************************************************************************************
 ******************************************************************************************
 Written by: Hisham Yakan AbuBakr
 Project: Final Project
 File type: Module source file
  */



/************************ Setting the system frequency to 8 MHz ******************************/
# define F_CPU 8000000

/*****************************************Definitions ***************************************/
#define true 1
#define false 0


/******************************Static configuration****************************************/
#define PASSWORD_ADDRESS 0x201
#define PASSWORD_SIZE 	 5



/*
 ************************************** Included files******************************************
 */
#include "AppMC2.h"
#include "UART.h"
#include "I2C.h"
#include "external_eeprom.h"
#include "DCmotor.h"
#include "std_types.h"
#include "Buzzer.h"
#include <util/delay.h>
#include "Timer.h"


uint8 stateFlag = 0; // This flag is set when the time required to be counted is finished

uint16 volatile counter = 0;// This counter counts the number of ties an ISR of timer0 is executed

uint16 volatile numberOfInterrupts = 0; /*This variable is used to have the number of interrupts
required to count the time*/


/*
 ************************************* Functions **********************************************
 */

/*

 *******************  1.System normal mode function  **************************
 Function Description:
 This function is responsible of setting the system to the normal mode which is:
 1- Motor is off(Door is stationary)
 2- Buzzer is Off

 Function Inputs: No inputs(void)

Function return:No outputs(void)

 */

void AppMC2_systemAtNormalMode(void)
{
	buzzerOff();   // Turn off alarm (Buzzer is off)
	DcMotor_Rotate(Motor_OFF); // Motor is off(Door is stationary)
}



/*
 *******************  2.Door open function  **************************
 Function Description:
 This function is responsible of opening the door when the user request:
 1- Turn on the motor for 15 Seconds and rotate clockwise
 2- Turn off the motor for 3 seconds
 3- Turn on the motor  for 15 Seconds and rotate anti-clockwise

 Function Inputs: No inputs(void)

Function return:No outputs(void)
 */


void AppMC2_openDoor(void)
{

	// Rotate the motor clockwise and wait 15 Seconds

	Timer0_setCallBack(AppMC2_setCallBackFunction);
	DcMotor_Rotate(Motor_CW);  //Rotate motor
	AppMC2_startCounting(15);  // Wait 15 seconds
	while(stateFlag == 0);
	stateFlag = 0;

	// Stop motor for 3 seconds
	DcMotor_Rotate(Motor_OFF);  //stop motor
	AppMC2_startCounting(3);     // Wait 3 seconds
	while(stateFlag == 0);
	stateFlag = 0;

	// Rotate the motor anti-clockwise and wait 15 Seconds
	DcMotor_Rotate(Motor_AntiCW);//Rotate motor
	AppMC2_startCounting(15);  // Wait 15 seconds
	while(stateFlag == 0);
	stateFlag = 0;

	DcMotor_Rotate(Motor_OFF);  //stop motor


}


/*
 *******************  3.MicroController 2 orders send function  **************************
 Function Description: This function is responsible of sending the orders form microcontroller2
 to microcontroller1 by

 1-Sending a ready byte
 2-wait for microcontroller1 to send ready byte
 3-sending the order to microconttroller1


  Function Inputs:
1. message(order)


Function return: No return(void)

 */

void AppMC2_send(uint8 message)
{
	//Sent the ready byte to MC1 in order Synchronize
	UART_sendByte(MC2_ready);

	//Wait until MC1 is ready
	while(UART_recieveByte() != MC1_ready)
	{}

	// When MC1 is ready send the message
	UART_sendByte(message);
}



/*
 *******************  4.MicroController 2 receive order function  **************************
 Function Description: This function is responsible of receiving orders form microcontroller1
 and achieving synchronization by:

 1-waiting for microcontroller1 to send ready byte
 2-sending a ready byte to microcontroller1
 3-receive the order and return it


  Function Inputs: no input(void)


Function return: 1.uint8 returns microcontroller1 message (microcontroller1 order)

 */
uint8 AppMC2_recieve(void)
{

	//Wait until MC1 is ready

	while(UART_recieveByte() != MC1_ready)
	{
	}

	//Sent the ready byte to MC2 in order Synchronize
	UART_sendByte(MC2_ready);

	return UART_recieveByte();
}


/*
 *******************  5.MicroController 2 receive password function  **************************
 Function Description: This function is responsible of receiving the password(array of numbers)
 from microcontroller1

 Function Inputs:
1. Array to get password
2. Array size (Password size)

Function return: No return(void)

 */
void AppMC2_recievePassword(uint8 *receivedPassword,uint8 size)
{
	int i = 0;
		for(i =0 ; i<size;i++)
		{
			receivedPassword[i] = UART_recieveByte();
		}

}


/*
 *********  6.MicroController 2 store password in external EEPROM function  ***************
 Function Description: This function is responsible of storing the password in EEPROM in an
 array

 Function Inputs:
1. Array to store password

note:
The password size is static configuration you can modify

Function return: No return(void)

 */


void  AppMC2_storePassword(uint8* password)
{

	uint8 i = 0 ;

	while(i<PASSWORD_SIZE)
	{
		EEPROM_writeByte(PASSWORD_ADDRESS+i,password[i]);
		_delay_ms(10);
		i++;

	}

}



/*
 *********  7.MicroController 2 retrieve password from external EEPROM function  ***************
 Function Description: This function is responsible of storing the password in EEPROM in an
 array

 Function Inputs:
1. Array to get password

Function return: No return(void)

 */
void  AppMC2_returnStoredPasswrod(uint8* password)
{

	uint8 i = 0 ;

	while(i<PASSWORD_SIZE)
	{
		EEPROM_readByte(PASSWORD_ADDRESS+i,&password[i]);
		_delay_ms(10);
		i++;
	}

}


/*
 *******************  8.MicroController 2 checking equal passwords function   **************************
 Function Description:
 This function is responsible of checking if tow passwords are the same

 Function Inputs:
 1. Array to store the first input password
2. Array to store the second input password
3. Array size (Password size)

Function return:return true if passwords match and false if password mismatch

 */

uint8 AppMC2_checkEqualPasswords(uint8 *newPassword,uint8 *reEnteredPassword,uint8 size)
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
 *******************  9.MicroController 2 system block function  **************************
 Function Description: This function is responsible of blocking the system for 1 minute in case
 of password mismatch for three times by:
 1- setting the set  call back function
 2-starting count according to the time need
 3- Buzzer on until count is over
 4- Motor is off until count is over

 Function Inputs:void(no input)

Function return: No return(void)

 */
void AppMC2_systemBlock(void)
{
	Timer0_setCallBack(AppMC2_setCallBackFunction);// send the set call back function to timer

	// Count for 1 minute
	AppMC2_startCounting(60);

	// wait until time over in blocked state
	while(stateFlag == 0)
	{
		buzzerOn();  // Turn on buzzer (Alarm is ON)
		DcMotor_Rotate(Motor_OFF); // Stop Motor

	}
	stateFlag = 0;


}

/*
 *******************  10.MicroController 2 set Call Back Function   **************************
 Function Description: This function is the set call back function executed when an interrupt
 occurs.
 It has counter as a global variable and this counters increments every ISR until it exceeds
 certain limit then time is over

 Function Inputs:void(no input)

Function return: No return(void)

 */


void AppMC2_setCallBackFunction(void)
{
	counter++; // increment by 1 every interrupt

	// if you exceed the number of interrupts required set the flag and deInit Timer0
	if(counter>numberOfInterrupts)
	{
		stateFlag =1;
		counter = 0;
		numberOfInterrupts = 0;
		Timer0_deInit();

	}

}




/*
 ******************* 11.MicroController 2 start counting function   **************************
 Function Description: This function is responsible of :
 1-Initializing the timer in the compare mode.
 2-Calculate the number of interrupts required to achieve the time entered in seconds
 3- Storing this value in global variable


 Function Inputs:uint8 time in seconds

Function return: No return(void)

 */

void AppMC2_startCounting(uint8 timeSeconds)
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
	myTimer.action = normal;

	Timer0_init(&myTimer);
	// Number of interrupts = time in ms / time of one interrupt in ms
	numberOfInterrupts = (uint16)(timeSeconds * 1000)/25;

}
