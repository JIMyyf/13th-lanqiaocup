#ifndef __DELAY_H
#define __DELAY_H

#include "stm32g4xx.h" // Device header
#include "stdint.h"

void delay_init(uint8_t SYSTICK);
void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);

#endif
