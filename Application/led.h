#ifndef __LED_H
#define __LED_H

#include "stm32g4xx.h"

void led_init(void);
void led_ON(unsigned char location);
void led_OFF(unsigned char location);
void led_Toggle(unsigned char location);

#endif