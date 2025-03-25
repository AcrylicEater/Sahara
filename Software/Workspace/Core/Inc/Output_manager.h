/*
 * Output_manager.h
 *
 *  Created on: Mar 24, 2025
 *      Author: acryl
 */

#ifndef INC_OUTPUT_MANAGER_H_
#define INC_OUTPUT_MANAGER_H_

#include "main.h"

extern SPI_HandleTypeDef hspi1;

static const uint8_t SegVals[10] = {0b01110111,0b01000100,0b01101011,0b01101110,0b01011100,0b00111110,0b00111111,0b01100100,0b01111111,0b01111100};
typedef enum  {
	BLACK,
	RED,
	GREEN,
	BLUE,
	PURPLE,
	YELLOW,
	CYAN,
	WHITE
} Colors;
typedef enum {TEMP, TIME} SegType;
typedef enum {FAN, HEATER} RelayType;

void displayNumber(int num, SegType display);

void displayTime(uint16_t min);

void displayColor(Colors c);

void setRelay(RelayType relay, GPIO_PinState state);

void pwmHeater(float duty, int period);

#endif /* INC_OUTPUT_MANAGER_H_ */
