## MX25L_SPI 
MX25L_SPI is a library for Macronix MX25L SPI memory.
Tested with MX25L8006E 8 MBit chip and ATMEGA8A.But that should work with other chips.

#### Usage:
```c
#include "USART.h"
#include "MX25L.h"


int main(void)
{
	SPI_Init();
	SetStatus(0x00);
	byte status = GetStatus();
	DEBUG_PRINT("\r\nGet status:%d\r\n",status);
	DEBUG_PRINT("-----------------\r\n");
	char buffer[257];
	Get_Identification((byte *)buffer);
	DEBUG_PRINT("Identification read:\r\n");
	DEBUG_PRINT("Byte 0: %x\r\n", buffer[0]);
	DEBUG_PRINT("Byte 1: %x\r\n", buffer[1]);
	DEBUG_PRINT("Byte 2: %x\r\n", buffer[2]);
	DEBUG_PRINT("-----------------\r\n");
	DEBUG_PRINT("Clearing chip ... ");
	ChipErase();
	DEBUG_PRINT("done\r\n");
	DEBUG_PRINT("-----------------\r\n");
	buffer[256] = 0;
	char ch = 0x21;
	char page = 0;
	while (1) 
    {	
		memset(buffer,ch, 256);	
		DEBUG_PRINT("Writing page %d ... ", page);
		Write(page * 256, buffer, 256);
		DEBUG_PRINT("done\r\n");
		DEBUG_PRINT("Data: %s\r\n", buffer);
		DEBUG_PRINT("Reading page %d ... ", page);
		Read(page * 256, buffer, 256);
		buffer[256] = 0;
		DEBUG_PRINT("done\r\n");
		DEBUG_PRINT("Data: %s\r\n", buffer);
		DEBUG_PRINT("\r\n\r\n");
		ch ++;
		if(ch > 0x7E) ch = 0x21;
		page ++;
		if(page > 4095) page = 0;
		_delay_ms(1000);
	}
}
```

#### Pinout:
| MX25L | ATMEGA |
|-------|--------|
| CS    | PB2    |
| MISO  | PB4    |
| MOSI  | PB3    |
| SCLK  | PB5    |

The code above uses serial port to debug purposes. It is assumed that ATMEGA UART port connected to PC using RS232 or some FTDI adapter.

| PC   | ATMEGA  |
|------|---------|
| RX   | PD1     |
| TX   | PD0     |
| GND  | GND     |

