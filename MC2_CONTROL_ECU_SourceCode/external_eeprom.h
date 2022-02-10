/*						External EEPROM Driver Module
 ******************************************************************************************
 ******************************************************************************************
 Written by: Hisham Yakan AbuBakr
 Project: Final Project
 File type: Module header file
  */

#ifndef EXTERNAL_EEPROM_H_
#define EXTERNAL_EEPROM_H_

#include "std_types.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
#define ERROR 0
#define SUCCESS 1
/*
 ************************************* Functions **********************************************
 */



/*
 ***************************** 1.EEPROM Initialization Function ***********************
 Function Description: This function is used to initialize the external EEPROM Driver by:
 1-Determining the bit rate of I2C
 2-Determining the ADDRESS


  Function Inputs: A pointer to structure of type I2C_Configuration

 */
void EEPROM_init(void);


/*
 ***************************** 2.EEPROM write byte Function ***********************
 Function Description: This function is write data in a byte of external EEPROM

  Function Inputs: uint16 address and uint8 data stored in address

 */
uint8 EEPROM_writeByte(uint16 u16addr,uint8 u8data);


/*
 ***************************** 1.EEPROM read byte Function ***********************
 Function Description: This function is read data from a byte of external EEPROM

  Function Inputs: uint16 address and uint8 data stored in address

 */
uint8 EEPROM_readByte(uint16 u16addr,uint8 *u8data);
 
#endif /* EXTERNAL_EEPROM_H_ */
