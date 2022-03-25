#ifndef __DEV_STR_H__
#define __DEV_STR_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
typedef struct 
{
    uint8_t page;       //
    uint8_t ledCtrlColor;   //当前所调灯色
    uint8_t rgbColor[3];    //rgb
    uint32_t radio_freq;    //收音机当前设置频段
}dev_handler_str;


/*****************************************************************************/
/* Global pre-processor symbols/macros ('define')                            */
/*****************************************************************************/
/* Port/Pin definition */


#ifdef __cplusplus
}
#endif

#endif /* __DEV_STR_H__ */