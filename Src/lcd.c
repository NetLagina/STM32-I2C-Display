#include "stm32f1xx.h"
#include "i2c.h"

#include "main.h"
#include "lcd.h"

const uint8_t LCD_ADDRESS = 0x27 << 1;
const uint8_t PIN_RS = 0x01;
const uint8_t PIN_EN = 0x04;
const uint8_t BACKLIGHT = 0x08;

void I2C_Scan(void) {
	HAL_StatusTypeDef res;
	for (uint16_t i = 0; i < 128; i++) {
		res = HAL_I2C_IsDeviceReady(&hi2c1, i << 1, 1, 10);
		if (res == HAL_OK) {
			return;
		}
	}
}

HAL_StatusTypeDef LCD_SendInternal(uint8_t data, uint8_t flags) {
	HAL_StatusTypeDef res;
	do {
		res = HAL_I2C_IsDeviceReady(&hi2c1, LCD_ADDRESS, 1, HAL_MAX_DELAY);
		if (res == HAL_OK) {
			break;
		}
	} while (1);

	uint8_t up = data & 0xF0;
	uint8_t lo = (data << 4) & 0xF0;

	uint8_t data_arr[4];
	data_arr[0] = up | flags | BACKLIGHT | PIN_EN;
	data_arr[1] = up | flags | BACKLIGHT;
	data_arr[2] = lo | flags | BACKLIGHT | PIN_EN;
	data_arr[3] = lo | flags | BACKLIGHT;

	res = HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, data_arr, sizeof(data_arr), HAL_MAX_DELAY);
	HAL_Delay(10);
	return res;
}

void LCD_Backlight(LCD_Backlight_State state) {
	uint8_t data[1] = { 0b1000 };
	if (LCD_BACKLIGHT_OFF == state) {
		data[0] = 0;
	}
	HAL_I2C_IsDeviceReady(&hi2c1, LCD_ADDRESS, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, data, 1, HAL_MAX_DELAY);
}

void LCD_SendCommand(uint8_t cmd) {
	LCD_SendInternal(cmd, 0);
}

void LCD_SendData(uint8_t data) {
	LCD_SendInternal(data, PIN_RS);
}

void LCD_Clear(void) {
	LCD_SendCommand(0x01);
}

void LCD_Init() {
	HAL_Delay(50);
	LCD_SendCommand(0b00110000);
	HAL_Delay(20);
	LCD_SendCommand(0b00110000);
	HAL_Delay(20);
	LCD_SendCommand(0b00110000);
	HAL_Delay(20);
	LCD_SendCommand(0b00100000);
	HAL_Delay(2);

	LCD_SendCommand(0b00100010);
	HAL_Delay(2);
	LCD_SendCommand(0b00001000);
	HAL_Delay(2);
	LCD_SendCommand(0b00000001);
	HAL_Delay(2);
	LCD_SendCommand(0b00000110);
	HAL_Delay(2);
	LCD_SendCommand(0b00001111);
	HAL_Delay(2);
}

void LCD_SendString(char *str) {
	while (*str) {
		LCD_SendData((uint8_t) (*str));
		str++;
	}
}
