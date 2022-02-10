/*						Buzzer Driver Module
 ******************************************************************************************
 ******************************************************************************************
 Written by: Hisham Yakan AbuBakr
 Project: Final Project
 File type: Module header file
  */

#ifndef BUZZER_H_
#define BUZZER_H_

/*
 ************************************** Included files******************************************
 */
#include "std_types.h"

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


void Buzzer_init(void);

/*
 ***************************** 2.Buzzer On Function ***********************
 Function Description: This function is used to Turn on the Buzzer by:
 1-Giving value of digital high to buzzer pin


  Function Inputs: void
  Function outputs: void

 */
void buzzerOn(void);

/*
 ***************************** 3.Buzzer Off Function ***********************
 Function Description: This function is used to Turn off the Buzzer by:
 1-Giving value of digital low to buzzer pin


  Function Inputs: void
  Function outputs: void

 */
void buzzerOff(void);


#endif /* BUZZER_H_ */
