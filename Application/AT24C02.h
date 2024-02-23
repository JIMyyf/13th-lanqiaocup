#ifndef __AT24C02_H
#define __AT24C02_H

#include "IIC.h"
#include "main.h"

#define AT24C02_WRITE_ADDRESS   0xA0
#define AT24C02_READ_ADDRESS    0xA1

void AT24C02_Init(void);
void AT24C02_Write(unsigned char address, uint8_t data);
uint8_t AT24C02_Read(uint8_t address);

#endif
