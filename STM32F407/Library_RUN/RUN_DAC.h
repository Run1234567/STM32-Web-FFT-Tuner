#ifndef _RUN_DAC_H_
#define _RUN_DAC_H_

#include "stm32f4xx.h"

// ==============================================================================
// DAC 通道引脚全集枚举 (严格适配 STM32F407)
// 备注：F407 硬件上仅有这两个引脚支持真正的模拟 DAC 输出，无法重映射。
// ==============================================================================
typedef enum {
    RUN_DAC_CH1_PA4 = 1, // 通道1，固定对应 PA4
    RUN_DAC_CH2_PA5 = 2  // 通道2，固定对应 PA5
} RUN_DAC_Channel_t;

// ==============================================================================
// 函数声明
// ==============================================================================
void RUN_DAC_Init(RUN_DAC_Channel_t channel);
void RUN_DAC_Set_Vol(RUN_DAC_Channel_t channel, float vol);
void RUN_DAC_Set_Value(RUN_DAC_Channel_t channel, uint16_t val);

#endif