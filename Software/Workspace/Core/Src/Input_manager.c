/*
 * Input_manager.c
 *
 *  Created on: Mar 25, 2025
 *      Author: acryl
 */
#include "Input_manager.h"

void setupTMP102(){
	uint8_t ConfigData[3] = {CONFIG_REG, 0b01100000,0b0111000};
	uint8_t TempPointer[1]={TEMP_REG};
	while(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(TMP102_ADR<<1),ConfigData,3,5)!=HAL_OK){
		  displayColor(RED);
	  }
	while(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(TMP102_ADR<<1),TempPointer,1,5)!=HAL_OK){
			  displayColor(RED);
		  }
	displayColor(GREEN);
}

float readTemp(){
	uint8_t buffer[2];
	int16_t digitalTemp;

	HAL_I2C_Master_Receive(&hi2c1, TMP102_ADR<<1, buffer, 2, 5);

    // Combine bytes to create a signed int
    digitalTemp = ((buffer[0]) << 5) | (buffer[1] >> 3);
    // Temperature data can be + or -, if it should be negative,
    // convert 13 bit to 16 bit and use the 2s compliment.
    if (digitalTemp > 0xFFF)
    {
      digitalTemp |= 0xE000;
    }
    return digitalTemp * 0.0625;
}

static uint16_t oldEncVal;
static uint16_t encVal;

void setupEncoder(){
	  HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
	  __HAL_TIM_SET_COUNTER(&htim1, 32767);
	  oldEncVal = __HAL_TIM_GET_COUNTER(&htim1);
}

encReturn readEncoder(uint8_t thresh){
	encReturn returnVal = NONE;
	encVal = __HAL_TIM_GET_COUNTER(&htim1);
	if(encVal>=oldEncVal+thresh){
		returnVal = HIGHER;
		oldEncVal=encVal;
	}
	if(encVal<=oldEncVal-thresh){
		returnVal = LOWER;
		oldEncVal=encVal;
	}
	if(oldEncVal<6553||oldEncVal>58982){
	  __HAL_TIM_SET_COUNTER(&htim1, 32767);
	  oldEncVal = __HAL_TIM_GET_COUNTER(&htim1);
	}
	return(returnVal);
}

