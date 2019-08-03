#ifndef LCD_H_
#define LCD_H_

typedef enum {
	LCD_BACKLIGHT_OFF,
	LCD_BACKLIGHT_ON
} LCD_Backlight_State;

void LCD_Init();
void LCD_SendCommand(uint8_t);
void LCD_SendData(uint8_t);
void LCD_SendString(char*);
void LCD_Clear(void);
void LCD_Backlight(LCD_Backlight_State);

#endif /* LCD_H_ */
