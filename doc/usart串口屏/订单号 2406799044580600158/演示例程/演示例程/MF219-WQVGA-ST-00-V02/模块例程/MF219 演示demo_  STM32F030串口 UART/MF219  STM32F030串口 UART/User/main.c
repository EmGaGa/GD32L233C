#include "stm32f0xx.h"
#include "delay.h"
#include "led.h"
#include "USART1.h"
#include "Key.h"

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

extern unsigned char val[];

/////////////////////////////////////////////////////////////////////////////////////
//主函数
//1.接线方式 TX-->TX RX->RX 
//2.上电后，为了使模块正常工作需要等待1秒钟，保证模块能够正常的初始化
//3.串口初始化 模块的默认设置为 115200 8位数据位  1开始位 1停止位 无奇偶校验
//4.发送指令或者指令串，发送流程为："指令;\r\n"	或者"指令1;指令2;...指令n;\r\n"
//5.指令发送完必须等待模块回传的“OK\r\n” 或者通过延时的方式确保指令或者指令串是执行完
//////////////////////////////////////////////////////////////////////////////////////


int main(void)
{
  unsigned char i,k;	
	unsigned char ret;
	char buf[128];
	
  delay_init();//延时初始化
  delay_ms(1000);//等待1S
  USART1_Init(115200);	//串口初始化
	//printf("STM32F030F4P6 USART TEST\r\n");

		for(;;)//循环执行
  {		
		delay_ms(500);
		
		
		for(i=0;i<7;i++)
		{
			sprintf(buf,"DIR(1);FSIMG(%2d,0,0,376,240,0);\r\n",2097152+240*376*2*i);//在（0,0）位置连续调取8张整屏图片，第一张从地址2097152开始，图片大小376x240，模式0表示正常显示
			UartSend(buf);//发送字符指令串
			CheckBusy();	//等待模块执行完，也就是检测模块发送回来的"OK\r\n"
	  }
		delay_ms(1000);
			 
		UartSend("DIR(1);CLR(0);DC32(50,10,'深圳尚视界科技',4);\r\n");// 横屏显示，背景颜色为黑色，用黄色以(50,10)为起点,在屏幕上显示透明的'深圳尚视界科技'这四个字,字体大小为32,4为黄色的色表编号
		CheckBusy();

		UartSend("PL(0,60,375,60,15);\r\n");//画直线命令将屏幕上的(0,60)位置的点和（375,60）位置的点用白色(编号15)的直线连起来
		CheckBusy();
		//delay_ms(800);
		
		UartSend("PL(0,60,375,60,15);\r\n");//画直线命令同上
		CheckBusy();
		//delay_ms(800);
	
		UartSend("PL(0,120,375,120,15);\r\n");//画直线命令同上
		CheckBusy();
		//delay_ms(800);
		
		UartSend("PL(0,180,375,180,15);\r\n");//画直线命令同上
		CheckBusy();
		//delay_ms(800);
		
		UartSend("PL(0,239,375,240,15);\r\n");//画直线命令同上
		CheckBusy();
		//delay_ms(800);
		
		UartSend("PL(0,60,0,239,15);\r\n");//画直线命令同上
		CheckBusy();
		//delay_ms(800);
		
		UartSend("PL(105,60,105,239,15);\r\n");//画直线命令同上
		CheckBusy();
		//delay_ms(800);
		
		UartSend("PL(210,60,210,239,15);\r\n");//画直线命令同上
		CheckBusy();
		//delay_ms(800);
		
		UartSend("PL(375,60,375,239,15);\r\n");//画直线命令同上
		CheckBusy();
		//delay_ms(800);
		
		UartSend("CIR(50,90,20,1);\r\n"); //(50,90)为圆心 20为半径用红色画一个线圆 ,1为红色的色表编号
		CheckBusy();
		
		UartSend("CIRF(60,90,20,2);\r\n"); //(60,90)为圆心 20为半径用绿色画一个填充圆, ,2为绿色的色表编号
		CheckBusy();
		//delay_ms(800);
		
		UartSend("BOXF(140,75,180,100,3);\r\n");//对角线(140,75)和(180,100)画填充框 ,3为蓝色的色表编号 
		CheckBusy();
		
		UartSend("BOX(150,75,190,100,1);\r\n");//对角线(150,75)和(190,100)画线框 ,1为红色的色表编号
		CheckBusy();
		
		UartSend("PS(270,85,2);\r\n");//在屏幕的(270,85) 位置用绿色描绘一个点,2为绿色的色表编号
		CheckBusy();	
		
		UartSend("PL(250,95,300,95,1);\r\n");//画直线命令同上
		CheckBusy();
			
		UartSend("DC16(20,140,'ABC',3);\r\n");//刷字体命令同上
		CheckBusy();
		
		UartSend("SBC(15);DCV16(60,140,'ABC',0);\r\n"); //用黑色以(60,140)为起点,在屏幕显示带白色背景色的'ABC'这四个字母,字体大小为16,0为黑色的色表编号
		CheckBusy();
		
		UartSend("DC24(120,140,'123',2);\r\n");//刷字体命令同上
		CheckBusy();
		
		UartSend("SBC(4);DCV24(160,140,'123',0);\r\n");//刷字体命令同上
		CheckBusy();
		
		UartSend("DC32(230,140,'深圳',2);\r\n");//刷字体命令同上
		CheckBusy();
		
		UartSend("SBC(1);DCV32(20,190,'深圳',15);\r\n"); //刷字体命令同上
		CheckBusy();
		
		UartSend("DC48(110,190,'中国',6);\r\n");//刷字体命令同上
		CheckBusy();
		
		UartSend("DC48(220,190,'中国',1);\r\n"); //刷字体命令同上	
		CheckBusy();
    delay_ms(1200);
	//CheckBusy();
		UartSend("CLR(0);DIR(1);FSIMG(3392096,0,0,376,240,0);\r\n");//横屏显示，在(0,0)的位置显示地址为3392096图片,图片的宽度是376 高度是240，模式0表示正常显示
		CheckBusy();	
		UartSend("DC48(45,10,'2017-06-25 ',15);\r\n");//刷字体命令同上	
		CheckBusy();
		UartSend("DC32(20,65,' 星期二',15);\r\n");//刷字体命令同上	
		CheckBusy();
		
		UartSend("DC32(150,65,' 20 ：',15);\r\n");//刷字体命令同上	
		CheckBusy();
		
		UartSend("PL(0,60,376,60,15);\r\n");//画直线命令同上
		CheckBusy();
		
	{	 
		k=0;
		for(k=0;k<=60;k++)

	 {
			sprintf(buf,"SBC(15);DCV32(250,65,'%d',1);\r\n",k);//刷32号字体变化数字0-50
			k=k+1;
			delay_ms(100);
			UartSend(buf);
			CheckBusy();							 
		}
	 
		 k=0;
		 for(k=0;k<=50;k++)
	 {
			sprintf(buf,"SBC(15);DCV32(50,130,'%d',1);\r\n",k);//刷32号字体变化数字0-50
			k=k+1;
			delay_ms(100);
			UartSend(buf);
			CheckBusy();	
			 
			sprintf(buf,"SBC(15);DCV32(163,130,'%d',1);\r\n",k);//刷32号字体变化数字0-50
			k=k+1;
			delay_ms(100);
			UartSend(buf);
			CheckBusy();
			 
			 
			sprintf(buf,"SBC(15);DCV32(273,130,'%d',1);\r\n",k);//刷32号字体变化数字0-50
			k=k+1;
			delay_ms(100);
			UartSend(buf);
			CheckBusy();	
						 
		}	
	} 
		 delay_ms(1200);
	
		UartSend("DIR(1);CLR(15);QRCODE(80,50,Hi! 你好,200,1);\r\n");//横屏显示 底色为白色 二维码显示开始地址为x,y为80,50，显示内容为Hi! 你好，大小为200，颜色为1红色
		CheckBusy();
	  delay_ms(1200);
	
		UartSend("CLR(0);DIR(1);FSIMG(3572576,0,0,376,240,0);\r\n");//刷图片指令同上
		CheckBusy();	
	
	{
				k=40;
		 {
			sprintf(buf,"DCV32(120,110,'亮度：80 ',1);BL(%2d);\r\n",k);//刷32号字体，将亮度变量k转换成"BL(k);\r\n"的字符指令，范围：0为最亮 255为最暗 
			UartSend(buf);
			CheckBusy();	
			delay_ms(1000);
		 }	
	 }	
	
	
	{
				k=127;
		{
			sprintf(buf,"DCV32(120,110,'亮度：50 ',1);BL(%2d);\r\n",k);//刷32号字体，将亮度变量k转换成"BL(k);\r\n"的字符指令，范围：0为最亮 255为最暗 
			UartSend(buf);
			CheckBusy();	
			delay_ms(1000);
		}	
	}	
		
		{
				k=200;
	{
			sprintf(buf,"DCV32(120,110,'亮度：20 ',1);BL(%2d);\r\n",k);//刷32号字体，将亮度变量k转换成"BL(k);\r\n"的字符指令，范围：0为最亮 255为最暗 
			UartSend(buf);
			CheckBusy();	
			delay_ms(1000);
		}	
	}	
			
	{
				k=0;
		{
			sprintf(buf,"DCV32(120,110,'亮度：100',1);BL(%2d);\r\n",k);//刷32号字体，将亮度变量k转换成"BL(k);\r\n"的字符指令，范围：0为最亮 255为最暗 
			UartSend(buf);
			CheckBusy();	
			delay_ms(1000);
		}	
	}		
	  delay_ms(1000);
	
		UartSend("DIR(1);CLR(61);DC48(70,90,'演示结束！',15);\r\n");//刷字体命令同上 	
		CheckBusy();
	  delay_ms(1500);


  } 
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
