/*
* The MIT License (MIT)
*
* Copyright (c) 20014 Moukhlynin Ruslan <ruslan@khvmntk.ru>
*
* MX25L.h
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
 
#ifndef MX25L_H_
#define MX25L_H_

#ifndef bool
	#define bool unsigned short
#endif
#ifndef byte
	#define byte unsigned char
#endif
#ifndef uint
	#define uint unsigned int
#endif
#ifndef ulong
	#define ulong unsigned long
#endif

#ifndef true
	#define true 1
#endif
#ifndef false
	#define false 0
#endif

void SPI_Init();
byte SPI_Transfer(byte data);
void Get_Identification(byte data[3]);
byte GetStatus();
void SetWriteEnable(bool enable);
void SetStatus(byte status);
void SectorErase(ulong address);
void BlockErase(ulong address);
void ChipErase();
void SetAddress(ulong address);
void Write(ulong address, const char * data, uint length);
void Read(ulong address, char * data, uint length);
void FastRead(ulong address, char * data, uint length);
void ProtectBlock(bool bp0, bool bp1, bool bp2, bool bp3);

#endif /* MX25L_H_ */