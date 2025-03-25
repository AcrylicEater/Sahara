/*
 * Input_manager.h
 *
 *  Created on: Mar 25, 2025
 *      Author: acryl
 */

#ifndef INC_INPUT_MANAGER_H_
#define INC_INPUT_MANAGER_H_

#include "main.h"
#include "Output_manager.h"


extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim1;

#define TMP102_ADR 0x48
#define TEMP_REG 0x00
#define CONFIG_REG 0x01

typedef enum {NONE, HIGHER, LOWER} encReturn;

void setupTMP102();

float readTemp();

void setupEncoder();

encReturn readEncoder(uint8_t thresh);

#endif /* INC_INPUT_MANAGER_H_ */
