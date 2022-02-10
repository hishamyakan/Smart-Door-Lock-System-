/*						AVR Timer0 Driver Module
 ******************************************************************************************
 ******************************************************************************************
 Written by: Hisham Yakan AbuBakr
 Project: Final Project
 File type: Module header file
  */


#ifndef TIMER_H_
#define TIMER_H_
/*
 ************************************** Included files******************************************
 */
#include "std_types.h"


#define MY_CPU_FREQ 8000000

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

/*Enumeration to determine the operating mode of timer0 */

typedef enum
{
	normalMode,compareMode

}Timer0_Mode;


/*Enumeration to determine the preScaller  of timer0 clock */
typedef enum
{
	clk_NO_Source,clk_0,clk_8,clk_64,clk_256,clk_1024,clk_External_rising,clk_External_falling

}Timer0_Clock_Select;



/*Enumeration to determine the operation of timer0  */

typedef enum
{
	normal,toggle,clear,set

}OC0_Action;


/*Configuration Structure for timer0 */

typedef struct
{
	Timer0_Mode mode;
	Timer0_Clock_Select preScaller;
	uint8 intial_value;      			/* Max value for timer0 is 255 (1 byte) */
	uint8  compare_value;				/* Max value for timer0 OCR0 Register is 255 (1 byte) */
	OC0_Action action;

}Timer0_Configuration;


/*
 ************************************* Functions **********************************************
 */


/*
 ***************************** 1.Timer0 Initialization Function ***********************
 Function Description: This function is used to initialize the Timer0 Driver by:
 1-Determining the timer operating mode whether Normal Mode or Compare Mode
 2-Determining the preScaller of the timer
 3-Setting an initial value to the timer register

  Function Inputs: A pointer to structure of type Timer0_Configuration

 */
void Timer0_init(Timer0_Configuration *config);


/*
 ***************************** 3.Timer0 DeInitialization Function ***********************
 Function Description: This function is used to Deinitialize the Timer0 Driver by:
 1-Disabling the interrupt flag
 2-Setting the initial value to zero
 3-Setting the compare value to zero

  Function Inputs: void

 */

void Timer0_setCallBack(void(*a_ptr)(void));


/*
 ***************************** 3.Timer0 DeInitialization Function ***********************
 Function Description: This function is used to Deinitialize the Timer0 Driver by:
 1-Disabling the interrupt flag
 2-Setting the initial value to zero
 3-Setting the compare value to zero

  Function Inputs: void

 */
void Timer0_deInit(void);


//void Timer0_startCounting(uint8 seconds);


#endif /* TIMER_H_ */
