/*
 * LCD.h
 *
 *  Created on: Jun 26, 2024
 *      Author: Vande
 */


#ifndef LCD_H_
#define LCD_H_
#include "F28x_Project.h"
#include "stdio.h"
#include "F2837xD_device.h"

//
// Defines
//
#define Cursor_on 0x0E
#define Cursor_off 0x0C
#define Cursor_blink 0x0C
#define Shift_left 0x18
#define Shift_right 0x1C
#define Cursor_left 0x10
#define Cursor_right 0x14
#define Clear        0x01
#define I2C_SLAVE_ADDR        0x27
#define I2C_NUMBYTES          1
#define I2C_EEPROM_HIGH_ADDR  0x00
#define I2C_EEPROM_LOW_ADDR   0x30
#define LCD_SETDDRAMADDR 0x80
#include "F28x_Project.h"

//
// Function Prototypes
//
void I2CA_Init(void);
Uint16 I2CA_WriteData(struct I2CMSG *msg);

void LCD_command(unsigned int c);
void LCD_write(unsigned int val);
void convert(int c);
void dec_unid_convert(unsigned numero);
void intro(void);
void DisplayLCD(char LineNumber, char columNumber, char *Message);
// Function Prototypes  I2C LCD
//**********************************
void I2CA_Init(void);
Uint16 I2CA_WriteData(struct I2CMSG *msg);
void init_LCD(void);
void I2C_out(Uint16 data);

//void atualiza_LCD(void);
// Function Prototypes  I2C LCD
#endif /* LCD_H_ */
