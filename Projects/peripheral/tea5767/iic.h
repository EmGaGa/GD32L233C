#ifndef __IIC_H
#define __IIC_H 	
#include "gd32l23x.h"

#define IIC_RCU             (RCU_GPIOB)
/* Define port and pin for SDA and SDL */
#define IIC_PORT            (GPIOB)

#define IIC_SDA_PIN         (GPIO_PIN_7)
#define IIC_SDL_PIN         (GPIO_PIN_6)

//IO方向设置
#define SDA_IN()  GPIO_CTL(IIC_PORT) &= ~CTL_CLTR(1)  //{GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define SDA_OUT() GPIO_CTL(IIC_PORT) |= CTL_CLTR(1) //{GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}


//IO操作函数	 
#define IIC_SCL(sta)    gpio_bit_write(IIC_PORT, IIC_SDL_PIN, sta)//GPIO_BOP(IIC_PORT) = (uint32_t)IIC_SDL_PIN //SCL
#define IIC_SDA(sta)    gpio_bit_write(IIC_PORT, IIC_SDA_PIN, sta)//GPIO_BOP(IIC_PORT) = (uint32_t)IIC_SDA_PIN //SDA	 
#define READ_SDA   (GPIO_ISTAT(IIC_PORT) & (IIC_SDA_PIN)) //输入SDA 

//IIC所有操作函数
void IIC_Init(void);                			//初始化IIC的IO口				 
void IIC_Start(void);						//发送IIC开始信号
void IIC_Stop(void);	  						//发送IIC停止信号
void IIC_Send_Byte(uint8_t txd);					//IIC发送一个字节
uint8_t IIC_Read_Byte(unsigned char ack);	 //IIC读取一个字节
uint8_t IIC_Wait_Ack(void); 					//IIC等待ACK信号
void IIC_Ack(void);							//IIC发送ACK信号
void IIC_NAck(void);							//IIC不发送ACK信号

#endif

