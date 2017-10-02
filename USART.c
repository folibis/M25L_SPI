/*
* The MIT License (MIT)
*
* Copyright (c) 20014 Moukhlynin Ruslan <ruslan@khvmntk.ru>
*
* USART.c
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
#include "USART.h"
#include <avr/io.h>
#include <stdio.h>

void USART_init(unsigned int speed)
{
	UBRRH = (unsigned char)(speed>>8);
	UBRRL = (unsigned char)(speed);
	UCSRB = (1<<RXEN) | (1<<TXEN)|(1<<RXCIE);
	UCSRA |= (1<<U2X);
	UCSRC = (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
}

void USART_TX(unsigned char data)
{
	while(!(UCSRA&(1<<UDRE)));
	UDR = data;
}

void USART_Send(const char * data)
{
	while(*data != 0x0)
		USART_TX(*data++);
}

void DEBUG_PRINT(char *str, ...)
{
	static char initialized = 0;
	static char buffer[256]; 
	if(!initialized)
	{
		USART_init(51);
		initialized = 1;
	}
	va_list args;
	va_start (args, str);
	vsprintf (buffer, str, args);
	USART_Send(buffer);
	va_end (args);
}