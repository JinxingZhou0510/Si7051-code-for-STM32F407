#include "myiic.h"
#include "delay.h"

/**
* @file   IIC_Init
* @brief  初始化IIC
* @param  无
* @retval 无
*/
void IIC_Init(void)
{					     
//	MX_GPIO_Init();
 
	IIC_SCL = 1;
	IIC_SDA = 1;
}

/**
* @file   IIC_Start
* @brief  产生IIC起始信号
* @param  无
* @retval 无
*/
void IIC_Start(void)
{
	SDA_OUT();  //sda线输出
	IIC_SDA = 1;	  	  
	IIC_SCL = 1;
	delay_us(5);
 	IIC_SDA = 0;  //START:when CLK is high,DATA change form high to low 
	delay_us(5);
	IIC_SCL = 0;  //钳住I2C总线，准备发送或接收数据
	delay_us(50);
}	  

/**
* @file   IIC_Stop
* @brief  产生IIC停止信号
* @param  无
* @retval 无
*/
void IIC_Stop(void)
{
	SDA_OUT();  //sda线输出
	IIC_SCL = 0;
	IIC_SDA = 0;  //STOP:when CLK is high DATA change form low to high
 	delay_us(5);
	IIC_SCL = 1; 
	delay_us(3);
	IIC_SDA = 1;  //发送I2C总线结束信号
	delay_us(4);							   	
}

/**
* @file   IIC_Wait_Ack
* @brief  等待应答信号到来
* @param  无
* @retval 1，接收应答失败
*         0，接收应答成功
*/
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime = 0;
	
	SDA_IN();  //SDA设置为输入  
	IIC_SCL = 0; 
	IIC_SDA = 1;
	delay_us(2);	   
	IIC_SCL = 1; 
	delay_us(1);
	IIC_SCL = 0; 
	delay_us(1);	
	while (READ_SDA){
		ucErrTime++;
		if(ucErrTime > 250){
//			IIC_Stop();
			return 1;
		}
	}
//	IIC_SCL = 0;  //时钟输出0 	   
	return 0;    
} 

/**
* @file   IIC_Ack
* @brief  产生IIC起始信号
* @param  无
* @retval 无
*/
void IIC_Ack(void)
{
	IIC_SCL = 0;
	SDA_OUT();
	IIC_SDA = 0;
	delay_us(2);
	IIC_SCL = 1;
	delay_us(2);
	IIC_SCL = 0;
}

/**
* @file   IIC_NAck
* @brief  不产生ACK应答
* @param  无
* @retval 无
*/    
void IIC_NAck(void)
{
	IIC_SCL = 0;
	SDA_OUT();
	IIC_SDA = 1;
	delay_us(2);
	IIC_SCL = 1;
	delay_us(2);
	IIC_SCL = 0;
}					 				     
	
/**
* @file   IIC_Send_Byte
* @brief  IIC发送一个字节
* @param  txd -- 待发送的数据
* @retval 无
*/	  
void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t = 0;   
	
	SDA_OUT(); 	    
    IIC_SCL = 0;  //拉低时钟开始数据传输
    for (t = 0; t < 8; t++) {  	
        IIC_SDA = (txd&0x80)>>7;
        txd <<= 1; 	  
		delay_us(2);  //对TEA5767这三个延时都是必须的
		IIC_SCL = 1;
		delay_us(2); 
		IIC_SCL = 0;	
		delay_us(2);	
    }	 
} 	  

void IIC_Write_Byte(uint8_t txd)
{
	int8_t i = 0;

    SDA_OUT(); 	
    for ( i = 7; i >= 0; i-- ) {
        IIC_SCL = 0;
        delay_us(1); // tf:IN
        IIC_SDA = ((txd >> i) & 0x01);
        delay_us(2); // tLOW
        IIC_SCL = 1;
        delay_us(1); // tf:IN + tHIGH
    }
	IIC_Wait_Ack();
}

/**
* @file   IIC_Read_Byte
* @brief  读1个字节数据
* @param  ack=1时，发送ACK，ack=0，发送nACK
* @retval 读取到的一字节数据
*/
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i = 0, receive=0;
	
	SDA_IN();  //SDA设置为输入
    for (i = 0; i < 8; i++ ){
        IIC_SCL = 0; 
        delay_us(2);
		IIC_SCL = 1;
        receive <<= 1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();  //发送nACK
    else 
        IIC_Ack();   //发送ACK   
    return receive;
}

uint8_t iic_read_byte(unsigned char ack)
{
    int8_t i = 0;
    uint8_t rddata = 0;

    SDA_IN();  //SDA设置为输入
    for ( i = 7; i >= 0; i-- ) {
        IIC_SCL = 0;
        IIC_SDA = 1;                        // Configure P0^7(SDIO) as a digital input
        delay_us(1);                      // tf:IN
        delay_us(2);                      // tLOW
        IIC_SCL = 1;
        delay_us(1);                      // tf:IN + tHIGH
        rddata = ((rddata << 1) | READ_SDA);
    }
	if (!ack)
        IIC_NAck();  //发送nACK
    else 
        IIC_Ack();   //发送ACK   
	delay_us(10);  
    return rddata;
}
