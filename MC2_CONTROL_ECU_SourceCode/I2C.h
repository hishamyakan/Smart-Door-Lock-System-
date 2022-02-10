/*						I2C Driver Module
 ******************************************************************************************
 ******************************************************************************************
 Written by: Hisham Yakan Abuabkr
 Project: Final Project
 File type: Module header file
  */

#ifndef I2C_H_
#define I2C_H_


/*
 ************************************** Included files******************************************
 */
#include "std_types.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */


/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/


/*Enumeration to determine the mode of operation of I2C
 * I2C in atmega16 has 2 modes
 * FastMode and Normal Mode
 * */
typedef enum{
	NormalMode = 100000, FastMode = 400000
}I2C_bitRate;




/*Configuration Structure for timer0 */
typedef struct
{
	I2C_bitRate bit_rate;

	uint8 address;

}I2C_Configuration;


/*
 ************************************* Functions *******************************************
 */


/*
 ***************************** 1.I2C Initialization Function ***********************
 Function Description: This function is used to initialize the I2C Driver

  Function Inputs: A pointer to structure of type I2C_Configuration
  Function return: void

 */
void TWI_init(I2C_Configuration* config);


/*
 ***************************** 2.I2C start Function ***********************
 Function Description: This function is used to start the transfer

 */
void TWI_start(void);

/*
 ***************************** 3.I2C stop Function ***********************
 Function Description: This function is used to stop the transfer

 */
void TWI_stop(void);

/*
 ***************************** 4.I2C writeByte Function ***********************
 Function Description: This function is used to write byte

 */

void TWI_writeByte(uint8 data);

/*
 ***************************** 5.I2C read byte with acknowledgment Function ***********************
 Function Description: This function is used to read byte with acknowledgment

 */
uint8 TWI_readByteWithACK(void);


/*
 ***************** 6.I2C read byte with negative acknowledgment Function ******************
 Function Description: This function is used to read byte with negative acknowledgment

 */
uint8 TWI_readByteWithNACK(void);


/*
 ***************** 7.I2C get status Function ******************
 Function Description: This function is used to get transfer status

 */
uint8 TWI_getStatus(void);


#endif /* I2C_H_ */
