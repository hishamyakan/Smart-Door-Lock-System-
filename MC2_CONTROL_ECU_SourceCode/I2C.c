/*						I2C Driver Module
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
#include "I2C.h"
#include "avr/io.h"
#include "std_types.h"
#include "Macros.h"



/*
 ************************************* Functions *******************************************
 */


/*
 ***************************** 1.I2C Initialization Function ***********************
 Function Description: This function is used to initialize the I2C Driver

  Function Inputs: A pointer to structure of type I2C_Configuration
  Function return: void

 */
void TWI_init(I2C_Configuration *config)
{


	TWSR = 0x00;

	if(config->bit_rate == NormalMode)
	{
		TWBR = 32;
	}
	else if(config->bit_rate == NormalMode)
	{
		TWBR = 2;
	}


	TWCR = (1<<TWEN);

	TWAR = (config->address)<<1;

}


/*
 ***************************** 2.I2C start Function ***********************
 Function Description: This function is used to start the transfer

 */
void TWI_start(void)
{
    /*
	 * Clear the TWINT flag before sending the start bit TWINT=1
	 * send the start bit by TWSTA=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

    /* Wait for TWINT flag set in TWCR Register (start bit is send successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
}


/*
 ***************************** 3.I2C stop Function ***********************
 Function Description: This function is used to stop the transfer

 */
void TWI_stop(void)
{
    /*
	 * Clear the TWINT flag before sending the stop bit TWINT=1
	 * send the stop bit by TWSTO=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

/*
 ***************************** 4.I2C writeByte Function ***********************
 Function Description: This function is used to write byte

 */
void TWI_writeByte(uint8 data)
{
    /* Put data On TWI data Register */
    TWDR = data;
    /*
	 * Clear the TWINT flag before sending the data TWINT=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWEN);
    /* Wait for TWINT flag set in TWCR Register(data is send successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
}

/*
 ***************************** 5.I2C read byte with acknowledgment Function ***********************
 Function Description: This function is used to read byte with acknowledgment

 */
uint8 TWI_readByteWithACK(void)
{
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable sending ACK after reading or receiving data TWEA=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
    /* Read Data */
    return TWDR;
}


/*
 ***************** 6.I2C read byte with negative acknowledgment Function ******************
 Function Description: This function is used to read byte with negative acknowledgment

 */
uint8 TWI_readByteWithNACK(void)
{
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWEN);
    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
    /* Read Data */
    return TWDR;
}

/*
 ***************** 7.I2C get status Function ******************
 Function Description: This function is used to get transfer status

 */
uint8 TWI_getStatus(void)
{
    uint8 status;
    /* masking to eliminate first 3 bits and get the last 5 bits (status bits) */
    status = TWSR & 0xF8;
    return status;
}
