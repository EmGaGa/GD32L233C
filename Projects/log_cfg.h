#ifndef __LOG_CFG_H__
#define __LOG_CFG_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

#define _LOG_DEBUG

#define LOG_ALL		(0x00)
#define LOG_DEBUG	(0x01)
#define LOG_INFO	(0x02)
#define LOG_WARN	(0x03)
#define LOG_ERROR	(0x04)
#define LOG_FATAL	(0x05)
#define LOG_OFF		(0xff)

#define LOG_LEVEL	LOG_DEBUG

#ifdef _LOG_DEBUG
#define PRO_LOG(log_level, fmt, arg...)	do{	\
									if(log_level >= LOG_LEVEL)	\
									{	\
										rt_kprintf(fmt, ##arg);\
									}\
								}while(0)
#else
#define PRO_LOG(log_level, fmt, arg...)	
#endif 

/*使用断言测试*/
#define _ASSERT

#ifdef _ASSERT
#define ASSERT(condition)\
		if(condition)\
			; \
		else\
		{\
			fflush(stdout);\
			fprintf(stderr,"\n Assert failed, file %s, line %u \n",__FILE__, __LINE__);\
			fflush(stderr);\
			abort();\
		}
#else
#define ASSERT(condition)	;
#endif

//#include "rtthread.h"
//#ifndef __aeabi_assert(EX)
//#define __aeabi_assert ASSERT
//#endif

#ifdef __cplusplus
}
#endif

#endif /* __LOG_CFG_H__ */

