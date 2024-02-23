#include "AT24C02.h"

void AT24C02_Init(void)
{
    IIC_Init();
}

void AT24C02_Write(unsigned char address, uint8_t data)
{
    IIC_Start();
    IIC_SendByte(AT24C02_WRITE_ADDRESS);
    if (IIC_ReceiveAck() != 0)
        Error_Handler();
    IIC_SendByte(address);
    if (IIC_ReceiveAck() != 0)
        Error_Handler();
    IIC_SendByte(data);
    if (IIC_ReceiveAck() != 0)
        Error_Handler();
    IIC_Stop();
}

uint8_t AT24C02_Read(uint8_t address)
{
    uint8_t data;
    IIC_Start();
    IIC_SendByte(AT24C02_WRITE_ADDRESS);
    if (IIC_ReceiveAck() != 0)
        Error_Handler();
    IIC_SendByte(address);
    if (IIC_ReceiveAck() != 0)
        Error_Handler();
    IIC_Start();
    IIC_SendByte(AT24C02_READ_ADDRESS);
    if (IIC_ReceiveAck() != 0)
        Error_Handler();
    data = IIC_ReceiveByte();
    IIC_SendAck(1);
    IIC_Stop();
    return data;
}
