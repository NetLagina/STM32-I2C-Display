/*
 * led.c
 *
 *  Created on: Jul 16, 2019
 *      Author: NetL
 */

#include "gpio.h"

#include "led.h"

void LED_State(Led_State led_state) {
	if (LED_ON == led_state) {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	} else if (LED_OFF == led_state) {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	}
}
