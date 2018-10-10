/*-----------------------------------------------------------------------------
*
* Project:        Silicon Labs Si7005 UDP Demo
*
* Copyright:      2012 Silicon Labs, Inc. (www.silabs.com)
*
* File Name:      Sensor_Si7005.c
*
* Description:    Read the temperature and humidity from a Si7005 device
*
* Revision History:
*
*   09/17/12  QHS  Initial Release
*
*----------------------------------------------------------------------------*/
#include "Si7051.h"

void Si7051_Init(uint8_t resolution)
{
	IIC_Init();
	Si7051_set_resolution(resolution);
}

void Si7051_start(void)
{
	IIC_Start();
}

void Si7051_reset(void)
{
	IIC_Start();
	IIC_Send_Byte(CMD_RESET);
	IIC_Stop();
}

void Si7051_Sleep(void)
{
	IIC_Stop();
}

uint8_t Si7051_read_user_register(void)
{
	IIC_Start();
	IIC_Send_Byte(SI705X_ADDR_W);
	IIC_Wait_Ack();
	IIC_Send_Byte(CMD_READ_REGISTER);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(SI705X_ADDR_R);
	IIC_Wait_Ack();
	uint8_t sb = IIC_Read_Byte(NACK);
	IIC_Stop();
	
	return sb;
}

void Si7051_write_user_register(uint8_t data)
{
	IIC_Start();
	IIC_Write_Byte(SI705X_ADDR_W);
	IIC_Write_Byte(CMD_WRITE_REGISTER);
	IIC_Write_Byte(data);
	IIC_Ack();
	IIC_Stop();
}

uint8_t Si7051_read_firmware_version()
{
	IIC_Start();
	IIC_Send_Byte(SI705X_ADDR_W);
	IIC_Wait_Ack();
	IIC_Send_Byte(CMD_READ_FIRMWARE_LSB);
	IIC_Wait_Ack();
	IIC_Send_Byte(CMD_READ_FIRMWARE_HSB);
	IIC_Wait_Ack();
	IIC_Send_Byte(SI705X_ADDR_R);
	IIC_Wait_Ack();
	uint8_t sb = IIC_Read_Byte(NACK);
	IIC_Stop();
	
	return sb;
}

void Si7051_set_resolution(uint8_t resolution)
{
	SI7051_Register reg;

	switch (resolution) {
	case 11:
		reg.resolution0 = 1;
		reg.resolution7 = 1;
		break;
	case 12:
		reg.resolution0 = 1;
		reg.resolution7 = 0;
		break;	
	case 13:
		reg.resolution0 = 0;
		reg.resolution7 = 1;
		break;
	case 14:
		reg.resolution0 = 0;
		reg.resolution7 = 0;
		break;
	default: 
		break;
	}
	
	Si7051_write_user_register(reg.rawData);
}

float Si7051_readT() 
{
	return Si7051_read_temperature();
}

float Si7051_read_temperature() 
{
	IIC_Start();
	IIC_Write_Byte(SI705X_ADDR_W);
	IIC_Write_Byte(0XF3);
	delay_ms(10);
	IIC_Start();
	IIC_Write_Byte(SI705X_ADDR_R);
//	IIC_Write_Byte(SI705X_ADDR_R);
	uint8_t msb = IIC_Read_Byte(ACK );
	uint8_t lsb = IIC_Read_Byte(NACK);
	IIC_Stop();
	uint16_t val = msb << 8 | lsb;

	return (175.72*val) / 65536 - 46.85;
}

float Si7051_read_temperature_master() 
{
	IIC_Start();
	IIC_Write_Byte(SI705X_ADDR_W);
	IIC_Write_Byte(CMD_MEASURE_TEMPERATURE_HOLD);
	delay_ms(10);
	IIC_Start();
	IIC_Write_Byte(SI705X_ADDR_R);
	uint8_t msb = IIC_Read_Byte(ACK );
	uint8_t lsb = IIC_Read_Byte(NACK);
	IIC_Stop();
	uint16_t val = msb << 8 | lsb;

	return (175.72*val) / 65536 - 46.85;
}
