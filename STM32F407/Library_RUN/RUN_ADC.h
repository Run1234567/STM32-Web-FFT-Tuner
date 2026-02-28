#ifndef _RUN_ADC_H_
#define _RUN_ADC_H_

#include "stm32f4xx.h"
#include "RUN_Gpio.h" // 需要用到 GPIO 

// ==========================================================
// ADC 通道枚举 (对应引脚速查，STM32F407 的 ADC123 共有通道)
// ==========================================================
typedef enum {
    RUN_ADC_CH0_PA0 = ADC_Channel_0,
    RUN_ADC_CH1_PA1 = ADC_Channel_1,
    RUN_ADC_CH2_PA2 = ADC_Channel_2,
    RUN_ADC_CH3_PA3 = ADC_Channel_3,
    RUN_ADC_CH4_PA4 = ADC_Channel_4,
    RUN_ADC_CH5_PA5 = ADC_Channel_5,
    RUN_ADC_CH6_PA6 = ADC_Channel_6,
    RUN_ADC_CH7_PA7 = ADC_Channel_7,
    
    RUN_ADC_CH8_PB0 = ADC_Channel_8,
    RUN_ADC_CH9_PB1 = ADC_Channel_9,
    
    RUN_ADC_CH10_PC0 = ADC_Channel_10,
    RUN_ADC_CH11_PC1 = ADC_Channel_11,
    RUN_ADC_CH12_PC2 = ADC_Channel_12,
    RUN_ADC_CH13_PC3 = ADC_Channel_13,
    RUN_ADC_CH14_PC4 = ADC_Channel_14,
    RUN_ADC_CH15_PC5 = ADC_Channel_15,
    
    // 内部通道 (常用于检测内部温度或芯片供电)
    RUN_ADC_CH16_TEMP = ADC_Channel_16, 
    RUN_ADC_CH17_VREF = ADC_Channel_17
} RUN_ADC_CH_enum;

// ==========================================================
// 函数声明
// ==========================================================
void RUN_ADC_Init(void);
uint16_t RUN_ADC_Get(RUN_ADC_CH_enum ch);
uint16_t RUN_ADC_Get_Average(RUN_ADC_CH_enum ch, uint8_t times);

#endif