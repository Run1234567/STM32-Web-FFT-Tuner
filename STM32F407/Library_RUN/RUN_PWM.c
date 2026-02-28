#include "RUN_header_file.h"

// 内部硬件配置抽象结构体
typedef struct {
    TIM_TypeDef* tim_base;   // 定时器外设基地址
    uint32_t     tim_rcc;    // 定时器时钟
    uint8_t      bus_type;   // 1:APB2(168M), 0:APB1(84M)
    
    GPIO_TypeDef* port;      // 端口
    uint16_t     pin;        // 引脚宏 (如 GPIO_Pin_8)
    uint32_t     gpio_rcc;   // 端口时钟 (AHB1)
    uint8_t      pin_source; // 引脚号 (0-15)
    uint8_t      af_value;   // AF 复用通道号
    
    uint8_t      channel;    // 定时器通道号 (1~4)
} pwm_info_t;

// 为了防报错而定义的 AF9 宏 (部分旧库可能漏掉)
#ifndef GPIO_AF_TIM12
#define GPIO_AF_TIM12 ((uint8_t)0x09)
#define GPIO_AF_TIM13 ((uint8_t)0x09)
#define GPIO_AF_TIM14 ((uint8_t)0x09)
#endif

// ==============================================================================
// 全映射表 (严格与枚举一对一对应)
// ==============================================================================
const pwm_info_t pwm_cfg[PWM_MAX] = {
    // --- TIM1 ---
    {TIM1, RCC_APB2Periph_TIM1, 1, GPIOA, GPIO_Pin_8,  RCC_AHB1Periph_GPIOA, 8,  GPIO_AF_TIM1, 1},
    {TIM1, RCC_APB2Periph_TIM1, 1, GPIOE, GPIO_Pin_9,  RCC_AHB1Periph_GPIOE, 9,  GPIO_AF_TIM1, 1},
    {TIM1, RCC_APB2Periph_TIM1, 1, GPIOA, GPIO_Pin_9,  RCC_AHB1Periph_GPIOA, 9,  GPIO_AF_TIM1, 2},
    {TIM1, RCC_APB2Periph_TIM1, 1, GPIOE, GPIO_Pin_11, RCC_AHB1Periph_GPIOE, 11, GPIO_AF_TIM1, 2},
    {TIM1, RCC_APB2Periph_TIM1, 1, GPIOA, GPIO_Pin_10, RCC_AHB1Periph_GPIOA, 10, GPIO_AF_TIM1, 3},
    {TIM1, RCC_APB2Periph_TIM1, 1, GPIOE, GPIO_Pin_13, RCC_AHB1Periph_GPIOE, 13, GPIO_AF_TIM1, 3},
    {TIM1, RCC_APB2Periph_TIM1, 1, GPIOA, GPIO_Pin_11, RCC_AHB1Periph_GPIOA, 11, GPIO_AF_TIM1, 4},
    {TIM1, RCC_APB2Periph_TIM1, 1, GPIOE, GPIO_Pin_14, RCC_AHB1Periph_GPIOE, 14, GPIO_AF_TIM1, 4},

    // --- TIM2 ---
    {TIM2, RCC_APB1Periph_TIM2, 0, GPIOA, GPIO_Pin_0,  RCC_AHB1Periph_GPIOA, 0,  GPIO_AF_TIM2, 1},
    {TIM2, RCC_APB1Periph_TIM2, 0, GPIOA, GPIO_Pin_5,  RCC_AHB1Periph_GPIOA, 5,  GPIO_AF_TIM2, 1},
    {TIM2, RCC_APB1Periph_TIM2, 0, GPIOA, GPIO_Pin_15, RCC_AHB1Periph_GPIOA, 15, GPIO_AF_TIM2, 1},
    {TIM2, RCC_APB1Periph_TIM2, 0, GPIOA, GPIO_Pin_1,  RCC_AHB1Periph_GPIOA, 1,  GPIO_AF_TIM2, 2},
    {TIM2, RCC_APB1Periph_TIM2, 0, GPIOB, GPIO_Pin_3,  RCC_AHB1Periph_GPIOB, 3,  GPIO_AF_TIM2, 2},
    {TIM2, RCC_APB1Periph_TIM2, 0, GPIOA, GPIO_Pin_2,  RCC_AHB1Periph_GPIOA, 2,  GPIO_AF_TIM2, 3},
    {TIM2, RCC_APB1Periph_TIM2, 0, GPIOB, GPIO_Pin_10, RCC_AHB1Periph_GPIOB, 10, GPIO_AF_TIM2, 3},
    {TIM2, RCC_APB1Periph_TIM2, 0, GPIOA, GPIO_Pin_3,  RCC_AHB1Periph_GPIOA, 3,  GPIO_AF_TIM2, 4},
    {TIM2, RCC_APB1Periph_TIM2, 0, GPIOB, GPIO_Pin_11, RCC_AHB1Periph_GPIOB, 11, GPIO_AF_TIM2, 4},

    // --- TIM3 ---
    {TIM3, RCC_APB1Periph_TIM3, 0, GPIOA, GPIO_Pin_6,  RCC_AHB1Periph_GPIOA, 6,  GPIO_AF_TIM3, 1},
    {TIM3, RCC_APB1Periph_TIM3, 0, GPIOB, GPIO_Pin_4,  RCC_AHB1Periph_GPIOB, 4,  GPIO_AF_TIM3, 1},
    {TIM3, RCC_APB1Periph_TIM3, 0, GPIOC, GPIO_Pin_6,  RCC_AHB1Periph_GPIOC, 6,  GPIO_AF_TIM3, 1},
    {TIM3, RCC_APB1Periph_TIM3, 0, GPIOA, GPIO_Pin_7,  RCC_AHB1Periph_GPIOA, 7,  GPIO_AF_TIM3, 2},
    {TIM3, RCC_APB1Periph_TIM3, 0, GPIOB, GPIO_Pin_5,  RCC_AHB1Periph_GPIOB, 5,  GPIO_AF_TIM3, 2},
    {TIM3, RCC_APB1Periph_TIM3, 0, GPIOC, GPIO_Pin_7,  RCC_AHB1Periph_GPIOC, 7,  GPIO_AF_TIM3, 2},
    {TIM3, RCC_APB1Periph_TIM3, 0, GPIOB, GPIO_Pin_0,  RCC_AHB1Periph_GPIOB, 0,  GPIO_AF_TIM3, 3},
    {TIM3, RCC_APB1Periph_TIM3, 0, GPIOC, GPIO_Pin_8,  RCC_AHB1Periph_GPIOC, 8,  GPIO_AF_TIM3, 3},
    {TIM3, RCC_APB1Periph_TIM3, 0, GPIOB, GPIO_Pin_1,  RCC_AHB1Periph_GPIOB, 1,  GPIO_AF_TIM3, 4},
    {TIM3, RCC_APB1Periph_TIM3, 0, GPIOC, GPIO_Pin_9,  RCC_AHB1Periph_GPIOC, 9,  GPIO_AF_TIM3, 4},

    // --- TIM4 ---
    {TIM4, RCC_APB1Periph_TIM4, 0, GPIOB, GPIO_Pin_6,  RCC_AHB1Periph_GPIOB, 6,  GPIO_AF_TIM4, 1},
    {TIM4, RCC_APB1Periph_TIM4, 0, GPIOD, GPIO_Pin_12, RCC_AHB1Periph_GPIOD, 12, GPIO_AF_TIM4, 1},
    {TIM4, RCC_APB1Periph_TIM4, 0, GPIOB, GPIO_Pin_7,  RCC_AHB1Periph_GPIOB, 7,  GPIO_AF_TIM4, 2},
    {TIM4, RCC_APB1Periph_TIM4, 0, GPIOD, GPIO_Pin_13, RCC_AHB1Periph_GPIOD, 13, GPIO_AF_TIM4, 2},
    {TIM4, RCC_APB1Periph_TIM4, 0, GPIOB, GPIO_Pin_8,  RCC_AHB1Periph_GPIOB, 8,  GPIO_AF_TIM4, 3},
    {TIM4, RCC_APB1Periph_TIM4, 0, GPIOD, GPIO_Pin_14, RCC_AHB1Periph_GPIOD, 14, GPIO_AF_TIM4, 3},
    {TIM4, RCC_APB1Periph_TIM4, 0, GPIOB, GPIO_Pin_9,  RCC_AHB1Periph_GPIOB, 9,  GPIO_AF_TIM4, 4},
    {TIM4, RCC_APB1Periph_TIM4, 0, GPIOD, GPIO_Pin_15, RCC_AHB1Periph_GPIOD, 15, GPIO_AF_TIM4, 4},

    // --- TIM5 ---
    {TIM5, RCC_APB1Periph_TIM5, 0, GPIOA, GPIO_Pin_0,  RCC_AHB1Periph_GPIOA, 0,  GPIO_AF_TIM5, 1},
    {TIM5, RCC_APB1Periph_TIM5, 0, GPIOH, GPIO_Pin_10, RCC_AHB1Periph_GPIOH, 10, GPIO_AF_TIM5, 1},
    {TIM5, RCC_APB1Periph_TIM5, 0, GPIOA, GPIO_Pin_1,  RCC_AHB1Periph_GPIOA, 1,  GPIO_AF_TIM5, 2},
    {TIM5, RCC_APB1Periph_TIM5, 0, GPIOH, GPIO_Pin_11, RCC_AHB1Periph_GPIOH, 11, GPIO_AF_TIM5, 2},
    {TIM5, RCC_APB1Periph_TIM5, 0, GPIOA, GPIO_Pin_2,  RCC_AHB1Periph_GPIOA, 2,  GPIO_AF_TIM5, 3},
    {TIM5, RCC_APB1Periph_TIM5, 0, GPIOH, GPIO_Pin_12, RCC_AHB1Periph_GPIOH, 12, GPIO_AF_TIM5, 3},
    {TIM5, RCC_APB1Periph_TIM5, 0, GPIOA, GPIO_Pin_3,  RCC_AHB1Periph_GPIOA, 3,  GPIO_AF_TIM5, 4},
    {TIM5, RCC_APB1Periph_TIM5, 0, GPIOI, GPIO_Pin_0,  RCC_AHB1Periph_GPIOI, 0,  GPIO_AF_TIM5, 4},

    // --- TIM8 ---
    {TIM8, RCC_APB2Periph_TIM8, 1, GPIOC, GPIO_Pin_6,  RCC_AHB1Periph_GPIOC, 6,  GPIO_AF_TIM8, 1},
    {TIM8, RCC_APB2Periph_TIM8, 1, GPIOI, GPIO_Pin_5,  RCC_AHB1Periph_GPIOI, 5,  GPIO_AF_TIM8, 1},
    {TIM8, RCC_APB2Periph_TIM8, 1, GPIOC, GPIO_Pin_7,  RCC_AHB1Periph_GPIOC, 7,  GPIO_AF_TIM8, 2},
    {TIM8, RCC_APB2Periph_TIM8, 1, GPIOI, GPIO_Pin_6,  RCC_AHB1Periph_GPIOI, 6,  GPIO_AF_TIM8, 2},
    {TIM8, RCC_APB2Periph_TIM8, 1, GPIOC, GPIO_Pin_8,  RCC_AHB1Periph_GPIOC, 8,  GPIO_AF_TIM8, 3},
    {TIM8, RCC_APB2Periph_TIM8, 1, GPIOI, GPIO_Pin_7,  RCC_AHB1Periph_GPIOI, 7,  GPIO_AF_TIM8, 3},
    {TIM8, RCC_APB2Periph_TIM8, 1, GPIOC, GPIO_Pin_9,  RCC_AHB1Periph_GPIOC, 9,  GPIO_AF_TIM8, 4},
    {TIM8, RCC_APB2Periph_TIM8, 1, GPIOI, GPIO_Pin_2,  RCC_AHB1Periph_GPIOI, 2,  GPIO_AF_TIM8, 4},

    // --- TIM9 ---
    {TIM9, RCC_APB2Periph_TIM9, 1, GPIOA, GPIO_Pin_2,  RCC_AHB1Periph_GPIOA, 2,  GPIO_AF_TIM9, 1},
    {TIM9, RCC_APB2Periph_TIM9, 1, GPIOE, GPIO_Pin_5,  RCC_AHB1Periph_GPIOE, 5,  GPIO_AF_TIM9, 1},
    {TIM9, RCC_APB2Periph_TIM9, 1, GPIOA, GPIO_Pin_3,  RCC_AHB1Periph_GPIOA, 3,  GPIO_AF_TIM9, 2},
    {TIM9, RCC_APB2Periph_TIM9, 1, GPIOE, GPIO_Pin_6,  RCC_AHB1Periph_GPIOE, 6,  GPIO_AF_TIM9, 2},

    // --- TIM10 ---
    {TIM10, RCC_APB2Periph_TIM10, 1, GPIOB, GPIO_Pin_8, RCC_AHB1Periph_GPIOB, 8, GPIO_AF_TIM10, 1},
    {TIM10, RCC_APB2Periph_TIM10, 1, GPIOF, GPIO_Pin_6, RCC_AHB1Periph_GPIOF, 6, GPIO_AF_TIM10, 1},

    // --- TIM11 ---
    {TIM11, RCC_APB2Periph_TIM11, 1, GPIOB, GPIO_Pin_9, RCC_AHB1Periph_GPIOB, 9, GPIO_AF_TIM11, 1},
    {TIM11, RCC_APB2Periph_TIM11, 1, GPIOF, GPIO_Pin_7, RCC_AHB1Periph_GPIOF, 7, GPIO_AF_TIM11, 1},

    // --- TIM12 ---
    {TIM12, RCC_APB1Periph_TIM12, 0, GPIOB, GPIO_Pin_14, RCC_AHB1Periph_GPIOB, 14, GPIO_AF_TIM12, 1},
    {TIM12, RCC_APB1Periph_TIM12, 0, GPIOH, GPIO_Pin_6,  RCC_AHB1Periph_GPIOH, 6,  GPIO_AF_TIM12, 1},
    {TIM12, RCC_APB1Periph_TIM12, 0, GPIOB, GPIO_Pin_15, RCC_AHB1Periph_GPIOB, 15, GPIO_AF_TIM12, 2},
    {TIM12, RCC_APB1Periph_TIM12, 0, GPIOH, GPIO_Pin_9,  RCC_AHB1Periph_GPIOH, 9,  GPIO_AF_TIM12, 2},

    // --- TIM13 ---
    {TIM13, RCC_APB1Periph_TIM13, 0, GPIOA, GPIO_Pin_6,  RCC_AHB1Periph_GPIOA, 6,  GPIO_AF_TIM13, 1},
    {TIM13, RCC_APB1Periph_TIM13, 0, GPIOF, GPIO_Pin_8,  RCC_AHB1Periph_GPIOF, 8,  GPIO_AF_TIM13, 1},

    // --- TIM14 ---
    {TIM14, RCC_APB1Periph_TIM14, 0, GPIOA, GPIO_Pin_7,  RCC_AHB1Periph_GPIOA, 7,  GPIO_AF_TIM14, 1},
    {TIM14, RCC_APB1Periph_TIM14, 0, GPIOF, GPIO_Pin_9,  RCC_AHB1Periph_GPIOF, 9,  GPIO_AF_TIM14, 1}
};

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      PWM 初始化
// 参数说明      pwm_ch          PWM通道枚举 (例如: PWM_TIM1_CH1_PA8)
// 参数说明      freq            PWM 初始频率 (Hz)
// 参数说明      duty_cycle      初始占空比，范围 0 ~ 10000 (代表 0.00% ~ 100.00%)
// 返回参数      void
// 使用示例      RUN_pwm_init(PWM_TIM1_CH1_PA8, 1000, 5000); // 在 PA8 输出 1kHz 频率，50% 占空比的方波
// 备注信息      内部已自动处理 F407 的引脚复用(AF)配置，以及高级定时器(TIM1/8)的 MOE 主输出使能。
//-------------------------------------------------------------------------------------------------------------------
void RUN_pwm_init(RUN_PWM_enum pwm_ch, uint32_t freq, uint16_t duty_cycle)
{
    if (pwm_ch >= PWM_MAX) return;

    GPIO_InitTypeDef GPIO_InitStructure = {0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
    TIM_OCInitTypeDef TIM_OCInitStructure = {0};
    uint32_t timer_clk;

    // 1. 开启时钟
    RCC_AHB1PeriphClockCmd(pwm_cfg[pwm_ch].gpio_rcc, ENABLE);
    if (pwm_cfg[pwm_ch].bus_type == 1) {
        RCC_APB2PeriphClockCmd(pwm_cfg[pwm_ch].tim_rcc, ENABLE);
        timer_clk = 168000000; // APB2 高速总线
    } else {
        RCC_APB1PeriphClockCmd(pwm_cfg[pwm_ch].tim_rcc, ENABLE);
        timer_clk = 84000000;  // APB1 低速总线
    }

    // 2. 引脚复用映射
    GPIO_PinAFConfig(pwm_cfg[pwm_ch].port, pwm_cfg[pwm_ch].pin_source, pwm_cfg[pwm_ch].af_value);
    
    // 3. 配置 GPIO
    GPIO_InitStructure.GPIO_Pin = pwm_cfg[pwm_ch].pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;      
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(pwm_cfg[pwm_ch].port, &GPIO_InitStructure);

    // =========================================================================
    // 4. 【核心修复】动态计算 PSC 和 ARR，支持极高频率 (如 20kHz, 100kHz)
    // =========================================================================
    uint32_t period_cycles = timer_clk / freq; 
    uint16_t psc_val = 0;
    uint32_t arr_val = 0;

    // 如果所需周期大于 16位定时器的最大值 65536，则增加预分频
    if (period_cycles > 65536) {
        psc_val = period_cycles / 65536;
        arr_val = period_cycles / (psc_val + 1);
    } else {
        psc_val = 0;  // 高频时，PSC为0，确保最高占空比分辨率
        arr_val = period_cycles;
    }

    TIM_TimeBaseStructure.TIM_Period = arr_val - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = psc_val;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; 
    TIM_TimeBaseInit(pwm_cfg[pwm_ch].tim_base, &TIM_TimeBaseStructure);

    // 5. 输出比较通道配置 (动态映射 0~10000 到真实的 arr_val)
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = ((uint32_t)duty_cycle * arr_val) / 10000; 
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;

    switch(pwm_cfg[pwm_ch].channel) {
        case 1: 
            TIM_OC1Init(pwm_cfg[pwm_ch].tim_base, &TIM_OCInitStructure); 
            TIM_OC1PreloadConfig(pwm_cfg[pwm_ch].tim_base, TIM_OCPreload_Enable);
            break;
        case 2: 
            TIM_OC2Init(pwm_cfg[pwm_ch].tim_base, &TIM_OCInitStructure); 
            TIM_OC2PreloadConfig(pwm_cfg[pwm_ch].tim_base, TIM_OCPreload_Enable);
            break;
        case 3: 
            TIM_OC3Init(pwm_cfg[pwm_ch].tim_base, &TIM_OCInitStructure); 
            TIM_OC3PreloadConfig(pwm_cfg[pwm_ch].tim_base, TIM_OCPreload_Enable);
            break;
        case 4: 
            TIM_OC4Init(pwm_cfg[pwm_ch].tim_base, &TIM_OCInitStructure); 
            TIM_OC4PreloadConfig(pwm_cfg[pwm_ch].tim_base, TIM_OCPreload_Enable);
            break;
    }

    // 6. 使能预装载
    TIM_ARRPreloadConfig(pwm_cfg[pwm_ch].tim_base, ENABLE);

    // 7. 高级定时器 (TIM1/8) 使能 MOE
    if (pwm_cfg[pwm_ch].tim_base == TIM1 || pwm_cfg[pwm_ch].tim_base == TIM8) {
        TIM_CtrlPWMOutputs(pwm_cfg[pwm_ch].tim_base, ENABLE);
    }

    // 8. 开启定时器
    TIM_Cmd(pwm_cfg[pwm_ch].tim_base, ENABLE);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      动态调节 PWM 占空比
// 参数说明      pwm_ch          PWM通道枚举
// 参数说明      duty_cycle      目标占空比，范围 0 ~ 10000 (代表 0.00% ~ 100.00%)
// 返回参数      void
// 使用示例      RUN_pwm_set(PWM_TIM1_CH1_PA8, 7500); // 运行时将占空比无缝修改为 75%
// 备注信息      步进精度极高(0.01%)，非常适合用于平滑的电机无级调速或细腻的呼吸灯效果。
//-------------------------------------------------------------------------------------------------------------------
void RUN_pwm_set(RUN_PWM_enum pwm_ch, uint16_t duty_cycle)
{
    if (pwm_ch >= PWM_MAX) return;
    
    // 获取当前真实 ARR 的值，重新计算目标捕获比较值 (CCR)
    uint32_t arr = pwm_cfg[pwm_ch].tim_base->ARR + 1;
    uint32_t ccr_val = ((uint32_t)duty_cycle * arr) / 10000;

    switch(pwm_cfg[pwm_ch].channel) {
        case 1: TIM_SetCompare1(pwm_cfg[pwm_ch].tim_base, ccr_val); break;
        case 2: TIM_SetCompare2(pwm_cfg[pwm_ch].tim_base, ccr_val); break;
        case 3: TIM_SetCompare3(pwm_cfg[pwm_ch].tim_base, ccr_val); break;
        case 4: TIM_SetCompare4(pwm_cfg[pwm_ch].tim_base, ccr_val); break;
    }
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介      动态修改 PWM 频率
// 参数说明      pwm_ch          PWM通道枚举
// 参数说明      freq            新频率 (Hz)
// 返回参数      void
// 使用示例      RUN_pwm_freq(PWM_TIM1_CH1_PA8, 5000); // 运行时将频率改为 5kHz
// 备注信息      注意：本驱动库底层通过修改预分频器(PSC)来调节频率，自动重装载寄存器(ARR)始终固定为10000。
//               因此，修改频率后，原本设定的占空比比例【不会】发生变化，无需重新调用 RUN_pwm_set。
//-------------------------------------------------------------------------------------------------------------------
void RUN_pwm_freq(RUN_PWM_enum pwm_ch, uint32_t freq)
{
    if (pwm_ch >= PWM_MAX) return;
    if (freq == 0) return;

    uint32_t timer_clk = (pwm_cfg[pwm_ch].bus_type == 1) ? 168000000 : 84000000;
    
    // 重新计算并应用新的 ARR 和 PSC
    uint32_t period_cycles = timer_clk / freq;
    uint16_t psc_val = 0;
    uint32_t arr_val = 0;

    if (period_cycles > 65536) {
        psc_val = period_cycles / 65536;
        arr_val = period_cycles / (psc_val + 1);
    } else {
        psc_val = 0;
        arr_val = period_cycles;
    }

    // 动态更新预分频器和重装载寄存器
    TIM_PrescalerConfig(pwm_cfg[pwm_ch].tim_base, psc_val, TIM_PSCReloadMode_Immediate);
    TIM_SetAutoreload(pwm_cfg[pwm_ch].tim_base, arr_val - 1);
}