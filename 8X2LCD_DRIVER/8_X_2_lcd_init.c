/*
 * 8_X_2_lcd_init.c
 *
 *  Created on: May 4, 2024
 *      Author: SEIL
 */

#include"8_X_2_lcd_init.h"

void LCD_Init(void) {
    HAL_Delay(50);
    LCD_SendCommand(0x33); // Initialization sequence
    HAL_Delay(1);
    LCD_SendCommand(0x32); // Initialization sequence
    HAL_Delay(1);
    LCD_SendCommand(0x28); // 4-bit mode initialization
    HAL_Delay(1);
    LCD_SendCommand(0x0C); // Display ON, Cursor OFF
    HAL_Delay(1);
    LCD_SendCommand(0x01); // Clear display
    HAL_Delay(2);
}

void LCD_SendCommand(uint8_t command) {
    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, (command >> 4) & 0x01);
    HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, (command >> 5) & 0x01);
    HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, (command >> 6) & 0x01);
    HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, (command >> 7) & 0x01);

    HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, command & 0x01);
    HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, (command >> 1) & 0x01);
    HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, (command >> 2) & 0x01);
    HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, (command >> 3) & 0x01);

    HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_RESET);
}

void LCD_SendData(uint8_t data) {
    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET);

    HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, (data >> 4) & 0x01);
    HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, (data >> 5) & 0x01);
    HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, (data >> 6) & 0x01);
    HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, (data >> 7) & 0x01);

    HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, data & 0x01);
    HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, (data >> 1) & 0x01);
    HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, (data >> 2) & 0x01);
    HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, (data >> 3) & 0x01);

    HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_RESET);
}
void LCD_ClearScreen() {
    // Send command to clear the screen
    LCD_SendCommand(0x01);
    HAL_Delay(2); // Delay after clearing the screen
}
void LCD_SendString(char *str) {
    while (*str) {
        LCD_SendData(*str++);
    }
}
//LCD_SendCommand(0xC0);
void LCD_PrintVoltageCurrent(uint32_t voltage, uint32_t current) {
    char buffer[16]; // Assuming a maximum length of 16 characters for the formatted string
    // Format the string for voltage
    if (voltage < 10) {
        sprintf(buffer, "00%luV", (unsigned long)voltage);
    } else if (voltage < 100) {
        sprintf(buffer, "0%luV", (unsigned long)voltage);
    } else {
        sprintf(buffer, "%luV", (unsigned long)voltage);
    }

    // Format the string for current
    if (current < 10) {
        sprintf(buffer + strlen(buffer), "0.%luA", (unsigned long)current);
    } else {
        sprintf(buffer + strlen(buffer), "%lu.%luA", (current/10) % 10, current % 10);
    }

    LCD_SendString(buffer);
}
void LCD_Print_Current_Calibration(uint32_t current1, uint32_t current2) {
    LCD_SendCommand(0xC0); // Move the cursor to the desired position

    char buffer[16]; // Buffer to hold the formatted string
    memset(buffer, 0, sizeof(buffer)); // Ensure the buffer is empty

    if (current1 < 10) {
        sprintf(buffer, "0.%luA", current1);
    } else if (current1 < 100) {
        sprintf(buffer, "%lu.%luA", (current1/10) % 10, current1 % 10);
    }

    if (current2 < 10) {
        sprintf(buffer + strlen(buffer), "0.%luA", current2);
    } else if (current2 < 100) {
        sprintf(buffer + strlen(buffer), "%lu.%luA", (current2/10) % 10, current2 % 10);
    }

    // Print the formatted string
    LCD_SendString(buffer);
}

void LCD_Print_Voltage_Calibration(uint32_t voltage1, uint32_t voltage2) {
    LCD_SendCommand(0xC0); // Move the cursor to the desired position

    char buffer[16]; // Buffer to hold the formatted string
    memset(buffer, 0, sizeof(buffer)); // Ensure the buffer is empty

    // Format the string for voltage1
    if (voltage1 < 10) {
        sprintf(buffer, "00%luV", (unsigned long)voltage1);
    } else if (voltage1 < 100) {
        sprintf(buffer, "0%luV", (unsigned long)voltage1);
    } else {
        sprintf(buffer, "%luV", (unsigned long)voltage1);
    }

    // Format the string for voltage2 and append it to the buffer
    if (voltage2 < 10) {
        sprintf(buffer + strlen(buffer), "00%luV", (unsigned long)voltage2);
    } else if (voltage2 < 100) {
        sprintf(buffer + strlen(buffer), "0%luV", (unsigned long)voltage2);
    } else {
        sprintf(buffer + strlen(buffer), "%luV", (unsigned long)voltage2);
    }

    // Print the formatted string
    LCD_SendString(buffer);
}
