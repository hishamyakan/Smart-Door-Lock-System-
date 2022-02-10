
/*						Smart Door System MC2 Application Driver
 ******************************************************************************************
 ******************************************************************************************
 Written by: Hisham Yakan AbuBakr
 Project: Final Project
 File type: Module header file
  */

#ifndef APPMC2_H_
#define APPMC2_H_

/*
 ************************************** Included files******************************************
 */
#include "std_types.h"


/***********************************************************************************************
                          Types Declaration
 ***********************************************************************************************/


/****************************UART transfer protocol enum************************************
 This enum is used to encode the orders sent from one micro-controller to another the numbers
 were chosen random but with distinct number for each .
 using these orders both  micro-controllers can understand each other
 This enum is in the header file for both micro-controllers
  */


typedef enum
{
	recievePassword=15,sendPassword=18,
	checkPassword=22,passwordMatch=24,passwordMisMatch=26,
	storeNewPassword=33,passwordIsStored=36,
	openDoor=40,doorIsOpen=42,
	inputMismatch_3_times=66,blockSystem=68,
	MC1_ready=71,MC2_ready=74,
	noOrder

}UART_Encoding;


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

void AppMC2_systemAtNormalMode(void);


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


void AppMC2_openDoor(void);


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
void AppMC2_send(uint8 message);



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
uint8 AppMC2_recieve(void);



/*
 *******************  5.MicroController 2 receive password function  **************************
 Function Description: This function is responsible of receiving the password(array of numbers)
 from microcontroller1

 Function Inputs:
1. Array to get password
2. Array size (Password size)

Function return: No return(void)

 */


void AppMC2_recievePassword(uint8 *receivedPassword,uint8 size);



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

void  AppMC2_storePassword(uint8* password);




/*
 *********  7.MicroController 2 retrieve password from external EEPROM function  ***************
 Function Description: This function is responsible of storing the password in EEPROM in an
 array

 Function Inputs:
1. Array to get password

Function return: No return(void)

 */


void  AppMC2_returnStoredPasswrod(uint8* password);

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

uint8 AppMC2_checkEqualPasswords(uint8 *newPassword,uint8 *reEnteredPassword,uint8 size);


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

void AppMC2_systemBlock(void);

/*
 *******************  10.MicroController 2 set Call Back Function   **************************
 Function Description: This function is the set call back function executed when an interrupt
 occurs.
 It has counter as a global variable and this counters increments every ISR until it exceeds
 certain limit then time is over

 Function Inputs:void(no input)

Function return: No return(void)

 */
void AppMC2_setCallBackFunction(void);

/*
 ******************* 11.MicroController 2 start counting function   **************************
 Function Description: This function is responsible of :
 1-Initializing the timer in the compare mode.
 2-Calculate the number of interrupts required to achieve the time entered in seconds
 3- Storing this value in global variable


 Function Inputs:uint8 time in seconds

Function return: No return(void)

 */
void AppMC2_startCounting(uint8 timeSeconds);




#endif /* APPMC2_H_ */
