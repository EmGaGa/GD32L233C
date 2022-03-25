#include "USART1.h"
typedef  unsigned long	 uint32; 


///////////////////////////////////////////////////////////////////////////////////////////////////////	 
//���������ڼ���֤/�������ӽ�JCϵ�м��ɴ���ģ��
//STM32F030������	   
//���ӽ�Ƽ�@���̲�
//��˾��վ:www.sun-lcm.com
//�Ա��꣺https://shop151604432.taobao.com
//������ҵ�꣺https://sundisplay2008.1688.com
//ҵ��֧�֣����� mobile:13509671256 QQ:3167153224
//����֧�֣����� QQ:3085638545
//�޸�����:2015/09/18 
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ���ӽ�Ƽ����޹�˾ 2015-2021
//All rights reserved
//******************************************************************************************************
//V1.0�޸�˵��
//����
///////////////////////////////////////////////////////////////////////////////////////////////////////// 	  
	


//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{  
	while(!((USART1->ISR)&(1<<7))){}
  USART1->TDR=ch;
  return (ch);
}



	
#endif 

#define u8 unsigned char
	
/////////////////////////////////////////////////////////////////////////////////////
//STM32���ڳ�ʼ������
//1.���߷�ʽ TX-->TX RX->RX 
//2.���ڳ�ʼ�� ģ���Ĭ������Ϊ 115200 8λ����λ  1��ʼλ 1ֹͣλ ����żУ��
//3.baud��ʹ�õ��Ĳ�����
/////////////////////////////////////////////////////////////////////////////////////

/* USART��ʼ�� */
void USART1_Init(uint32_t baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  //ʹ��GPIOA��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//ʹ��USART��ʱ��
	/* USART1�Ķ˿����� */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);//����PA9�ɵڶ���������	TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);//����PA10�ɵڶ���������  RX	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	

	/* USART1�Ļ������� */
	USART_InitStructure.USART_BaudRate = baud;              //������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);		
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);           //ʹ�ܽ����ж�
	USART_Cmd(USART1, ENABLE);                             //ʹ��USART1
	
	/* USART1��NVIC�ж����� */
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
//��ȡ�����ַ�����
//1.���߷�ʽ TX-->TX RX->RX 
//2.���ڳ�ʼ�� ģ���Ĭ������Ϊ 115200 8λ����λ  1��ʼλ 1ֹͣλ ����żУ��
//3.���յ����ַ����ظ���һ�㺯��
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
//��ȡ����״̬����
//1.���߷�ʽ TX-->TX RX->RX 
//2.���ڳ�ʼ�� ģ���Ĭ������Ϊ 115200 8λ����λ  1��ʼλ 1ֹͣλ ����żУ��
//3.�����жϷ�ʽ����"OK\r\n"
//4.��λokȫ�ֱ�����ֱ��ok��������Ϊ0x0f��������ȡ"OK\r\n"�����������˳������������ѭ��
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
//����ģ�鷢�ͻ������ַ�����
//1.���߷�ʽ TX-->TX RX->RX 
//2.���ڳ�ʼ�� ģ���Ĭ������Ϊ 115200 8λ����λ  1��ʼλ 1ֹͣλ ����żУ��
//3.��λokȫ�ֱ����ͳ�ȡVAR������ASCII �ַ�
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
//VAR�������ν�������
//1.���߷�ʽ TX-->TX RX->RX 
//2.���ڳ�ʼ�� ģ���Ĭ������Ϊ 115200 8λ����λ  1��ʼλ 1ֹͣλ ����żУ��
//3.VAR������ASCII �ַ�ת��Ϊvalֵ�������ظ���һ�㺯��
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
				/*��һ���ֽ�*/
				if ((rch[1][2]>=0x30) && (rch[1][2]<=0x39))//100���ϵ���  
				{		
					val[0]=(rch[1][0]-0x30)*100+(rch[1][1]-0x30)*10+(rch[1][2]-0x30);
					
				}
				else if((rch[1][1]>=0x30) && (rch[1][1]<=0x39)) //10���ϵ���
				{
					val[0]=(rch[1][0]-0x30)*10+(rch[1][1]-0x30);
				}		
				else if((rch[1][0]>=0x30) && (rch[1][0]<=0x39))
				{
					val[0]=(rch[1][0]-0x30);						
				}
				/*�ڶ����ֽ�*/
					if ((rch[2][2]>=0x30) && (rch[2][2]<=0x39))//100���ϵ���
				{		
					val[1]=(rch[2][0]-0x30)*100+(rch[2][1]-0x30)*10+(rch[2][2]-0x30);				
				}
				else if((rch[2][1]>=0x30) && (rch[2][1]<=0x39)) //10���ϵ���
				{
					val[1]=(rch[2][0]-0x30)*10+(rch[2][1]-0x30);
				}		
				else if((rch[2][0]>=0x30) && (rch[2][0]<=0x39))
				{
					val[1]=(rch[2][0]-0x30);						
				}
				
			/*�������ֽ�*/
					if ((rch[3][2]>=0x30) && (rch[3][2]<=0x39))//100���ϵ���
				{		
					val[2]=(rch[3][0]-0x30)*100+(rch[3][1]-0x30)*10+(rch[3][2]-0x30);
					
				}
			
				else if((rch[3][1]>=0x30) && (rch[3][1]<=0x39)) //10���ϵ���
				{
					val[2]=(rch[3][0]-0x30)*10+(rch[3][1]-0x30);
				}		
				else if((rch[3][0]>=0x30) && (rch[3][0]<=0x39))
				{
					val[2]=(rch[3][0]-0x30);						
				}
				/*���ĸ��ֽ�*/
					if ((rch[4][2]>=0x30) && (rch[4][2]<=0x39))//100���ϵ���
				{		
					val[3]=(rch[4][0]-0x30)*100+(rch[4][1]-0x30)*10+(rch[4][2]-0x30);
					
				}
			
				else if((rch[4][1]>=0x30) && (rch[4][1]<=0x39)) //10���ϵ���
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
//���ڷ������ݺ���
//1.���߷�ʽ TX-->TX RX->RX 
//2.���ڳ�ʼ�� ģ���Ĭ������Ϊ 115200 8λ����λ  1��ʼλ 1ֹͣλ ����żУ��
//3.���������ַ�����ֱ���ַ�������
//4. databuf ΪҪ�����ַ����ĵ�ַ
//////////////////////////////////////////////////////////////////////////////////////  

void UartSend(char * databuf) 
{ 
	u8 i=0;
	while (1)
	{ 
		if ((*databuf)!=0)//ֱ�����ݶ�������� 
	{ 
		USART_SendData(USART1, *databuf); //����һ���ֽ�����
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}; //
		databuf++;//i++;
	}
	else return;
	}
}		
	
/////////////////////////////////////////////////////////////////////////////////////
//���ڽ����жϺ���
//1.���߷�ʽ TX-->TX RX->RX 
//2.���ڳ�ʼ�� ģ���Ĭ������Ϊ 115200 8λ����λ  1��ʼλ 1ֹͣλ ����żУ��
//3.��ȡÿһ���ַ������н���
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
//�ļ����ƣ�
//���ܸ�Ҫ��USART1�жϺ���
//����˵������
//�������أ���
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
