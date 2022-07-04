/*
 * ssd1306_i2c.c
 *
 *  Created on: Apr 28, 2022
 *      Author: Alaa Elkhateeb
 */

/**
 * original author:  Tilen Majerle<tilen@majerle.eu>
 * modification for STM32f10x: Alexander Lutsai<s.lyra@ya.ru>
 * modification line 19 to line 33: GrunF
 */
#include "ssd1306_i2c.h"
#include "stdtypes.h"
#include "bitmath.h"
#include "RCC_Interface.h"
#include "GPIO_Interface.h"
#include "I2C_Interface.h"
#include "I2C_Private.h"

/* Private variables */
static uint32_t ssd1306_I2C_Timeout;

/* Private defines */
#define I2C_TRANSMITTER_MODE   0
#define I2C_RECEIVER_MODE      1
#define I2C_ACK_ENABLE         1
#define I2C_ACK_DISABLE        0

void ssd1306_I2C_Init() {

	RCC_APB2_EnableCLK(PORTB_EN);
	RCC_APB1_EnableCLK(I2C1_EN); //ENABLE RCC CLK
	RCC_APB1_EnableCLK(AFIOEN);

		GPIO_u8SetPinMode(GPIO_U8_PORTB, GPIO_U8_P6, GPIO_U8_OUT_10MHZ_AF_OD);
		GPIO_u8SetPinMode(GPIO_U8_PORTB, GPIO_U8_P7, GPIO_U8_OUT_10MHZ_AF_OD);

		I2C1_CR2_REG |= (36 & 0x3F); //  14 FREQ value in CR2 reg masked all except first 6 bits 0x3F ->11 11 11
		I2C1_CCR_REG |= (180 & 0xFFF); // 70 CCR value in CCR reg masked all except first 12 bits 0xFFF ->1111 1111 1111
		I2C1_TRISE_REG = (37 & 0x3F); //15fr0m datasheet TRISE = (FREQvalue/1000ns)+1;

		SET(I2C1_CR1_REG, 0);		//EN PERIPHERAL
		I2C1_CR1_REG |= (1 << 10);   //SETTING ACK


}

void ssd1306_I2C_WriteMulti(I2C_TypeDef *I2Cx, uint8_t address, uint8_t reg,
		uint8_t *data, uint16_t count) {
	u8 i;

	ssd1306_I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
	ssd1306_I2C_WriteData(I2Cx, reg);
	for (i = 0; i < count; i++) {
		ssd1306_I2C_WriteData(I2Cx, data[i]);
	}
	ssd1306_I2C_Stop(I2Cx);

}

/* Private functions */
int16_t ssd1306_I2C_Start(I2C_TypeDef *I2Cx, uint8_t address, uint8_t direction,
		uint8_t ack) {
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
			ssd1306_I2C_Timeout = ssd1306_I2C_TIMEOUT;
			if (--ssd1306_I2C_Timeout == 0x00) {
				return 1;
			}

		}
		/* Read status register to clear ADDR flag */
		u16 temp = (I2C1_SR2_REG)|(I2C1_SR2_REG);

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
			ssd1306_I2C_Timeout = ssd1306_I2C_TIMEOUT;
			if (ssd1306_I2C_Timeout == 0x00) {
				return 1;
			}

		}
		/* Read status register to clear ADDR flag */
		u16 temp = (I2C2_SR2_REG)|(I2C2_SR2_REG);

	}


	/* Return 0, everything ok */
	return 0;
}

void ssd1306_I2C_WriteData(I2C_TypeDef *I2Cx, uint8_t data) {
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

void ssd1306_I2C_Write(I2C_TypeDef *I2Cx, uint8_t address, uint8_t reg,
		uint8_t data) {
	ssd1306_I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
	ssd1306_I2C_WriteData(I2Cx, reg);
	ssd1306_I2C_WriteData(I2Cx, data);
	ssd1306_I2C_Stop(I2Cx);

}

uint8_t ssd1306_I2C_Stop(I2C_TypeDef *I2Cx) {
	/* Wait till transmitter not empty */
	ssd1306_I2C_Timeout = ssd1306_I2C_TIMEOUT;
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

uint8_t ssd1306_I2C_IsDeviceConnected(I2C_TypeDef *I2Cx, uint8_t address) {
	uint8_t connected = 0;
	/* Try to start, function will return 0 in case device will send ACK */
	if (!ssd1306_I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE,
	I2C_ACK_ENABLE)) {
		connected = 1;
	}

	/* STOP I2C */
	ssd1306_I2C_Stop(I2Cx);

	/* Return status */
	return connected;
}
