/*
 * Output_manager.c
 *
 *  Created on: Mar 24, 2025
 *      Author: acryl
 */
#include "Output_manager.h"

void displayNumber(int num, SegType display){
	uint8_t SegBuf[3];
	HAL_GPIO_WritePin(SRCLR_GPIO_Port, SRCLR_Pin, 0);
	HAL_GPIO_WritePin(SRCLR_GPIO_Port, SRCLR_Pin, 1);
	if(display==TEMP && num<100){
		SegBuf[0]=SegVals[num%10];
		SegBuf[1]=SegVals[num/10];
		HAL_SPI_Transmit(&hspi1, SegBuf, 2, 10);
		HAL_GPIO_WritePin(TEMP_LATCH_GPIO_Port, TEMP_LATCH_Pin, 1);
		HAL_GPIO_WritePin(TEMP_LATCH_GPIO_Port, TEMP_LATCH_Pin, 0);
		return;
	}
	if(display==TIME && num<1000){
		SegBuf[0]=SegVals[num%10];
		SegBuf[1]=SegVals[num&100];
		SegBuf[2]=SegVals[num/100];
		HAL_SPI_Transmit(&hspi1, SegBuf, 3, 10);
		HAL_GPIO_WritePin(TEMP_LATCH_GPIO_Port, TEMP_LATCH_Pin, 1);
		HAL_GPIO_WritePin(TEMP_LATCH_GPIO_Port, TEMP_LATCH_Pin, 0);
		return;
	}
	displayColor(RED);
}

void displayTime(uint16_t min){
	if(min>599){
		displayColor(RED);
		return;
	}
	uint8_t h = min/60;
	min = min%60;
	displayNumber((h*100)+min,TIME);

}

void displayColor(Colors c){
	switch(c){
	case BLACK:
		HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, 1);
		HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, 1);
		HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, 1);
		break;
	case RED:
		HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, 0);
		HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, 1);
		HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, 1);
		break;
	case GREEN:
		HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, 1);
		HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, 0);
		HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, 1);
		break;
	case BLUE:
		HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, 1);
		HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, 1);
		HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, 0);
		break;
	case PURPLE:
		HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, 0);
		HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, 1);
		HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, 0);
		break;
	case YELLOW:
		HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, 0);
		HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, 0);
		HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, 1);
		break;
	case CYAN:
		HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, 1);
		HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, 0);
		HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, 0);
		break;
	case WHITE:
		HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, 0);
		HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, 0);
		HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, 0);
		break;
	}
}

void setRelay(RelayType relay, GPIO_PinState state){
	if(relay==FAN){
		HAL_GPIO_WritePin(FAN_RELAY_GPIO_Port,FAN_RELAY_Pin,state);
		return;
	}
	if(relay==HEATER){
		HAL_GPIO_WritePin(HEATER_RELAY_GPIO_Port,HEATER_RELAY_Pin,state);
		return;
	}
	displayColor(RED);
}

static uint32_t nxtTargetSW = 0;

void pwmHeater(float duty, int period){
	if(duty<0.0 || duty>1.0){
		displayColor(RED);
		return;
	}
	uint32_t period_ms = period * 1000;
	uint32_t onTime = period_ms * duty;
	uint32_t offTime = period_ms - onTime;
	uint32_t CurrTime = HAL_GetTick();
	if(CurrTime>nxtTargetSW){
		if(HAL_GPIO_ReadPin(HEATER_RELAY_GPIO_Port, HEATER_RELAY_Pin)==0){
			setRelay(HEATER,1);
			nxtTargetSW = CurrTime + onTime;
		}else{
			setRelay(HEATER,0);
			nxtTargetSW = CurrTime + offTime;
		}
	}
}
