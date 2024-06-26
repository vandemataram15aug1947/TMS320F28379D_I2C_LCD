/*
 * LCD_main.c
 *
 *  Created on: Jun 26, 2024
 *      Author: Vande
 */

#include "F28x_Project.h"

//
// Defines **************** LCD

void init_LCD(void);

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
void I2C_out(Uint16 data);
//**********************************

// Main
//

void main(void)
{

//
// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the F2837xD_SysCtrl.c file.
//
	InitSysCtrl();
//
// Step 2. Initialize GPIO:
// This example function is found in the F2837xD_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
//
	InitGpio();

//
// For this example, only init the pins for the SCI-A port.
// These functions are found in the F2837xD_Gpio.c file.
//

//
// Step 3. Clear all __interrupts and initialize PIE vector table:
// Disable CPU __interrupts
//
	DINT;

//
// Initialize PIE control registers to their default state.
// The default state is all PIE __interrupts disabled and flags
// are cleared.
// This function is found in the F2837xD_PieCtrl.c file.
//
	InitPieCtrl();

//
// Disable CPU __interrupts and clear all CPU __interrupt flags:
//
	IER = 0x0000;
	IFR = 0x0000;

//
// Initialize the PIE vector table with pointers to the shell Interrupt
// Service Routines (ISR).
// This will populate the entire table, even if the __interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in F2837xD_DefaultIsr.c.
// This function is found in F2837xD_PieVect.c.
//
	InitPieVectTable();

//
// Step 4. Initialize the Device Peripherals:
//
	I2CA_Init();

//
// Step 5. User specific code
//

//
// Clear Counters
//

//
// Enable CPU INT8 which is connected to PIE group 8
//
	IER |= M_INT8;
	EINT;

	init_LCD();

	DisplayLCD(0, 2, "TMS3202F28379D");
	DisplayLCD(1, 2, "Nguyen Phuoc Loc");
	DisplayLCD(2, 2, "VLUTE University");
	DisplayLCD(3, 2, "locnp@gmail.com");
	LCD_command(0xc0);

}

