/*						AVR UART Driver Module
 ******************************************************************************************
 ******************************************************************************************
 Written by: Hisham Yakan AbuBakr
 Project: Final Project
 File type: Module header file
 */

#ifndef UART_H_
#define UART_H_

/*
 ************************************** Included files******************************************
 */
#include "std_types.h"


/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

//Enumeration to determine the number of bits sent or received by UART

typedef enum
{
	bits_5,	bits_6,	bits_7,	bits_8,	bits_9=7
}Number_Of_Bits;


//Enumeration to determine the Parity type
typedef enum
{
	disabled,even=2,odd=3
}parity_type;


//Enumeration to determine the number of stop bits
typedef enum
{
	stop_bit_1,stop_bit_2
}stop_bit;


//Enumeration to determine the clock edge
typedef enum
{
	rising_edge,falling_edge
}clock_edge;


/*Configuration Structure for UART */
typedef struct
{
	Number_Of_Bits numberOfTransBits;
	parity_type UARTparity;
	stop_bit UARTstopBit;
	uint32 baudRate;
	clock_edge clk_edge;


}UART_Configuration;


/*
 ************************************* Functions **********************************************
 */


/*
 ***************************** 1.UART Initialization Functions ***********************
 Function Description: This function is used to initialize the uart Driver

  Function Inputs: A pointer to structure of type uart_Configuration

 */


void UART_init(UART_Configuration *config);

/*
 ***************************** 2.UART SEND data Functions ***********************
 Function Description: This function is used to send data through the uart Driver

  Function Inputs: uint16 data (uint16v is for 9 bits mode)
    return: void

 */


void UART_sendByte(const uint16 data);



/*
 ***************************** 3.UART RECEIVE data Functions ***********************
 Function Description: This function is used to receive data from the UART Driver

  Function Inputs: void
  return: uint16 data

 */
uint16 UART_recieveByte(void);


/*
 ***************************** 4.UART send string Function ***********************
 Function Description: This function is used to send data through the UART Driver

  Function Inputs: pointer to uint8 data (uint16v is for 9 bits mode)
  return: void

 */
void UART_sendString(const uint8 *data);

/*
 ***************************** 5.UART receive string Function ***********************
 Function Description: This function is used to send data through the UART Driver

  Function Inputs: pointer to uint8 data (uint16v is for 9 bits mode)
  return: void

 */
void UART_recieveString(uint8 *Str);


#endif /* UART_H_ */
