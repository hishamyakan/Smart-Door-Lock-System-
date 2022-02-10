/*						External EEPROM Driver Module
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
#include "external_eeprom.h"
#include "I2C.h"


/*
 ************************************* Functions **********************************************
 */

/*
 ***************************** 1.EEPROM Initialization Function ***********************
 Function Description: This function is used to initialize the external EEPROM Driver by:
 1-Determining the bit rate of I2C
 2-Determining the ADDRESS


  Function Inputs: void

 */

void EEPROM_init(void)
{
	I2C_Configuration myI2C;
	myI2C.bit_rate = FastMode;
	myI2C.address = 0x02;
	TWI_init(&myI2C);

}

/*
 ***************************** 2.EEPROM write byte Function ***********************
 Function Description: This function is write data in a byte of external EEPROM

  Function Inputs: uint16 address and uint8 data stored in address

 */

uint8 EEPROM_writeByte(uint16 u16addr, uint8 u8data)
{
	/* Send the Start Bit */
    TWI_start();
    if (TWI_getStatus() != TWI_START)
        return ERROR;
		
    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=0 (write) */
    TWI_writeByte((uint8)(0xA0 | ((u16addr & 0x0700)>>7)));
    if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
        return ERROR; 
		 
    /* Send the required memory location address */
    TWI_writeByte((uint8)(u16addr));
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return ERROR;
		
    /* write byte to eeprom */
    TWI_writeByte(u8data);
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return ERROR;

    /* Send the Stop Bit */
    TWI_stop();
	
    return SUCCESS;
}



/*
 ***************************** 1.EEPROM read byte Function ***********************
 Function Description: This function is read data from a byte of external EEPROM

  Function Inputs: uint16 address and uint8 data stored in address

 */
uint8 EEPROM_readByte(uint16 u16addr, uint8 *u8data)
{
	/* Send the Start Bit */
    TWI_start();
    if (TWI_getStatus() != TWI_START)
        return ERROR;
		
    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=0 (write) */
    TWI_writeByte((uint8)((0xA0) | ((u16addr & 0x0700)>>7)));
    if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
        return ERROR;
		
    /* Send the required memory location address */
    TWI_writeByte((uint8)(u16addr));
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return ERROR;
		
    /* Send the Repeated Start Bit */
    TWI_start();
    if (TWI_getStatus() != TWI_REP_START)
        return ERROR;
		
    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=1 (Read) */
    TWI_writeByte((uint8)((0xA0) | ((u16addr & 0x0700)>>7) | 1));
    if (TWI_getStatus() != TWI_MT_SLA_R_ACK)
        return ERROR;

    /* Read Byte from Memory without send ACK */
    *u8data = TWI_readByteWithNACK();
    if (TWI_getStatus() != TWI_MR_DATA_NACK)
        return ERROR;

    /* Send the Stop Bit */
    TWI_stop();

    return SUCCESS;
}
