/*
 * 8_X_2_lcd_init.h
 *
 *  Created on: May 4, 2024
 *      Author: SEIL
 */

#ifndef INC_8_X_2_LCD_INIT_H_
#define INC_8_X_2_LCD_INIT_H_

#include"main.h"



#define LCD_RS_Pin        GPIO_PIN_1
#define LCD_RS_GPIO_Port  GPIOA
#define LCD_EN_Pin        GPIO_PIN_0
#define LCD_EN_GPIO_Port  GPIOA
#define LCD_D4_Pin        GPIO_PIN_6
#define LCD_D4_GPIO_Port  GPIOA
#define LCD_D5_Pin        GPIO_PIN_7
#define LCD_D5_GPIO_Port  GPIOA
#define LCD_D6_Pin        GPIO_PIN_0
#define LCD_D6_GPIO_Port  GPIOB
#define LCD_D7_Pin        GPIO_PIN_1
#define LCD_D7_GPIO_Port  GPIOB

#define LCD_COMMAND       0
#define LCD_DATA          1

void LCD_Init(void);
void LCD_SendCommand(uint8_t command);
void LCD_SendData(uint8_t data);
void LCD_SendString(char *str);
void LCD_ClearScreen();
void LCD_PrintVoltageCurrent(uint32_t voltage ,uint32_t current );
void LCD_Print_Current_Calibration(uint32_t current1, uint32_t current2);
void LCD_Print_Voltage_Calibration(uint32_t voltage1, uint32_t voltage2);


#endif /* INC_8_X_2_LCD_INIT_H_ */
