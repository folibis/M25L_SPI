/*
* The MIT License (MIT)
*
* Copyright (c) 20014 Moukhlynin Ruslan <ruslan@khvmntk.ru>
*
* MX25L.c
* This file is part of the MX25L SPI memory library.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#include <avr/io.h>
#include "MX25L.h"

#define SLAVESELECT (PORTB &= ~(1<<PORTB2))
#define SLAVEDESELECT (PORTB |= (1<<PORTB2))

#define COMMAND_WREN  0x06
#define COMMAND_WRDI  0x04
#define COMMAND_RDSR  0x05
#define COMMAND_WRSR  0x01
#define COMMAND_READ  0x03
#define COMMAND_FREAD 0x0B
#define COMMAND_WRITE 0x02
#define COMMAND_RDID  0x9F
#define COMMAND_SE    0x20
#define COMMAND_BE    0x52
#define COMMAND_CE    0x60

#define STATUS_WIP    0b00000001
#define STATUS_WEL    0b00000010
#define STATUS_BP0    0b00000100
#define STATUS_BP1    0b00001000
#define STATUS_BP2    0b00010000
#define STATUS_BP3    0b00100000
#define STATUS_RES    0b01000000
#define STATUS_SWRD   0b10000000

#define DUMMY         0xFF

void SPI_Init()
{
	DDRB = (1<<PORTB2)|(1<<PORTB3)|(1<<PORTB5);
	SLAVEDESELECT;
	SPSR |= (1<<SPI2X);
	SPCR |= (1<<SPE)|(1<<MSTR);
}

byte SPI_Transfer(byte data)
{
	SPDR = data;
	while (!(SPSR & (1<<SPIF)));
	return SPDR;
}

void Get_Identification(byte data[3])
{
	SLAVESELECT;
	SPI_Transfer(COMMAND_RDID);
	data[0] = SPI_Transfer(DUMMY);
	data[1] = SPI_Transfer(DUMMY);
	data[2] = SPI_Transfer(DUMMY);
	SLAVEDESELECT;
}

byte GetStatus()
{
	byte status;
	SLAVESELECT;
	SPI_Transfer(COMMAND_RDSR);
	status = SPI_Transfer(DUMMY);
	SLAVEDESELECT;
	return status;
}

void SetWriteEnable(bool enable)
{
	SLAVESELECT;
	SPI_Transfer(enable ? COMMAND_WREN : COMMAND_WRDI);
	SLAVEDESELECT;
}

void SetStatus(byte status)
{
	SetWriteEnable(true);
	SLAVESELECT;
	SPI_Transfer(COMMAND_WRSR);
	SPI_Transfer(status);
	SLAVEDESELECT;
}

void Erase(byte command, ulong address)
{
}

void SectorErase(ulong address)
{
	SetWriteEnable(true);
	SLAVESELECT;
	SPI_Transfer(SE);
	SetAddress(address);
	SLAVEDESELECT;
	while(GetStatus() & STATUS_WIP);
}

void BlockErase(ulong address)
{
	SetWriteEnable(true);
	SLAVESELECT;
	SPI_Transfer(COMMAND_BE);
	SetAddress(address);
	SLAVEDESELECT;
	while(GetStatus() & STATUS_WIP);
}

void ChipErase()
{
	SetWriteEnable(true);
	SLAVESELECT;
	SPI_Transfer(COMMAND_CE);
	SLAVEDESELECT;
	while(GetStatus() & STATUS_WIP);
}

void SetAddress(ulong address)
{
	SPI_Transfer( (byte) (address >> 16) );
	SPI_Transfer( (byte) (address >> 8) );
	SPI_Transfer( (byte) address );
}

void Write(ulong address, const char * data, uint length)
{
	SetWriteEnable(true);
	SLAVESELECT;
	SPI_Transfer(COMMAND_WRITE);
	SetAddress(address);
	for(uint i = 0;i < length;i ++)
	{
		SPI_Transfer(*(data + i));
	}
	SLAVEDESELECT;
	while(GetStatus() & STATUS_WIP);
}

void Read(ulong address, char * data, uint length)
{
	SLAVESELECT;
	SPI_Transfer(COMMAND_READ);
	SetAddress(address);
	for(uint i = 0;i < length;i ++)
	{
		*(data + i) = SPI_Transfer(DUMMY);
	}
	SLAVEDESELECT;
	while(GetStatus() & STATUS_WIP);
}

void FastRead(ulong address, char * data, uint length)
{
	SLAVESELECT;
	SPI_Transfer(COMMAND_READ);
	SetAddress(address);
	SPI_Transfer(DUMMY);
	for(uint i = 0;i < length;i ++)
	{
		*(data + i) = SPI_Transfer(DUMMY);
	}
	SLAVEDESELECT;
	while(GetStatus() & STATUS_WIP);
}

void ProtectBlock(bool bp0, bool bp1, bool bp2, bool bp3)
{
	byte status = GetStatus();
	if(bp0) status |= STATUS_BP0;
	if(bp1) status |= STATUS_BP1;
	if(bp2) status |= STATUS_BP2;
	if(bp3) status |= STATUS_BP3;
	SetStatus(status);

}