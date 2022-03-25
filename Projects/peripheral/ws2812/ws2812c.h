#ifndef __WS2812C_H__
#define __WS2812C_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
/**
 *******************************************************************************
 ** \defgroup W25QXXGroup W25Q64 SPI NOR Flash driver
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*****************************************************************************/
/* Global pre-processor symbols/macros ('define')                            */
/*****************************************************************************/
#define WS2812C_LED_NUM		(8)

#define RGB888CONVERT(red, green, blue)	(uint32_t)((green << 16) | (red << 8) | (blue))

#define LED_COLOR_BLUE		(0x0000ff)
#define LED_COLOR_RED		(0x00ff00)
#define LED_COLOR_GREEN		(0xff0000)
#define LED_COLOR_YELLOW	(0xffff00)
#define LED_COLOR_CYAN		(0xff00ff)
#define LED_COLOR_PURPLE	(0x00ffff)
#define LED_COLOR_WHITE		(0xffffff)
#define LED_COLOR_OFF		(0x000000)

void WS2812C_Init(void);
void WS2812C_SetRGB(uint32_t RGB888);		//G8R8B8
void WS2812C_Reset(void);
void Led_Cycle_Run(void);
void ws2812c_All_Ctrl(uint32_t RGB888);

#ifdef __cplusplus
}
#endif

#endif /* __WS2812C_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
