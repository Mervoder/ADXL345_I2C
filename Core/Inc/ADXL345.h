/*
 * ADXL345.h
 *
 *  Created on: 27 Haz 2023
 *      Author: Oğuz Kaan
 */

#ifndef INC_ADXL345_H_
#define INC_ADXL345_H_

#include<stm32f4xx.h>


extern I2C_HandleTypeDef hi2c1;


/************************************
 * Alt adres pini high ise addr 0x1D
 * 0x3A yazma
 * 0x3B okuma için
 *
 * Alt adres pini low ise addr 0x53
 * 0xA6 write
 * 0xA7 read
 *
 * *********************************/



#define ADXL345_I2C_SLAVE_ADDRESS 0XA6 // 0X53 << 1
#define ADXL345_POWER_CTL_REG 0x2D // power control register
#define ADXL345_DATA_FORMAT_REG 0x31 // data format register
#define DATAX0_Reg	0x32 // read only
#define DATAX1_Reg	0x33
#define DATAY0_Reg	0x34
#define DATAY1_Reg	0x35
#define DATAZ0_Reg	0x36
#define DATAZ1_Reg	0x37
#define INT_SOURCE  0x30 // interrupt register for single tap duble tap free fall etc.
#define TAP_Axes	0x2A
#define THRES_TAP	0x1D
#define DURATION	0x21
#define LATENT_TAP	0x22
#define INT_ENABLE  0x2E
#define INT_MAP		0x2F

/* Range ayarı data format registerına
	 * 0x00  -+ 2g
	 * 0x01  -+ 4g range
	 * 0x02  -+ 8g
	 * 0x03  -+ 16g
	 * */
typedef enum{
	RANGE_2G = 0x00,
	RANGE_4G = 0x01,
	RANGE_8G = 0x02,
	RANGE_16G = 0x03
}ADXL345_Range;


void Scan_I2C_Address();

void ADX345_Init(ADXL345_Range Range);

void ADXL345_Write(uint8_t wRegister , uint8_t value);

void ADXL345_Read( uint8_t rRegister, uint8_t numberOfByte);

void ADX345_Init_Tap();

void ADXL345_Get_Tap();

float ADXL345_Get_Value_X();
float ADXL345_Get_Value_Y();
float ADXL345_Get_Value_Z();




#endif /* INC_ADXL345_H_ */
