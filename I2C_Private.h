
/**
  ******************************************************************************
  * @file    I2C_Private.h
  * @author  Alaa Elkhateeb
  * @brief   contains all reg addresses and macros for developers only.
  * @date    Feb 7, 2022
  ******************************************************************************
  */

#ifndef I2C_PRIVATE_H_
#define I2C_PRIVATE_H_
/*******************************************************************************
 ***                       Base Addressing                            ***
 ******************************************************************************/


#define I2C1_BASE_ADDRESS         *((volatile u32 *)(0x40005400))
#define I2C2_BASE_ADDRESS         *((volatile u32 *)(0x40005800))

/*******************************************************************************
 ***                       Registers Addressing                            ***
 ******************************************************************************/
				/*I2C1 REGISTERS ADDERSSING*/
#define	I2C1_CR1_REG				*((volatile u32 *)(0x40005400))
#define	I2C1_CR2_REG				*((volatile u32 *)(0x40005404))
#define	I2C1_OAR1_REG				*((volatile u32 *)(0x40005408))
#define	I2C1_OAR2_REG				*((volatile u32 *)(0x4000540C))
#define	I2C1_DR_REG					*((volatile u32 *)(0x40005410))
#define	I2C1_SR1_REG				*((volatile u32 *)(0x40005414))
#define	I2C1_SR2_REG				*((volatile u32 *)(0x40005418))
#define	I2C1_CCR_REG				*((volatile u32 *)(0x4000541C))
#define	I2C1_TRISE_REG				*((volatile u32 *)(0x40005420))

				/*I2C2 REGISTERS ADDERSSING*/
#define	I2C2_CR1_REG				*((volatile u32 *)(0x40005800))
#define	I2C2_CR2_REG				*((volatile u32 *)(0x40005804))
#define	I2C2_OAR1_REG				*((volatile u32 *)(0x40005808))
#define	I2C2_OAR2_REG				*((volatile u32 *)(0x4000580C))
#define	I2C2_DR_REG					*((volatile u32 *)(0x40005810))
#define	I2C2_SR1_REG				*((volatile u32 *)(0x40005814))
#define	I2C2_SR2_REG				*((volatile u32 *)(0x40005818))
#define	I2C2_CCR_REG				*((volatile u32 *)(0x4000581C))
#define	I2C2_TRISE_REG				*((volatile u32 *)(0x40005820))
////////////////////////////////////////////////////////////////////////////


#endif /* I2C_PRIVATE_H_ */
