#include "USART1.h"
typedef  unsigned long	 uint32; 


///////////////////////////////////////////////////////////////////////////////////////////////////////	 
//本程序用于简单验证/测试尚视界JC系列集成串口模块
//STM32F030开发板	   
//尚视界科技@工程部
//公司网站:www.sun-lcm.com
//淘宝店：https://shop151604432.taobao.com
//阿里企业店：https://sundisplay2008.1688.com
//业务支持：潘生 mobile:13509671256 QQ:3167153224
//技术支持：梁生 QQ:3085638545
//修改日期:2015/09/18 
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 尚视界科技有限公司 2015-2021
//All rights reserved
//******************************************************************************************************
//V1.0修改说明
//初版
///////////////////////////////////////////////////////////////////////////////////////////////////////// 	  
	


//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{  
	while(!((USART1->ISR)&(1<<7))){}
  USART1->TDR=ch;
  return (ch);
}



	
#endif 

#define u8 unsigned char
	
/////////////////////////////////////////////////////////////////////////////////////
//STM32串口初始化函数
//1.接线方式 TX-->TX RX->RX 
//2.串口初始化 模块的默认设置为 115200 8位数据位  1开始位 1停止位 无奇偶校验
//3.baud是使用到的波特率
/////////////////////////////////////////////////////////////////////////////////////

/* USART初始化 */
void USART1_Init(uint32_t baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  //使能GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//使能USART的时钟
	/* USART1的端口配置 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);//配置PA9成第二功能引脚	TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);//配置PA10成第二功能引脚  RX	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	

	/* USART1的基本配置 */
	USART_InitStructure.USART_BaudRate = baud;              //波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);		
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);           //使能接收中断
	USART_Cmd(USART1, ENABLE);                             //使能USART1
	
	/* USART1的NVIC中断配置 */
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x02;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
			     //_KEY_STEP   PA2  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
		

				
}

/////////////////////////////////////////////////////////////////////////////////////
//获取串口字符函数
//1.接线方式 TX-->TX RX->RX 
//2.串口初始化 模块的默认设置为 115200 8位数据位  1开始位 1停止位 无奇偶校验
//3.接收到的字符返回给上一层函数
//////////////////////////////////////////////////////////////////////////////////////

int UartGet (void)  { 

    while (!(USART1->ISR & USART_FLAG_RXNE));

    return ((int)(USART1->TDR & 0x1FF));
} 

extern u8 rx_flag_finished;
extern u8 RX_BUF[];
extern u8 rx_count;
extern u8 ok;

/////////////////////////////////////////////////////////////////////////////////////
//获取就绪状态函数
//1.接线方式 TX-->TX RX->RX 
//2.串口初始化 模块的默认设置为 115200 8位数据位  1开始位 1停止位 无奇偶校验
//3.串口中断方式接收"OK\r\n"
//4.置位ok全局变量，直到ok变量设置为0x0f，正常获取"OK\r\n"，函数正常退出，否则进入死循环
//////////////////////////////////////////////////////////////////////////////////////
void CheckBusy(void)  
{
	while(1)
	{
   if(ok==0x0f)
		 break;
	}		
	
	ok=0;
}

 void get_var(unsigned char *val)
		{	 
			for(;;)
			{ 
				if((RX_BUF[0]=='V')&&(RX_BUF[1]=='A')&&(RX_BUF[2]=='R'))
				{
					RX_BUF[0]=0;
					RX_BUF[1]=0;
					RX_BUF[2]=0;
					val[0]=RX_BUF[3];
					val[1]=RX_BUF[4];
					val[2]=RX_BUF[5];
					val[3]=RX_BUF[6];
					rx_flag_finished=0;	
					rx_count=0;
					break;				
				}	
			}

		}


		
char rch[5][3];     //xxx:xxx,xxx,xxx,xxx
u8 i=0;	
u8 p=0;//
u8 cmd=0;		//==1
u8 cmdok=0;	//==1 
u8 ok=0x00;
		
