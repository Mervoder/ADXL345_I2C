/*
 * ADXL345.c
 *
 *  Created on: 27 Haz 2023
 *      Author: Oğuz Kaan
 */

#include"ADXL345.h"


extern I2C_HandleTypeDef hi2c1;

uint8_t buff[6]; // 0-1 for x , 2-3 for y , 4-5 for z

int16_t x, y , z;
float xG,yG, zG, multipler_for_g;
uint8_t status;



// I2C tarama fonksiyonu
void Scan_I2C_Address()
{
	for(int i = 0; i<=255; i++) // 8 bit olan adresleri deniyoruz
	{
		if(HAL_I2C_IsDeviceReady(I2C1, i, 1, 10) == HAL_OK)
		{
			// cihazı bulduk
			break;	}
	}
}

void ADXL345_Read(uint8_t rRegister, uint8_t numberOfByte)
{

	HAL_I2C_Mem_Read(&hi2c1, ADXL345_I2C_SLAVE_ADDRESS, rRegister, 1, buff, numberOfByte, 100);
}


void ADXL345_Write(uint8_t wRegister , uint8_t value)
{
	uint8_t data[2] = {0};
	data[0] = wRegister;
	data[1] = value;

	HAL_I2C_Master_Transmit(&hi2c1, ADXL345_I2C_SLAVE_ADDRESS, data, 2, 100);
}

// ADXL345 init fonksiyonu
void ADX345_Init(ADXL345_Range Range)
{

/* 0x00 DEVID register (read only)
 * bu registeri okuduğunda 0xE5 döner ise cihaz normal şekilde çalışıyor demek
 *
 * */
	ADXL345_Read(0x00, 1); // 0xe5 default val
	ADXL345_Write(ADXL345_POWER_CTL_REG, 0); // Reset Power control all bits

	/* Setup -
	 * Power_Ctl_Reg de Measure biti aktifleştir
	 * link-sleep-wakeup bitleri 0
	 * 0x08
	 * */
	ADXL345_Write(ADXL345_POWER_CTL_REG, 0x08);

	/* Range ayarı data format registerına
	 * 0x00  -+ 2g
	 * 0x01  -+ 4g range
	 * 0x02  -+ 8g
	 * 0x03  -+ 16g
	 * */
	ADXL345_Write(ADXL345_DATA_FORMAT_REG, Range); // set +-16g

	if(Range == RANGE_16G )
	{
		multipler_for_g = 0.345;
	}
	else if (Range == RANGE_8G){
		multipler_for_g = 0.175;
	}
	else if (Range == RANGE_4G){
		multipler_for_g = 0.087;
	}
	else	multipler_for_g = 0.043;



}



float ADXL345_Get_Value_X()
{

	  ADXL345_Read(0x32, 6);
	  x = (buff[1] << 8) | (buff[0]);      // x values in buff [0] & buff[1]
	  xG = x*multipler_for_g ;

	  return xG;
}

float ADXL345_Get_Value_Y()
{
	  ADXL345_Read(0x32, 6);
	  y = (buff[3] << 8) | (buff[2]);      // x values in buff [2] & buff[3]
	  yG = y*multipler_for_g ;

	return yG;
}

float ADXL345_Get_Value_Z()
{
	  ADXL345_Read(0x32, 6);

	  z = (buff[5] << 8) | (buff[4]);      // x values in buff [4] & buff[5]
	  zG = z*multipler_for_g;

	  return zG;
}


void ADX345_Init_Tap()
{
	ADXL345_Read(0x00, 1); // 0xe5 default val
	ADXL345_Write(ADXL345_POWER_CTL_REG, 0); // Reset Power control all bits

	ADXL345_Write(ADXL345_POWER_CTL_REG, 0x08);

	// enable tap to z axis

	ADXL345_Write(TAP_Axes, 0x1);

	// tap thresh -- 0.625mg/lsb ----   40*0.0625 = 2.5g
	ADXL345_Write(THRES_TAP, 40 );

	// duration of tap **** 625us/LSB --- 625us*32 = 0,02 sec

	ADXL345_Write(DURATION, 32);

	// latent between 2 tap  1.25 ms/LBS 1.25ms*80 = 0.01s
	ADXL345_Write(LATENT_TAP, 80);

	// interrupt enable
	ADXL345_Write(INT_MAP, 0x00 );
	// single tap and double tap enable
	ADXL345_Write(INT_ENABLE, 0x60);

}

/*
 * Write down in if statement what you want to do
 * */
void ADXL345_Get_Tap()
{
	ADXL345_Read(INT_SOURCE, 1);

// buff[0]= data ready-single tap-double tap-activity-inactivity-free fall-water mark- overrun


	if((buff[0]>>6) & 0x01 ) // single tap
	{
		//  led on
	}
	else if ((buff[0] >>5 ) & 0x01) // double tap
	{
		// led on
	}

}





