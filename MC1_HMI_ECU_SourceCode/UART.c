/*							AVR UART Driver Module
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

#include "UART.h"
#include "std_types.h"
#include "avr/io.h"
#include "Macros.h"


/****************************************************************************************************
 *                           Global Variables                                  *
 ****************************************************************************************************/

/* Global variable to be used as flag in case of 9 bit mode UART*/
static uint8 flag_9_bit_mode = 0;



/*
 ************************************* Functions **********************************************
 */


/*
 ***************************** 1.UART Initialization Functions ***********************
 Function Description: This function is used to initialize the uart Driver

  Function Inputs: A pointer to structure of type uart_Configuration

 */

void UART_init(UART_Configuration *config)
{
	uint16 ubrr_value = 0;

	/*	Enabling the double transmission by setting the bit U2X in UCSRA */
	UCSRA = (1<<U2X);

	/*	Enabling the module send and receive */

	UCSRB = (1<<RXEN) | (1<<TXEN);

	if(config->numberOfTransBits == bits_9)
	{
			UCSRB |= (1<<UCSZ2);
			flag_9_bit_mode = 1;
	}
	UCSRC = ((1<<URSEL))|((UCSRC&0xCF)|(config->UARTparity<<4))|((UCSRC&0xF7)|(config->UARTstopBit<<3))\
		|((UCSRC&0xF9)|((config->numberOfTransBits&0x3)<<1))|(config->clk_edge);


	ubrr_value = (uint16)(((F_CPU / (config->baudRate * 8UL))) - 1);

	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	UBRRH = ubrr_value>>8;
	UBRRL = ubrr_value;


}


/*
 ***************************** 2.UART SEND data Functions ***********************
 Function Description: This function is used to send data through the uart Driver

  Function Inputs: uint16 data (uint16v is for 9 bits mode)
    return: void

 */


void UART_sendByte( uint16 data)
{
	data &= 0x01FF;
	while(BIT_IS_CLEAR(UCSRA,UDRE))
	{} // wait until UDR is empty
	if(flag_9_bit_mode)
	{
		UCSRB |= (data>>8);
	}
	UDR = data;
}


/*
 ***************************** 3.UART RECEIVE data Functions ***********************
 Function Description: This function is used to receive data from the UART Driver

  Function Inputs: void
  return: uint16 data

 */

uint16 UART_recieveByte(void)
{
	uint16 recData;
	while(BIT_IS_CLEAR(UCSRA,RXC)){}
	recData = GET_BIT(UCSRB,RXB8)<<8;
	recData |= UDR;

	return recData;
}



/*
 ***************************** 4.UART send string Function ***********************
 Function Description: This function is used to send data through the UART Driver

  Function Inputs: pointer to uint8 data (uint16v is for 9 bits mode)
  return: void

 */
void UART_sendString(const uint8 *data)
{
	while(*data != '\0') // wait until Null character
	{
		UART_sendByte(*data);
		data++;
	}

}

/*
 ***************************** 5.UART receive string Function ***********************
 Function Description: This function is used to send data through the UART Driver

  Function Inputs: pointer to uint8 data (uint16v is for 9 bits mode)
  return: void

 */
void UART_recieveString(uint8 *Str)
{
	uint8 i = 0;

		/* Receive the first byte */
		Str[i] = UART_recieveByte();

		/* Receive the whole string until the '#' */
		while(Str[i] != '#')
		{
			i++;
			Str[i] =UART_recieveByte();
		}

		/* After receiving the whole string plus the '#', replace the '#' with '\0' */
		Str[i] = '\0';

}