/////////////////////////////////////////////////////////////////////////////////////
//解析模块发送回来的字符函数
//1.接线方式 TX-->TX RX->RX 
//2.串口初始化 模块的默认设置为 115200 8位数据位  1开始位 1停止位 无奇偶校验
//3.置位ok全局变量和抽取VAR变量的ASCII 字符
//////////////////////////////////////////////////////////////////////////////////////
		
void getch(u8 c)
{	
	 if (c=='{')
	{	i=0;
		cmd=1;
		p=0;
	}
	else if(c=='}')
	{	cmd=0;
		cmdok=1;
	}
	else if((c==':')||(c==','))
	{
			p++;
			i=0;
	}		
	else if (cmd==1)
	{	
		if (i<3) rch[p][i]=c;
		i++;
	}
	else	if(c=='O')
	{
		ok=(ok&0x00)|(0x01);
	}		
	else if(c=='K')
	{
		ok=(ok&0x0d)|(0x02);
	}	
  else if(c=='\r')
	{
		ok=(ok&0x0b)|(0x04);
	}
  else if(c=='\n')
	{
	 ok=(ok&0x07)|(0x08);
	}
	
}		

unsigned char val[4];

/////////////////////////////////////////////////////////////////////////////////////
//VAR变量二次解析函数
//1.接线方式 TX-->TX RX->RX 
//2.串口初始化 模块的默认设置为 115200 8位数据位  1开始位 1停止位 无奇偶校验
//3.VAR变量的ASCII 字符转换为val值，并返回给上一层函数
//////////////////////////////////////////////////////////////////////////////////////
unsigned char GetValue(void)  
{	
	unsigned char m,n;
	while(1)
	{			
		if(cmdok==1)
		{			
			if ((rch[0][0]=='V')&&(rch[0][1]=='A')&&(rch[0][2]=='R'))
			{	
				/*第一个字节*/
				if ((rch[1][2]>=0x30) && (rch[1][2]<=0x39))//100以上的数  
				{		
					val[0]=(rch[1][0]-0x30)*100+(rch[1][1]-0x30)*10+(rch[1][2]-0x30);
					
				}
				else if((rch[1][1]>=0x30) && (rch[1][1]<=0x39)) //10以上的数
				{
					val[0]=(rch[1][0]-0x30)*10+(rch[1][1]-0x30);
				}		
				else if((rch[1][0]>=0x30) && (rch[1][0]<=0x39))
				{
					val[0]=(rch[1][0]-0x30);						
				}
				/*第二个字节*/
					if ((rch[2][2]>=0x30) && (rch[2][2]<=0x39))//100以上的数
				{		
					val[1]=(rch[2][0]-0x30)*100+(rch[2][1]-0x30)*10+(rch[2][2]-0x30);				
				}
				else if((rch[2][1]>=0x30) && (rch[2][1]<=0x39)) //10以上的数
				{
					val[1]=(rch[2][0]-0x30)*10+(rch[2][1]-0x30);
				}		
				else if((rch[2][0]>=0x30) && (rch[2][0]<=0x39))
				{
					val[1]=(rch[2][0]-0x30);						
				}
				
			/*第三个字节*/
					if ((rch[3][2]>=0x30) && (rch[3][2]<=0x39))//100以上的数
				{		
					val[2]=(rch[3][0]-0x30)*100+(rch[3][1]-0x30)*10+(rch[3][2]-0x30);
					
				}
			
				else if((rch[3][1]>=0x30) && (rch[3][1]<=0x39)) //10以上的数
				{
					val[2]=(rch[3][0]-0x30)*10+(rch[3][1]-0x30);
				}		
				else if((rch[3][0]>=0x30) && (rch[3][0]<=0x39))
				{
					val[2]=(rch[3][0]-0x30);						
				}
				/*第四个字节*/
					if ((rch[4][2]>=0x30) && (rch[4][2]<=0x39))//100以上的数
				{		
					val[3]=(rch[4][0]-0x30)*100+(rch[4][1]-0x30)*10+(rch[4][2]-0x30);
					
				}
			
				else if((rch[4][1]>=0x30) && (rch[4][1]<=0x39)) //10以上的数
				{
					val[3]=(rch[4][0]-0x30)*10+(rch[4][1]-0x30);
				}		
				else if((rch[4][0]>=0x30) && (rch[4][0]<=0x39))
				{
					val[3]=(rch[4][0]-0x30);						
				}		
				
			for(n=0;n<5;n++)
					for(m=0;m<3;m++)
				rch[n][m]=0;
				 cmdok=0;
					break;
				
				
		}
			//	cmdok=0;
			
		
		
	}	
		
}
}

