#include "ADC_DMA.h"
#include <stddef.h>
// ================= 全局变量定义 =================
// 存放 ADC 原始数据的超级大数组
uint16_t ADC_Value_Buffer[ADC_BUF_SIZE];

// 数据就绪标志位：1表示有1024个数据采好了，主循环可以开始算 FFT 了
volatile uint8_t FFT_Ready_Flag = 0;

// 数据指针：指向刚刚采好的那 1024 个数据的首地址
volatile uint16_t* FFT_Process_Ptr = NULL;

// ================= 初始化函数 =================
// ================= 初始化函数 =================
void Audio_ADC_DMA_Init(void)
{
    GPIO_InitTypeDef       GPIO_InitStructure;
    ADC_InitTypeDef        ADC_InitStructure;
    ADC_CommonInitTypeDef  ADC_CommonInitStructure;
    DMA_InitTypeDef        DMA_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef       NVIC_InitStructure;

    // 1. 开启时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_DMA2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    // 2. 配置 GPIO: PA1 设为模拟输入 (请确保你的麦克风真的接在 PA1 上)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN; 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 3. 配置 DMA2 (增加结构体清零防玄学)
    DMA_DeInit(DMA2_Stream0);
    DMA_StructInit(&DMA_InitStructure); // 【补丁1】清除栈内存垃圾，防止突发传输模式错乱！
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;  
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADC_Value_Buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize = ADC_BUF_SIZE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);

    // 【补丁2】在开启中断前，暴力清除所有的历史中断标志位，防止“幽灵中断”假触发！
    DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0 | DMA_FLAG_HTIF0 | DMA_FLAG_TEIF0 | DMA_FLAG_DMEIF0 | DMA_FLAG_FEIF0);

    DMA_ITConfig(DMA2_Stream0, DMA_IT_HT | DMA_IT_TC, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    DMA_Cmd(DMA2_Stream0, ENABLE);

    // 4. 配置 ADC1 (增加结构体清零防玄学)
    ADC_DeInit(); // 复位ADC
    ADC_CommonStructInit(&ADC_CommonInitStructure); // 【补丁3】清除 ADC 通用寄存器垃圾
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStructure);

    ADC_StructInit(&ADC_InitStructure); // 【补丁4】清除 ADC 结构体垃圾
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising; 
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;        
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_15Cycles);
    
    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE); 
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);

    // 5. 配置 TIM3 (增加结构体清零)
    TIM_DeInit(TIM3); // 复位定时器
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); // 【补丁5】
    TIM_TimeBaseStructure.TIM_Period = 8399; 
    TIM_TimeBaseStructure.TIM_Prescaler = 0; 
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
    TIM_Cmd(TIM3, ENABLE); 
}

// ================= DMA 中断服务函数 =================
// 注意：如果你的工程中的 stm32f4xx_it.c 里已经有了这个函数，
// 请把这里的注释掉，并把代码搬到那边去；如果没有，直接放这就行。
void DMA2_Stream0_IRQHandler(void)
{
    // 1. 数组前半段（0 ~ 1023）填满了
    if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_HTIF0) != RESET) 
    {
        DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_HTIF0);
        
        FFT_Process_Ptr = &ADC_Value_Buffer[0]; 
        FFT_Ready_Flag = 1;
    }
    
    // 2. 数组后半段（1024 ~ 2047）填满了
    if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0) != RESET) 
    {
        DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
        
        FFT_Process_Ptr = &ADC_Value_Buffer[1024]; 
        FFT_Ready_Flag = 1;
    }
}