/*-----------------------------------------------------------------------------
*
* Project:        Silicon Labs Si7005 UDP Demo
*
* Copyright:      2012 Silicon Labs, Inc. (www.silabs.com)
*
* File Name:      Sensor.h
*
* Description:    Read the temperature and humidity from a sensor device
*
* Revision History:
*
*   09/17/12  QHS  Initial Release
*
*----------------------------------------------------------------------------*/
#ifndef SI7051_H__
#define SI7051_H__

#include "myiic.h"
#include "delay.h"

/* I2C slave address of Si705x */
#define SI705X_ADDR_R      0x81
#define SI705X_ADDR_W      0x80

/* Commands */
#define CMD_MEASURE_TEMPERATURE_HOLD    0xE3
#define CMD_MEASURE_TEMPERATURE_NO_HOLD 0xF3
#define CMD_RESET                       0xFE
#define CMD_WRITE_REGISTER              0xE6
#define CMD_READ_REGISTER               0xE7
#define CMD_READ_FIRMWARE_LSB           0X84
#define CMD_READ_FIRMWARE_HSB           0XB8

#pragma anon_unions
typedef union {
	uint8_t rawData;
	struct {
		uint8_t resolution0 : 1;
		uint8_t reserve1 : 4;
		uint8_t vdds : 1; // vdds = 1 if and only if VDD between 1.8V and 1.9V
		uint8_t reserved2 : 1;
		uint8_t resolution7 : 1;
	};
} SI7051_Register;

void    Si7051_Init(uint8_t resolution);
void    Si7051_start(void);
void    Si7051_reset(void);
void    Si7051_Sleep(void);
uint8_t Si7051_read_user_register(void);
void    Si7051_write_user_register(uint8_t data);
uint8_t Si7051_read_firmware_version(void);
void    Si7051_set_resolution(uint8_t resolution);
float   Si7051_readT(void);
float   Si7051_read_temperature(void);
float   Si7051_read_temperature_master(void);
#endif
