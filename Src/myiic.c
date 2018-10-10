#include "myiic.h"
#include "delay.h"

/**
* @file   IIC_Init
* @brief  ��ʼ��IIC
* @param  ��
* @retval ��
*/
void IIC_Init(void)
{					     
//	MX_GPIO_Init();
 
	IIC_SCL = 1;
	IIC_SDA = 1;
}

/**
* @file   IIC_Start
* @brief  ����IIC��ʼ�ź�
* @param  ��
* @retval ��
*/
void IIC_Start(void)
{
	SDA_OUT();  //sda�����
	IIC_SDA = 1;	  	  
	IIC_SCL = 1;
	delay_us(5);
 	IIC_SDA = 0;  //START:when CLK is high,DATA change form high to low 
	delay_us(5);
	IIC_SCL = 0;  //ǯסI2C���ߣ�׼�����ͻ��������
	delay_us(50);
}	  

/**
* @file   IIC_Stop
* @brief  ����IICֹͣ�ź�
* @param  ��
* @retval ��
*/
void IIC_Stop(void)
{
	SDA_OUT();  //sda�����
	IIC_SCL = 0;
	IIC_SDA = 0;  //STOP:when CLK is high DATA change form low to high
 	delay_us(5);
	IIC_SCL = 1; 
	delay_us(3);
	IIC_SDA = 1;  //����I2C���߽����ź�
	delay_us(4);							   	
}

/**
* @file   IIC_Wait_Ack
* @brief  �ȴ�Ӧ���źŵ���
* @param  ��
* @retval 1������Ӧ��ʧ��
*         0������Ӧ��ɹ�
*/
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime = 0;
	
	SDA_IN();  //SDA����Ϊ����  
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
//	IIC_SCL = 0;  //ʱ�����0 	   
	return 0;    
} 

/**
* @file   IIC_Ack
* @brief  ����IIC��ʼ�ź�
* @param  ��
* @retval ��
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
* @brief  ������ACKӦ��
* @param  ��
* @retval ��
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
* @brief  IIC����һ���ֽ�
* @param  txd -- �����͵�����
* @retval ��
*/	  
void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t = 0;   
	
	SDA_OUT(); 	    
    IIC_SCL = 0;  //����ʱ�ӿ�ʼ���ݴ���
    for (t = 0; t < 8; t++) {  	
        IIC_SDA = (txd&0x80)>>7;
        txd <<= 1; 	  
		delay_us(2);  //��TEA5767��������ʱ���Ǳ����
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
* @brief  ��1���ֽ�����
* @param  ack=1ʱ������ACK��ack=0������nACK
* @retval ��ȡ����һ�ֽ�����
*/
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i = 0, receive=0;
	
	SDA_IN();  //SDA����Ϊ����
    for (i = 0; i < 8; i++ ){
        IIC_SCL = 0; 
        delay_us(2);
		IIC_SCL = 1;
        receive <<= 1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();  //����nACK
    else 
        IIC_Ack();   //����ACK   
    return receive;
}

uint8_t iic_read_byte(unsigned char ack)
{
    int8_t i = 0;
    uint8_t rddata = 0;

    SDA_IN();  //SDA����Ϊ����
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
        IIC_NAck();  //����nACK
    else 
        IIC_Ack();   //����ACK   
	delay_us(10);  
    return rddata;
}
