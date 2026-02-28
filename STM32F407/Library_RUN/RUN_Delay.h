#ifndef _RUN_DELAY_H_
#define _RUN_DELAY_H_

#include "stm32f4xx.h" // 替换为F407的标准库头文件

// -----------------------------------------------------------
// 宏定义：兼容逐飞风格的数据类型
// -----------------------------------------------------------
#ifndef uint32
typedef uint32_t uint32;
#endif

#ifndef uint16
typedef uint16_t uint16;
#endif

#ifndef uint8
typedef uint8_t uint8;
#endif

// -----------------------------------------------------------
// 函数声明
// -----------------------------------------------------------

/**
 * @brief  延时初始化
 * @param  sysclk_mhz: 系统时钟频率 (STM32F407通常是 168)
 */
void RUN_delay_init(uint8_t sysclk_mhz);

/**
 * @brief  微秒级延时
 * @param  nus: 延时微秒数
 */
void RUN_delay_us(uint32_t nus);

/**
 * @brief  毫秒级延时
 * @param  nms: 延时毫秒数 (支持长延时)
 */
void RUN_delay_ms(uint16_t nms);

#endif