#if 0
unsigned char GetKey(unsigned char *rval)  
{	
	unsigned char m,n;
	while(1)
	{			
		if(cmdok==1)
		{			
			if ((rch[0][0]=='U')&&(rch[0][1]=='P'))
			{		
				*rval=1;
						cmdok=0;
				for(n=0;n<5;n++)
					for(m=0;m<3;m++)
				rch[n][m]=0;
	
					break;	
				
			}
			else if ((rch[0][0]=='D')&&(rch[0][1]=='N'))
			{
						*rval=0;
				   cmdok=0;
				for(n=0;n<5;n++)
					for(m=0;m<3;m++)
				rch[n][m]=0;
					break;	
			}

		
		}
		
	}	
		
}
#endif	

/////////////////////////////////////////////////////////////////////////////////////
//串口发送数据函数
//1.接线方式 TX-->TX RX->RX 
//2.串口初始化 模块的默认设置为 115200 8位数据位  1开始位 1停止位 无奇偶校验
//3.连续发送字符串，直到字符串结束
//4. databuf 为要发送字符串的地址
//////////////////////////////////////////////////////////////////////////////////////  

void UartSend(char * databuf) 
{ 
	u8 i=0;
	while (1)
	{ 
		if ((*databuf)!=0)//直到数据都发送完成 
	{ 
		USART_SendData(USART1, *databuf); //发送一个字节数据
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}; //
		databuf++;//i++;
	}
	else return;
	}
}		
	
/////////////////////////////////////////////////////////////////////////////////////
//串口接收中断函数
//1.接线方式 TX-->TX RX->RX 
//2.串口初始化 模块的默认设置为 115200 8位数据位  1开始位 1停止位 无奇偶校验
//3.获取每一个字符并进行解析
//////////////////////////////////////////////////////////////////////////////////////  

u8 RX_BUF[4];
u8 rx_flag_finished=0;
u8 rx_count=0;
void USART1_IRQHandler(void)                	//??1??????
{
	u8 Res=1;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //????(?????????0x0d 0x0a??)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//????????
		getch(Res);
//		if(rx_flag_finished!=0xc0)//?????
//		{
//			if(rx_flag_finished&0x40)//????0x0d
//			{
//				if(Res==0x0a)
//				{
//					rx_flag_finished|=0x80;	//????? rx_flag_finished=0xc0
//					RX_BUF[rx_count]='\0'; //??????????????
//				}
//				else 
//				{
//					rx_flag_finished=0;//????,????					
//					rx_count=0;
//				}
//			}
//			else //????0X0D
//			{	
//				if(Res!=0x0d)
//				{
//					RX_BUF[rx_count++]=Res ;	
//				}
//				else
//				{
//					rx_flag_finished|=0x40;
//				}
//			}
//		}  
		
		

//		if(Res=='O')
//		busy_flag=0;
//		else
//		busy_flag=1;
  }
} 








#if 0




//=============================================================================
//文件名称：
//功能概要：USART1中断函数
//参数说明：无
//函数返回：无
//=============================================================================
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
   USART_SendData(USART1,USART_ReceiveData(USART1));
	 while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
	}
			
}

#endif
