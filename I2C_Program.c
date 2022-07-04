// ******************************************************************************
// * @file    I2C_Program.c
// * @author  Alaa Elkhateeb
// * @brief   all the functions implementation for I2C.
// * @date    Feb 7, 2022
// ******************************************************************************
// */
/*LIB Layer*/
#include "stdtypes.h"
#include "bitmath.h"
/*MCAL*/
#include "I2C_Interface.h"
#include "I2C_Private.h"
#include "RCC_Interface.h"
#include "GPIO_Interface.h"

void I2C_Init(I2CPeripheral I2Cx) {
	RCC_APB2_EnableCLK(PORTB_EN);
	RCC_APB1_EnableCLK(I2C1_EN); //ENABLE RCC CLK
	RCC_APB1_EnableCLK(AFIOEN);
	if (I2Cx == I2C1) {
		GPIO_u8SetPinMode(GPIO_U8_PORTB, GPIO_U8_P6, GPIO_U8_OUT_10MHZ_AF_OD);
		GPIO_u8SetPinMode(GPIO_U8_PORTB, GPIO_U8_P7, GPIO_U8_OUT_10MHZ_AF_OD);

		I2C1_CR2_REG |= (36 & 0x3F); //  14 FREQ value in CR2 reg masked all except first 6 bits 0x3F ->11 11 11
		I2C1_CCR_REG |= (180 & 0xFFF); // 70 CCR value in CCR reg masked all except first 12 bits 0xFFF ->1111 1111 1111
		I2C1_TRISE_REG = (37 & 0x3F); //15fr0m datasheet TRISE = (FREQvalue/1000ns)+1;

		SET(I2C1_CR1_REG, 0);		//EN PERIPHERAL
		I2C1_CR1_REG |= (ACK << 10);   //SETTING ACK
	} else if (I2Cx == I2C2) {
		GPIO_u8SetPinMode(GPIO_U8_PORTB, GPIO_U8_P8, GPIO_U8_OUT_10MHZ_AF_OD);
		GPIO_u8SetPinMode(GPIO_U8_PORTB, GPIO_U8_P9, GPIO_U8_OUT_10MHZ_AF_OD);

		I2C2_CR2_REG |= (36 & 0x3F); //  14 FREQ value in CR2 reg masked all except first 6 bits 0x3F ->11 11 11
		I2C2_CCR_REG |= (180 & 0xFFF); // 70 CCR value in CCR reg masked all except first 12 bits 0xFFF ->1111 1111 1111
		I2C2_TRISE_REG = (37 & 0x3F); //15fr0m datasheet TRISE = (FREQvalue/1000ns)+1;

		SET(I2C2_CR1_REG, 0);		//EN PERIPHERAL
		I2C2_CR1_REG |= (ACK << 10);   //SETTING ACK

	}

}

u16 I2C_Start(I2CPeripheral I2Cx, u8 address, I2C_Mode direction,
		I2C_AckBit ack) {
	if (I2Cx == I2C1) {
		SET(I2C1_CR1_REG, 8);		//GENERATE START
		while (((GET(I2C1_SR1_REG, 0)) == 0))
			;		/////START CONDITION SUCCESS
		if (ack) {
			I2C1_CR1_REG |= (1 << 10);   //SETTING ACK
		}
		/* Send write/read bit */
		if (direction == I2C_TRANSMITTER_MODE) {
			/* Send address with zero last bit */
			I2C1_DR_REG = (address) & ~(GET(I2C1_OAR1_REG, 0));
			while (((GET(I2C1_SR1_REG, 1)) == 0))
				;		/////ADDRESS RECIEVIED
		}
		if (direction == I2C_RECEIVER_MODE) {
			/* Send address with 1 last bit */
			I2C1_DR_REG = (address) | (GET(I2C1_OAR1_REG, 0));

			/* Wait till finished */
			I2C_Timeout = I2C_TIMEOUT;
			if (--I2C_Timeout == 0x00) {
				return 1;
			}

		}
		/* Read status register to clear ADDR flag */
		u8 temp = (I2C1_SR2_REG) | (I2C1_SR2_REG);

	} else if (I2Cx == I2C1) {
		SET(I2C2_CR1_REG, 8);
		while (((GET(I2C2_SR1_REG, 0)) == 0))
			;		/////START CONDITION SUCCESS
		if (ack) {
			I2C2_CR1_REG |= (1 << 10);   //SETTING ACK
		}
		/* Send write/read bit */
		if (direction == I2C_TRANSMITTER_MODE) {
			/* Send address with zero last bit */
			I2C2_DR_REG = (address) & ~(GET(I2C2_OAR1_REG, 0));
			/* Wait till finished */
			while (((GET(I2C2_SR1_REG, 1)) == 0))
				;		/////ADDRESS RECIEVIED

		}
		if (direction == I2C_RECEIVER_MODE) {
			/* Send address with 1 last bit */
			I2C2_DR_REG = (address) | (GET(I2C2_OAR1_REG, 0));

			/* Wait till finished */
			I2C_Timeout = I2C_TIMEOUT;
			if (I2C_Timeout == 0x00) {
				return 1;
			}

		}
		/* Read status register to clear ADDR flag */
		u8 temp = (I2C2_SR2_REG) | (I2C2_SR2_REG);

	}

	/* Return 0, everything ok */
	return 0;

}

