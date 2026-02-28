#include "RUN_header_file.h"

// 内部映射结构体
typedef struct {
    TIM_TypeDef* tim_base;
    uint32_t     rcc;
    uint8_t      bus_type; // 0:APB1(84M), 1:APB2(168M)
    IRQn_Type    irqn;
} timer_info_t;

// 严格对应枚举顺序的映射表
const timer_info_t timer_cfg[RUN_TIM_MAX] = {
    {TIM1,  RCC_APB2Periph_TIM1,  1, TIM1_UP_TIM10_IRQn},
    {TIM2,  RCC_APB1Periph_TIM2,  0, TIM2_IRQn},
    {TIM3,  RCC_APB1Periph_TIM3,  0, TIM3_IRQn},
    {TIM4,  RCC_APB1Periph_TIM4,  0, TIM4_IRQn},
    {TIM5,  RCC_APB1Periph_TIM5,  0, TIM5_IRQn},
    {TIM6,  RCC_APB1Periph_TIM6,  0, TIM6_DAC_IRQn},
    {TIM7,  RCC_APB1Periph_TIM7,  0, TIM7_IRQn},
    {TIM8,  RCC_APB2Periph_TIM8,  1, TIM8_UP_TIM13_IRQn},
    {TIM9,  RCC_APB2Periph_TIM9,  1, TIM1_BRK_TIM9_IRQn},
    {TIM10, RCC_APB2Periph_TIM10, 1, TIM1_UP_TIM10_IRQn},
    {TIM11, RCC_APB2Periph_TIM11, 1, TIM1_TRG_COM_TIM11_IRQn},
    {TIM12, RCC_APB1Periph_TIM12, 0, TIM8_BRK_TIM12_IRQn},
    {TIM13, RCC_APB1Periph_TIM13, 0, TIM8_UP_TIM13_IRQn},
    {TIM14, RCC_APB1Periph_TIM14, 0, TIM8_TRG_COM_TIM14_IRQn}
};
//-------------------------------------------------------------------------------------------------------------------
// 函数简介      定时器初始化配置 (适配 STM32F407)
// 参数说明      tim_n           定时器枚举 (如 RUN_TIM1, RUN_TIM2 等)
// 参数说明      time_ms         定时周期，单位为毫秒 (ms)
// 返回参数      void
// 使用示例      RUN_timer_init(RUN_TIM3, 500); // 初始化定时器3，每500ms触发一次中断
// 备注信息      底层自动识别 APB1(84MHz) 与 APB2(168MHz) 总线并计算预分频，自动重装载值基准为 10kHz。
//-------------------------------------------------------------------------------------------------------------------
void RUN_timer_init(RUN_TIM_enum tim_n, uint16_t time_ms)
{
    if (tim_n >= RUN_TIM_MAX) return;

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    uint32_t timer_clk;

    // 1. 根据总线开启时钟并确定频率
    if (timer_cfg[tim_n].bus_type == 1) {
        RCC_APB2PeriphClockCmd(timer_cfg[tim_n].rcc, ENABLE);
        timer_clk = 168000000; 
    } else {
        RCC_APB1PeriphClockCmd(timer_cfg[tim_n].rcc, ENABLE);
        timer_clk = 84000000;
    }

    // 2. 配置定时器基础参数 (设置为10kHz采样率，即0.1ms一步)
    TIM_TimeBaseStructure.TIM_Period = (time_ms * 10) - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = (timer_clk / 10000) - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(timer_cfg[tim_n].tim_base, &TIM_TimeBaseStructure);

    // 3. 配置中断
    TIM_ITConfig(timer_cfg[tim_n].tim_base, TIM_IT_Update, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = timer_cfg[tim_n].irqn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 4. 开启
    TIM_Cmd(timer_cfg[tim_n].tim_base, ENABLE);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介      定时器启停控制
// 参数说明      tim_n           定时器枚举 (如 RUN_TIM1)
// 参数说明      state           状态 (ENABLE: 开启定时器, DISABLE: 关闭定时器)
// 返回参数      void
// 使用示例      RUN_timer_cmd(RUN_TIM3, DISABLE); // 暂停定时器3的计数
// 备注信息      仅控制定时器的计数器使能(CEN位)，不改变初始化配置。
//-------------------------------------------------------------------------------------------------------------------
void RUN_timer_cmd(RUN_TIM_enum tim_n, FunctionalState state)
{
    if (tim_n < RUN_TIM_MAX)
        TIM_Cmd(timer_cfg[tim_n].tim_base, state);
}