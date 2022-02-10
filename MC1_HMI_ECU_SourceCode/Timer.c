/*							AVR Timer0 Driver Module
 ******************************************************************************************
 ******************************************************************************************
 Written by: Hisham Yakan AbuBakr
 Project: Final Project
 File type: Module source file
  */

# define F_CPU 8000000
/*
 ************************************** Included files******************************************
 */
#include "Timer.h"
#include "std_types.h"
#include "Macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>


/****************************************************************************************************
 *                           Global Variables                                  *
 ****************************************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;



/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER0_OVF_vect)
{
	(*g_callBackPtr)();
}

ISR(TIMER0_COMP_vect)
{
	(*g_callBackPtr)();
}



/*
 ************************************* Functions **********************************************
 */


/*
 ***************************** 1.Timer0 Initialization Functions ***********************
 Function Description: This function is used to initialize the Timer0 Driver by:
 1-Determining the timer operating mode whether Normal Mode or Compare Mode
 2-Determining the preScaller of the timer
 3-Setting an initial value to the timer register

  Function Inputs: A pointer to structure of type Timer0_Configuration

 */
void Timer0_init(Timer0_Configuration *config)
{


	if(config->mode == normalMode)
	{
		/*
		 * setting the preScaller value in the CS02/CS01/CS01
		 *
		 * */
		TCCR0 = (TCCR0 &(0x80) )| config->preScaller;

		TCNT0 = config->intial_value;
		SET_BIT(TIMSK,TOIE0);

	}
	else if (config->mode == compareMode)
	{
		SET_BIT(TCCR0,FOC0);
		SET_BIT(TCCR0,WGM01);
		TCCR0 = (TCCR0 &(0xF8))| config->preScaller;
		//TCCR0 = (TCCR0&(0xCF))|(config->action<<4);
		TCNT0 = config->intial_value;
		OCR0 = config->compare_value;
		SET_BIT(TIMSK,OCIE0);

	}
	else
	{

	}


}


void Timer0_setCallBack(void(*a_ptr)(void))
{
	g_callBackPtr = a_ptr;
}

/*
 ***************************** 3.Timer0 DeInitialization Function ***********************
 Function Description: This function is used to Deinitialize the Timer0 Driver by:
 1-Disabling the interrupt flag
 2-Setting the initial value to zero
 3-Setting the compare value to zero

  Function Inputs: void

 */
void Timer0_deInit(void)
{
	CLEAR_BIT(TIMSK,OCIE0);
	CLEAR_BIT(TIMSK,TOIE0);
	TCCR0 = 0;
	TCNT0 = 0;


}


