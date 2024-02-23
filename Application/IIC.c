#include "IIC.h"

void IIC_WRITE_SCL(uint8_t BitValue)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, (GPIO_PinState)BitValue);
    delay_us(5);
}

void IIC_WRITE_SDA(uint8_t BitValue)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, (GPIO_PinState)BitValue);
    delay_us(5);
}

uint8_t IIC_READ_SDA(void)
{
    uint8_t value;
    value = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7);
    delay_us(5);
    return value;
}

void IIC_Init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStructure.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void IIC_Start(void)
{
    IIC_WRITE_SDA(1);
    IIC_WRITE_SCL(1);
    IIC_WRITE_SDA(0);
    IIC_WRITE_SCL(0);
}

void IIC_Stop(void)
{
    IIC_WRITE_SDA(0);
    IIC_WRITE_SCL(1);
    IIC_WRITE_SDA(1);
}

void IIC_SendByte(uint8_t Byte)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        IIC_WRITE_SDA(Byte & (0x80 >> i));
        IIC_WRITE_SCL(1);
        IIC_WRITE_SCL(0);
    }
}

uint8_t IIC_ReceiveByte(void)
{
    uint8_t Byte;
    IIC_WRITE_SDA(1);
    for (int i = 0; i < 8; i++)
    {
        IIC_WRITE_SCL(1);
        if (IIC_READ_SDA() == 1)
            Byte |= (0x80 >> i);
        IIC_WRITE_SCL(0);
    }
    return Byte;
}

void IIC_SendAck(uint8_t AckBit)
{
    IIC_WRITE_SDA(AckBit);
    IIC_WRITE_SCL(1);
    IIC_WRITE_SCL(0);
}

uint8_t IIC_ReceiveAck(void)
{
    uint8_t AckBit;
    IIC_WRITE_SDA(1);
    IIC_WRITE_SCL(1);
    AckBit = IIC_READ_SDA();
    IIC_WRITE_SCL(0);
    return AckBit;
}
