#include "tea5767.h"
#include "log_cfg.h"

#define MAX_FREQ 108000
#define MIN_FREQ 87500
//static uint8_t radio_write_data[5]={0x29,0xc2,0x20,0x11,0x00};        //要写入TEA5767的数据
static uint8_t radio_write_data[5]={0x2a,0xb6,0x40,0x17,0x40};        //要写入TEA5767的数据
static uint8_t radio_read_data[5];        //TEA5767读出的状态
//static uint32_t default_pll=0x29c2;//0x29f9;        //默认存台的pll,87.8MHz
//uint32_t max_pll=0x339b;            //108MHz时的pll,
//uint32_t min_pll=9000;            //70MHz时的pll
static uint32_t pll = 0;
static uint32_t frequency = 0;

void tea5767_init(void)
{
	IIC_Init();
}

void tea5767_write(void)
{
    uint8_t i;
    IIC_Start();
    IIC_Send_Byte(0xc0);        //TEA5767写地址
    if(!IIC_Wait_Ack())
    {
        for(i=0;i<5;i++)
        {
            IIC_Send_Byte(radio_write_data[i]);
            if(IIC_Wait_Ack())
			{
				PRO_LOG(LOG_DEBUG, "write buffer failed. \r\n");
				break;
			}
        }
    }
    IIC_Stop(); 
}

void tea5767_read(void)
{
    uint8_t i;
    uint8_t temp_l,temp_h;
    pll=0;
    IIC_Start();
    IIC_Send_Byte(0xc1);        //TEA5767读地址
    if(!IIC_Wait_Ack())
    {
        for(i=0;i<5;i++)
        {
            radio_read_data[i]=(i==4)?IIC_Read_Byte(0):IIC_Read_Byte(1);
        }
		IIC_Stop();
    }
	else
    	IIC_Stop();
    temp_l=radio_read_data[1];
    temp_h=radio_read_data[0];
    temp_h&=0x3f;
    pll=temp_h*256+temp_l+1;
    get_tea5767_frequency();
}

void get_tea5767_pll(void)
{
    uint8_t hlsi;
    hlsi=radio_write_data[2]&0x10;
    if (hlsi)
        pll=(unsigned int)((float)((frequency+225)*4)/(float)32.768);    //频率单位:k
    else
        pll=(unsigned int)((float)((frequency-225)*4)/(float)32.768);    //频率单位:k	
}

void get_tea5767_frequency(void)
{
    uint8_t hlsi;
    uint32_t npll=0;
    npll=pll;
    hlsi=radio_write_data[2]&0x10;
    if (hlsi)
        frequency=(unsigned long)((float)(npll)*(float)8.192-225);    //频率单位:KHz
    else
        frequency=(unsigned long)((float)(npll)*(float)8.192+225);    //频率单位:KHz	
}

void tea5767_search(int mode)
{
    tea5767_read();        
    if(mode)
    {
        frequency+=100;
        if(frequency>MAX_FREQ)
            frequency=MIN_FREQ;
    }
    else
    {
        frequency-=100;
        if(frequency<MIN_FREQ)
            frequency=MAX_FREQ;
    }
    get_tea5767_pll();
    radio_write_data[0]=pll/256;
    radio_write_data[1]=pll%256;
    radio_write_data[2]=0x20;
    radio_write_data[3]=0x11;
    radio_write_data[4]=0x00;
    tea5767_write();	
}

//自动搜台,mode=1,频率增加搜台; mode=0:频率减小搜台,不过这个好像不能循环搜台
void tea5767_auto_search(int mode)
{
    tea5767_read();
    get_tea5767_pll();
    if(mode)
        radio_write_data[2]=0xa0;
    else
        radio_write_data[2]=0x20;    
    radio_write_data[0]=pll/256+0x40;
    radio_write_data[1]=pll%256;    
    radio_write_data[3]=0x11;
    radio_write_data[4]=0x00;
    tea5767_write();
    tea5767_read();
    while(!(radio_read_data[0]&0x80))     //搜台成功标志
    {
        tea5767_read();
    }    
}

void tea5767_set_frequency(uint32_t freq)
{
    if(freq>MAX_FREQ)
        frequency=MIN_FREQ;
    if(freq<MIN_FREQ)
    frequency=MAX_FREQ;

    get_tea5767_pll();
    radio_write_data[0]=pll/256;
    radio_write_data[1]=pll%256;
    radio_write_data[2]=0x20;
    radio_write_data[3]=0x11;
    radio_write_data[4]=0x00;
    tea5767_write();	
}


