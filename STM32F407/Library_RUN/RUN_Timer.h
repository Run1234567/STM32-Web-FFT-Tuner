#ifndef _RUN_TIMER_H_
#define _RUN_TIMER_H_

#include "stm32f4xx.h"

// ==========================================================
// STM32F407 定 timer 全集枚举 (共 14 个)
// ==========================================================
typedef enum {
    RUN_TIM1,   // 高级定时器 (APB2)
    RUN_TIM2,   // 通用定时器 (APB1)
    RUN_TIM3,   // 通用定时器 (APB1)
    RUN_TIM4,   // 通用定时器 (APB1)
    RUN_TIM5,   // 通用定时器 (APB1)
    RUN_TIM6,   // 基本定时器 (APB1)
    RUN_TIM7,   // 基本定时器 (APB1)
    RUN_TIM8,   // 高级定时器 (APB2)
    RUN_TIM9,   // 通用定时器 (APB2)
    RUN_TIM10,  // 通用定时器 (APB2)
    RUN_TIM11,  // 通用定时器 (APB2)
    RUN_TIM12,  // 通用定时器 (APB1)
    RUN_TIM13,  // 通用定时器 (APB1)
    RUN_TIM14,  // 通用定时器 (APB1)
    
    RUN_TIM_MAX
} RUN_TIM_enum;

// ==========================================================
// 函数声明
// ==========================================================
void RUN_timer_init(RUN_TIM_enum tim_n, uint16_t time_ms);
void RUN_timer_cmd(RUN_TIM_enum tim_n, FunctionalState state);


#endif