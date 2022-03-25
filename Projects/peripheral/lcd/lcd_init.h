#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "gd32l23x.h"

#define LCD_W 240
#define LCD_H 240

#define DRV_GPIO        0
#define DRV_SPI         1
#define DRV_MODE        DRV_SPI

#if (DRV_MODE == DRV_GPIO)
    #define SPI_SCK_RCU             (RCU_GPIOA)
    #define SPI_MOSI_RCU            (RCU_GPIOB)

    #define SPI_GPIO_PORT1          (GPIOA)
    #define SPI_GPIO_PORT2          (GPIOB)

    /* Define port and pin for SDA and SCL */
    #define SPI_SCK_PORT            (GPIOA)
    #define SPI_SCK_PIN             (GPIO_PIN_5)

    #define SPI_MOSI_PORT           (GPIOB)
    #define SPI_MOSI_PIN            (GPIO_PIN_5)

    /* RES Port/Pin definition */
    #define RES_PORT               (GPIOA)
    #define RES_PIN                (GPIO_PIN_1)

    /* DC Port/Pin definition */
    #define DC_PORT                (GPIOA)
    #define DC_PIN                 (GPIO_PIN_4)
    
    /* BLK Port/Pin definition */
    #define BLK_PORT               (GPIOA)
    #define BLK_PIN                (GPIO_PIN_6)

    //-----------------LCD端口定义---------------- 
    #define LCD_SCLK_Clr() GPIO_BC(SPI_SCK_PORT) = (uint32_t)SPI_SCK_PIN    //SCL=SCLK
    #define LCD_SCLK_Set() GPIO_BOP(SPI_SCK_PORT) = (uint32_t)SPI_SCK_PIN

    #define LCD_MOSI_Clr() GPIO_BC(SPI_MOSI_PORT) = (uint32_t)SPI_MOSI_PIN    //SDA=MOSI
    #define LCD_MOSI_Set() GPIO_BOP(SPI_MOSI_PORT) = (uint32_t)SPI_MOSI_PIN

    #define LCD_RES_Clr()  GPIO_BC(RES_PORT) = (uint32_t)RES_PIN        //RES
    #define LCD_RES_Set()  GPIO_BOP(RES_PORT) = (uint32_t)RES_PIN

    #define LCD_DC_Clr()   GPIO_BC(DC_PORT) = (uint32_t)DC_PIN         //DC
    #define LCD_DC_Set()   GPIO_BOP(DC_PORT) = (uint32_t)DC_PIN

    #define LCD_BLK_Clr()  GPIO_BC(BLK_PORT) = (uint32_t)BLK_PIN    //BLK
    #define LCD_BLK_Set()  GPIO_BOP(BLK_PORT) = (uint32_t)BLK_PIN

#elif (DRV_MODE == DRV_SPI)

    #define SPI_SCK_RCU             (RCU_GPIOA)
    #define SPI_MOSI_RCU            (RCU_GPIOB)

    #define SPI_GPIO_PORT1          (GPIOA)
    #define SPI_GPIO_PORT2          (GPIOB)

    /* RES Port/Pin definition */
    #define RES_PORT               (GPIOA)
    #define RES_PIN                (GPIO_PIN_1)

    /* DC Port/Pin definition */
    #define DC_PORT                (GPIOA)
    #define DC_PIN                 (GPIO_PIN_4)
    
    /* BLK Port/Pin definition */
    #define BLK_PORT               (GPIOA)
    #define BLK_PIN                (GPIO_PIN_6)

    #define LCD_RES_Clr()  GPIO_BC(RES_PORT) = (uint32_t)RES_PIN        //RES
    #define LCD_RES_Set()  GPIO_BOP(RES_PORT) = (uint32_t)RES_PIN

    #define LCD_DC_Clr()   GPIO_BC(DC_PORT) = (uint32_t)DC_PIN         //DC
    #define LCD_DC_Set()   GPIO_BOP(DC_PORT) = (uint32_t)DC_PIN

    #define LCD_BLK_Clr()  GPIO_BC(BLK_PORT) = (uint32_t)BLK_PIN    //BLK
    #define LCD_BLK_Set()  GPIO_BOP(BLK_PORT) = (uint32_t)BLK_PIN

    #define SPI_RCU_PERIPH          (RCU_SPI0)
    #define SPI_PERIPH              (SPI0)

    /* Define port and pin for SDA and SCL */
    #define SPI_SCK_PORT            (GPIOA)
    #define SPI_SCK_PIN             (GPIO_PIN_5)

    #define SPI_MOSI_PORT           (GPIOB)
    #define SPI_MOSI_PIN            (GPIO_PIN_5)

#endif

#if (DRV_MODE == DRV_SPI)
void LCD_SPI_Init(void); //初始化SPI
#endif
void LCD_GPIO_Init(void);//初始化GPIO
void LCD_Writ_Bus(uint8_t dat);//模拟SPI时序
void LCD_WR_DATA8(uint8_t dat);//写入一个字节
void LCD_WR_DATA(uint16_t dat);//写入两个字节
void LCD_WR_REG(uint8_t dat);//写入一个指令
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);//设置坐标函数
void LCD_Init(void);//LCD初始化

#endif
