/*						Buzzer Driver Module
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
#include "Buzzer.h"
#include "gpio.h"
#include "std_types.h"

/******************************Static configuration****************************************/
#define BUZZER_PORT_ID PORTB_ID
#define BUZZER_PIN_ID	PIN7_ID


/*
 ************************************* Functions **********************************************
 */

/*
 ***************************** 1.Buzzer Initialization Function ***********************
 Function Description: This function is used to initialize the Buzzer by:
 1-Set pin direction as output
 2-Giving initial value of digital low


  Function Inputs: void
  Function outputs: void

 */

void Buzzer_init(void)
{
	GPIO_setupPinDirection(BUZZER_PORT_ID,BUZZER_PIN_ID,PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);

}

/*
 ***************************** 2.Buzzer On Function ***********************
 Function Description: This function is used to Turn on the Buzzer by:
 1-Giving value of digital high to buzzer pin


  Function Inputs: void
  Function outputs: void

 */


void buzzerOn(void)
{
GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_HIGH);
}


/*
 ***************************** 3.Buzzer Off Function ***********************
 Function Description: This function is used to Turn off the Buzzer by:
 1-Giving value of digital low to buzzer pin


  Function Inputs: void
  Function outputs: void

 */

void buzzerOff(void)
{
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);
}

