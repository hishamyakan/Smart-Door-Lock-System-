
/*						Smart Door System MC1 Application Driver
 ******************************************************************************************
 ******************************************************************************************
 Written by: Hisham Yakan AbuBakr
 Project: Final Project
 File type: Module header file
  */



#ifndef APPMC1_H_
#define APPMC1_H_

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

void AppMC1_setNewPassword(uint8 *newPassword, uint8 *reEnteredPassword, uint8 size);

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

uint8 AppMC1_getPasswordFromUser(uint8 * password,uint8 size);


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

void AppMC1_send(uint8 message);



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

uint8 AppMC1_recieve(void);


/*
 *******************  5.MicroController 1 send password function  **************************
 Function Description: This function is responsible of sending the password(array of numbers)
 to microcontroller2

 Function Inputs:
1. Array to get password
2. Array size (Password size)

Function return: No return(void)

 */


void AppMC1_sendPassword(uint8 * password,uint8 size);

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


uint8 AppMC1_getOrderFromUser(void);


/*
 *******************  7.MicroController 1 system block function  **************************
 Function Description: This function is responsible of blocking the system for 1 minute in case
 of password mismatch for three times by:
 1- setting the set  call back function
 2-starting count according to the time need
 3- displaying an error message until the count is over

 Function Inputs:void(no input)

Function return: No return(void)

 */


void AppM1_systemBlock(void);


/*
 *******************  8.MicroController 1 set Call Back Function   **************************
 Function Description: This function is the set call back function executed when an interrupt
 occurs.
 It has counter as a global variable and this counters increments every ISR until it exceeds
 certain limit then time is over

 Function Inputs:void(no input)

Function return: No return(void)

 */

void AppMC1_setCallBackFunction(void);

/*
 *******************  9.MicroController 1 start counting function   **************************
 Function Description: This function is responsible of :
 1-Initializing the timer in the compare mode.
 2-Calculate the number of interrupts required to achieve the time entered in seconds
 3- Storing this value in global variable


 Function Inputs:uint8 time in seconds

Function return: No return(void)

 */

void AppMC1_startCounting(uint8 timeSeconds);


#endif /* APPMC1_H_ */