u8 I2C_Stop(I2CPeripheral I2Cx) {
	I2C_Timeout = I2C_TIMEOUT;
	if (I2Cx == I2C1) {

		while ((((GET(I2C1_SR1_REG, 7)) || (GET(I2C1_SR1_REG, 2))) == 0))
			;
		SET(I2C1_CR1_REG, 9); //GENERATE STOP
	} else if (I2Cx == I2C1) {

		while ((((GET(I2C1_SR1_REG, 7)) || (GET(I2C1_SR1_REG, 2))) == 0))
			;
		SET(I2C2_CR1_REG, 9);
	}

	/* Return 0, everything ok */
	return 0;
}
void I2C_Write(I2CPeripheral I2Cx, u8 address, u8 reg, u8 data) {

	I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, NACK);
	I2C_WriteData(I2Cx, reg);
	I2C_WriteData(I2Cx, data);
	I2C_Stop(I2Cx);

}
void I2C_WriteData(I2CPeripheral I2Cx, u8 data) {
	if (I2Cx == I2C1) {
		while (!(GET(I2C1_SR1_REG, 7)))
			;

		/* Send I2C data */
		I2C1_DR_REG = data;
	} else if (I2Cx == I2C2) {
		while (!(GET(I2C2_SR1_REG, 7)))
			;

		/* Send I2C data */
		I2C2_DR_REG = data;
	}

}
void I2C_WriteMulti(I2CPeripheral I2Cx, u8 address, u8 reg, u8 *data, u16 count) {
	u8 i;

	I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, ACK);
	I2C_WriteData(I2Cx, reg);
	for (i = 0; i < count; i++) {
		I2C_WriteData(I2Cx, data[i]);
	}
	I2C_Stop(I2Cx);

}

u8 I2C_IsDeviceConnected(I2CPeripheral I2Cx, u8 address) {
	u8 connected = 0;
	/* Try to start, function will return 0 in case device will send ACK */
	if (!I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, ACK)) {
		connected = 1;
	}

	/* STOP I2C */
	I2C_Stop(I2Cx);

	/* Return status */
	return connected;
}

u8* I2C_Read(I2CPeripheral I2Cx, u8 size) {
	u8 buffer[8];
	u8 remaining = size;

	if (size == 1) {

		if (I2Cx == I2C1) {
			RESET(I2C1_CR1_REG, 10);		//CLEARING ACK
			I2C_Stop(I2C1);
			while (((GET(I2C1_SR1_REG, 6)) == 0))
				;
			buffer[size - remaining] = I2C1_DR_REG;
		} else if (I2Cx == I2C2) {

			RESET(I2C2_CR1_REG, 10);
			I2C_Stop(I2C2);
			while (((GET(I2C2_SR1_REG, 6)) == 0))
				;
			buffer[size - remaining] = I2C2_DR_REG;
		}
	} else {
		if (I2Cx == I2C1) {
			while (remaining > 2) {
				while (((GET(I2C1_SR1_REG, 6)) == 0))
					; // wait for RxNE to set

				buffer[size - remaining] = I2C1_DR_REG; // copy the data into the buffer

				SET(I2C1_CR1_REG, 10); // Set the ACK bit to Acknowledge the data received
				remaining--;
			}
			// Read the  SECOND LAST BYTE
			while (((GET(I2C1_SR1_REG, 6)) == 0))
				; // wait for RxNE to set
			buffer[size - remaining] = I2C1_DR_REG; // copy the data into the buffer
			RESET(I2C1_CR1_REG, 10);
			I2C_Stop(I2C1);
			remaining--;
			while (((GET(I2C1_SR1_REG, 6)) == 0))
				; // wait for RxNE to set
			buffer[size - remaining] = I2C1_DR_REG; // copy the data into the buffer

		} else if (I2Cx == I2C2) {
			while (remaining > 2) {
				while (((GET(I2C2_SR1_REG, 6)) == 0))
					; // wait for RxNE to set

				buffer[size - remaining] = I2C2_DR_REG; // copy the data into the buffer

				SET(I2C2_CR1_REG, 10); // Set the ACK bit to Acknowledge the data received
				remaining--;
			}
			// Read the  SECOND LAST BYTE
			while (((GET(I2C2_SR1_REG, 6)) == 0))
				; // wait for RxNE to set
			buffer[size - remaining] = I2C2_DR_REG; // copy the data into the buffer
			RESET(I2C2_CR1_REG, 10);
			I2C_Stop(I2C2);
			remaining--;
			while (((GET(I2C2_SR1_REG, 6)) == 0))
				; // wait for RxNE to set
			buffer[size - remaining] = I2C2_DR_REG; // copy the data into the buffer

		}
	}

	return buffer;
}
