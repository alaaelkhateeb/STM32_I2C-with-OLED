/**
 ******************************************************************************
 * @file    I2C_Interface.h
 * @author  Alaa Elkhateeb
 * @brief   contains all the functions prototypes for I2C and necessary types.
 * @date     Feb 7, 2022
 ******************************************************************************
 */

#ifndef I2C_INTERFACE_H_
#define I2C_INTERFACE_H_


#ifndef I2C_TIMEOUT
#define I2C_TIMEOUT					20000
#endif


/* Private variables */
static u32 I2C_Timeout;

/* Private defines */

/*I2C Mode*/
typedef enum {
	I2C_TRANSMITTER_MODE = 0, I2C_RECEIVER_MODE
} I2C_Mode;
///////////////////////////////////////////////////////////////////////////////
/*I2C Peripheral*/
typedef enum {
	I2C1, I2C2
} I2CPeripheral;
///////////////////////////////////////////////////////////////////////////////
/* I2C_ACKNOWLEDGE */
typedef enum {
	NACK = 0, ACK
} I2C_AckBit;
///////////////////////////////////////////////////////////////////////////////
/* I2C_FASTMODE */
typedef enum {
	FASTMODE_DISABLE = 0, FASTMODE_EN
} I2C_FASTMODE;
///////////////////////////////////////////////////////////////////////////////

/* I2C_DUTY_CYCLE */
typedef enum {
	I2C_DUTY_2 = 0, I2C_DUTY_16_9
} I2C_DUTY;
///////////////////////////////////////////////////////////////////////////////
/* I2C_CLOCK_STRETCHING */
typedef enum {
	CLK_STRETCH_EN = 0, CLK_STRETCH_DISABLE
} I2C_CLK_STRETCH;
///////////////////////////////////////////////////////////////////////////////
/* I2C_ADDRESSING_MODE */
typedef enum {
	ADD_MODE_7BIT = 0, ADD_MODE_10BIT
} I2C_ADDRESSING_MODE;
///////////////////////////////////////////////////////////////////////////////
/* I2C_DUAL_ADDRESSING_MODE */
typedef enum {
	DUAL_DISABLE = 0, DUAL_EN
} I2C_DUAL_ADDRESSING_MODE;
///////////////////////////////////////////////////////////////////////////////
/* I2C_GENERAL_CALL */
typedef enum {
	GENERAL_CALL_DISABLE = 0, GENERAL_CALL_EN
} I2C_GENERAL_CALL;
///////////////////////////////////////////////////////////////////////////////
/* I2C_PEC */
typedef enum {
	PEC_DISABLE = 0, PEC_EN
} I2C_PEC;
///////////////////////////////////////////////////////////////////////////////
/* I2C_DMA_TRANSFER */
typedef enum {
	DMA_DISABLE = 0, DMA_EN
} I2C_MA_TRANSFER;
///////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
 ****                    Functions Prototypes                              ****
 ******************************************************************************/
/******************************************************************************
 * @brief 	   				Initiate I2C .
 * @param 					I2Cx: where x can be 1 or 2 to select
 * 										 the I2C peripheral
 * @param					ACK :TO ENABLE OR DISABLE ACKNOWLEDGE
 * @param					FMode: TO ENABLE FAST OR STANDARD MODE
 * @param					DutyCy:TO CHOOSE DUTY CYCLE
 * @param					ClkMode:TO ENABLE OR DISABLE CLK STRETCHING
 * @param					AddMode: TO CHOOSE 7 OR 10BIT ADD
 * @param					DAddMode: TO ENABLE OR DISABLE DUAL SLAVE ADD
 * @param					SlaveAdd1: TO INSERT 1ST ADDRESS
 * @param					SlaveAdd2: TO INSERT 2ND ADDRESS
 * @param					GCall:TO ENABLE OR DISABLE GENERAL CALL
 * @param					PacketErr:TOENABLE OR DISABLE PACKET ERROR
 * @return 					VOID
 *************************************************************************** */
void I2C_Init(I2CPeripheral I2Cx);
/******************************************************************************
 * @brief 	   				GENERATING START CONDITION.
 * @param 					I2Cx: where x can be 1 or 2 to select
 * 											the I2C peripheral
 * @param 					address: slave address
 * @param 					direction: master to slave or slave to master
 * @param 					ack: ack enabled or disabled
 * @return 					Start condition status
 *************************************************************************** */
u16 I2C_Start(I2CPeripheral I2Cx,u8 address,u8 direction,u8 ack);
/******************************************************************************
 * @brief 	   				GENERATING STOP CONDITION.
 * @param 					I2Cx: where x can be 1 or 2 to select
 * 										the I2C peripheral
 * @return 					Stop condition status
 *************************************************************************** */
u8 I2C_Stop(I2CPeripheral I2Cx);
/******************************************************************************
 * @brief 	   				TRANSMIT A BYTE OF DATA to slave.
 * @param 					I2Cx: where x can be 1 or 2 to select
 * 											 the I2C peripheral
 * @param					address: 7 bit slave address, left aligned,
 * 								 bits 7:1 are used, LSB bit is not used.
 * @param 					reg: register to write to
 * @return 					VOID
 *************************************************************************** */
void I2C_Write(I2CPeripheral I2Cx, u8 address, u8 reg ,u8 data );
/******************************************************************************
 * @brief 	   				Transmit a N byte of data.
 * @param 					I2Cx: where x can be 1 or 2 to select
 * 									the I2C peripheral
 * @param  					address: 7 bit slave address, left aligned,
 * 								bits 7:1 are used, LSB bit is not used
 * @param					reg: register to write to
 * @param					 *data: pointer to data array to write it to slave
 * @param					count: number bytes will be written
 * @return 					VOID
 *************************************************************************** */
void I2C_WriteMulti(I2CPeripheral I2Cx, u8 address, u8 reg,
		u8 *data, u16 count);
/******************************************************************************
 * @brief 	   				Receives N bytes of data .
 * @param 					I2Cx: where x can be 1 or 2 to select
 * 									the I2C peripheral
  * @param					size: number of bytes to to receive.

 * @return 					Pointer to array that holds data
 *************************************************************************** */
u8* I2C_Read(I2CPeripheral I2Cx, u8 size);
/******************************************************************************
 * @brief  					connection check
 * @param  					I2Cx: I2C used
 * @param  					address:to check
 * @@return					 connection status
  ***************************************************************************** */
u8 I2C_IsDeviceConnected(I2CPeripheral I2Cx, u8 address);

/******************************************************************************
 * @brief  					Writes to slave
 * @param  					I2Cx: I2C used
 * @param  					data: data to be sent
 * @@return					 Void
  ***************************************************************************** */
void I2C_WriteData(I2CPeripheral I2Cx, u8 data);
#endif /* I2C_INTERFACE_H_ */

