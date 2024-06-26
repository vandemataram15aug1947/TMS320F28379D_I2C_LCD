#include "LCD.h"

//
// Globals
//
struct I2CMSG I2cMsgOut1 = { I2C_MSGSTAT_SEND_WITHSTOP,
I2C_SLAVE_ADDR,
								I2C_NUMBYTES };

struct I2CMSG I2cMsgIn1 = { I2C_MSGSTAT_SEND_NOSTOP,
I2C_SLAVE_ADDR,
							I2C_NUMBYTES };

struct I2CMSG *CurrentMsgPtr;

void I2CA_Init(void)
{
	CurrentMsgPtr = &I2cMsgOut1;
	GPIO_SetupPinMux(104, GPIO_MUX_CPU1, 1);
	GPIO_SetupPinMux(105, GPIO_MUX_CPU1, 1);
	I2caRegs.I2CSAR.all = 0x0027;     // Slave address - EEPROM control code

	I2caRegs.I2CPSC.all = 6;          // Prescaler - need 7-12 Mhz on module clk
	I2caRegs.I2CCLKL = 140;            // Prescalers set for 100kHz bit rate
	I2caRegs.I2CCLKH = 140;             //At a 100khz I2C clock
	I2caRegs.I2CIER.all = 0x00;       // DESAable SCD & ARDY __interrupts
	I2caRegs.I2CMDR.all = 0x0020;     // Take I2C out of reset
	I2caRegs.I2CFFTX.all = 0x6000;    // Enable FIFO mode and TXFIFO
	I2caRegs.I2CFFRX.all = 0x2040;    // Enable RXFIFO, clear RXFFINT,

	return;
}

Uint16 I2CA_WriteData(struct I2CMSG *msg)
{
	Uint16 i;

	//
	// Wait until the STP bit is cleared from any previous master communication.
	// Clearing of this bit by the module is delayed until after the SCD bit is
	// set. If this bit is not checked prior to initiating a new message, the
	// I2C could get confused.
	//
	if (I2caRegs.I2CMDR.bit.STP == 1)
	{
		return I2C_STP_NOT_READY_ERROR;
	}

	//
	// Setup slave address
	//
	I2caRegs.I2CSAR.all = msg->SlaveAddress;

	//
	// Check if bus busy
	//
	if (I2caRegs.I2CSTR.bit.BB == 1)
	{
		return I2C_BUS_BUSY_ERROR;
	}

	//
	// Setup number of bytes to send
	// MsgBuffer + Address
	//
	I2caRegs.I2CCNT = msg->NumOfBytes + 2;

	//
	// Setup data to send
	//
	I2caRegs.I2CDXR.all = msg->MemoryHighAddr;
	I2caRegs.I2CDXR.all = msg->MemoryLowAddr;

	for (i = 0; i < msg->NumOfBytes; i++)
	{
		I2caRegs.I2CDXR.all = *(msg->MsgBuffer + i);
	}

	//
	// Send start as master transmitter
	//
	I2caRegs.I2CMDR.all = 0x6E20;

	return I2C_SUCCESS;
}
void I2C_out(Uint16 data)
{

	*CurrentMsgPtr->MsgBuffer = data;  // 36  ASCII
	I2CA_WriteData(&I2cMsgOut1);
}
/*----------LCD-------------------------------------*/
void init_LCD(void)
{

	DELAY_US(30000);
	LCD_command(0x02);
	DELAY_US(40);
	LCD_command(0x28);
	DELAY_US(40);
	LCD_command(0x0E);
	DELAY_US(40);
	LCD_command(0x06);
	DELAY_US(1500);
	LCD_command(0x01);

}

void LCD_command(unsigned int c)
{

	unsigned int commandEo;
	unsigned int commandEi;
	commandEo = 0x08;       //LED = 1, E = 0, RW = 0, RS = 0
	commandEi = 0x0C;       //LED = 1, E = 1, RW = 0, RS = 0

	unsigned int result = 0;

	result = (c & 0xf0) | commandEi;
	I2C_out(result);
	result = (c & 0xf0) | commandEi;
	I2C_out(result);

	result = (c & 0xf0) | commandEo;
	I2C_out(result);
	result = (c & 0xf0) | commandEo;
	I2C_out(result);

	result = (c & 0xf0) | commandEi;
	I2C_out(result);
	result = (c & 0xf0) | commandEi;
	I2C_out(result);

	DELAY_US(4000);

	result = (c << 4) | commandEi;
	I2C_out(result);
	result = (c << 4) | commandEi;
	I2C_out(result);

	result = (c << 4) | commandEo;
	I2C_out(result);
	result = (c << 4) | commandEo;
	I2C_out(result);

	result = (c << 4) | commandEi;
	I2C_out(result);
	result = (c << 4) | commandEi;
	I2C_out(result);

	DELAY_US(4000);

}

void LCD_write(unsigned int c)
{

	unsigned int commandEo;
	unsigned int commandEi;

	commandEo = 0x09;       //LED = 1, E = 0, RW = 0, RS = 1
	commandEi = 0x0D;       //LED = 1, E = 1, RW = 0, RS = 1

	unsigned int result = 0;

	result = (c & 0xf0) | commandEi;
	I2C_out(result);
	result = (c & 0xf0) | commandEi;
	I2C_out(result);

	result = (c & 0xf0) | commandEo;
	I2C_out(result);
	result = (c & 0xf0) | commandEo;
	I2C_out(result);

	result = (c & 0xf0) | commandEi;
	I2C_out(result);
	result = (c & 0xf0) | commandEi;
	I2C_out(result);

	DELAY_US(4000);

	result = (c << 4) | commandEi;
	I2C_out(result);
	result = (c << 4) | commandEi;
	I2C_out(result);

	result = (c << 4) | commandEo;
	I2C_out(result);
	result = (c << 4) | commandEo;
	I2C_out(result);

	result = (c << 4) | commandEi;
	I2C_out(result);
	result = (c << 4) | commandEi;
	I2C_out(result);

	DELAY_US(4000);

}       // end LCD
void DisplayLCD(char LineNumber, char columNumber, char *Message)
{
	char numlines;
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if (LineNumber > numlines)
	{
		LineNumber = numlines - 1;    // we count rows starting w/0
	}
	LCD_command((LCD_SETDDRAMADDR | (columNumber + row_offsets[LineNumber])));

	while (*Message != 0)
	{
		LCD_write(*Message++);
	}
	return;
}

/*
 void atualiza_LCD(void){
 if(relogio > 80000){
 relogio = 0;

 LCD_command(0x8A);

 dec_unid_convert(Iomed);

 convert(decimal);
 convert(unidade);

 LCD_command(0xCA);

 dec_unid_convert(Vomed);

 convert(decimal);
 convert(unidade);
 }
 }*//*
 * LCD.C
 *
 *  Created on: May 4, 2022
 *      Author: LOCNGUYEN
 */

