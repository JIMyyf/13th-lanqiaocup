#include "led.h"

unsigned char led_buff = 0x00;
unsigned int led_state[8] = {0};

void led_lock(void)
{
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}

void led_init(void)
{
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 |
                             GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

    HAL_GPIO_WritePin(GPIOC,
                      GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 |
                          GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15,
                      GPIO_PIN_SET); // close all the leds

    GPIO_InitStructure.Pin = GPIO_PIN_2;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
    led_lock();
}

void led_ON(unsigned char location)
{
    uint16_t temp = GPIOC->ODR;

    HAL_GPIO_WritePin(GPIOC,
                      GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 |
                          GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15,
                      GPIO_PIN_SET); // close all the led at first
    led_state[location - 1] = 0x01;
    led_buff = 0x00;
    for (int i = 0; i < 8; i++)
        led_buff |= (led_state[i] & 0x01) << i;
    HAL_GPIO_WritePin(GPIOC, led_buff << 8, GPIO_PIN_RESET);

    led_lock();

    GPIOC->ODR = temp;
}

void led_OFF(unsigned char location)
{
    uint16_t temp = GPIOC->ODR;

    HAL_GPIO_WritePin(GPIOC,
                      GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 |
                          GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15,
                      GPIO_PIN_SET); // close all the led at first
    led_state[location - 1] = 0x00;
    led_buff = 0x00;
    for (int i = 0; i < 8; i++)
        led_buff |= (led_state[i] & 0x01) << i;
    HAL_GPIO_WritePin(GPIOC, led_buff << 8, GPIO_PIN_RESET);

    led_lock();

    GPIOC->ODR = temp;
}

void led_Toggle(unsigned char location)
{
    if (led_state[location - 1] == 0x00)
        led_ON(location);
    else
        led_OFF(location);
}
