//#############################################################################
//
//  File:   F28027_LCD_I2C_main.c
//
//  Title:  F28027 connect to LCD via I2C main file
//
//  Created on: 2020Äê03ÔÂ09ÈÕ
//
//  Author: Jie Kang
//
//#############################################################################

#include "F28x_Project.h"
#include "F2837xD_device.h"
# include "stdio.h"
#include "LCD.h"

Uint16 Addr;
Uint16 displayfunction;
Uint16 displaycontrol;
Uint16 displaymode;
Uint16 numlines;
Uint16 cols;
Uint16 rows;
Uint16 backlightval;
Uint16 PassCount;
Uint16 FailCount;

void init_I2C(Uint16 lcd_Addr,Uint16 lcd_cols,Uint16 lcd_rows);
void init_LCD();
void backlight();
void cursor();
void blink();
void setCursor(Uint16, Uint16);
void writeNum(Uint16);
void writeStr(const char *str);
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
	GPIO_SetupPinMux(104, GPIO_MUX_CPU1, 1);
	GPIO_SetupPinMux(105, GPIO_MUX_CPU1, 1);

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
	   init_I2C(0x0027,20,4);
	    init_LCD();

//
// Step 5. User specific code
//

//
// Clear Counters
//
	PassCount = 0;
	FailCount = 0;

//
// Enable CPU INT8 which is connected to PIE group 8
//
	IER |= M_INT8;
	EINT;


    backlight();
    cursor();
    blink();
    setCursor (0,0);  // go to the top left corner
    writeStr("F28027 LCD I2C");
    setCursor (0,1);
    writeStr("It's fully function");
    setCursor (0,2);
    writeStr("Can use in project");
    setCursor (0,3);
    writeNum(5);
}






