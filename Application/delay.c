#include "delay.h"

static uint32_t fac_us = 0;

/**
 * @brief 延时函数初始化
 * @param SYSTICK
 */
void delay_init(uint8_t SYSTICK)
{
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
    fac_us = SYSTICK;
}

/**
 * @brief 微秒级延时
 * @param nus
 */
void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;
    ticks = nus * fac_us;
    told = SysTick->VAL;
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
                tcnt += told - tnow;
            else
                tcnt += reload - tnow + told;
            told = tnow;
            if (tcnt >= ticks)
                break;
        }
    }
}

/**
 * @brief 毫秒级延时
 * @param nms
 */
void delay_ms(uint16_t nms)
{
    uint32_t i;
    for (i = 0; i < nms; i++)
    {
        delay_us(1000);
    }
}
