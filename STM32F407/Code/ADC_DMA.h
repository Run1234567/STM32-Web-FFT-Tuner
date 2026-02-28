#ifndef __ADC_DMA_H
#define __ADC_DMA_H

#include "stm32f4xx.h"

// ================= 宏定义 =================
// FFT 点数设为 1024
#define FFT_LENGTH 1024
// DMA 缓冲区大小必须是 FFT 点数的 2 倍，实现双缓冲（Ping-Pong）
#define ADC_BUF_SIZE (FFT_LENGTH * 2) 

// ================= 全局变量声明 =================
// 使用 extern 声明，让 main.c 能够访问这些变量
extern uint16_t ADC_Value_Buffer[ADC_BUF_SIZE];
extern volatile uint8_t FFT_Ready_Flag;
extern volatile uint16_t* FFT_Process_Ptr;

// ================= 函数声明 =================
void Audio_ADC_DMA_Init(void);

#endif /* __AUDIO_ADC_DMA_H */