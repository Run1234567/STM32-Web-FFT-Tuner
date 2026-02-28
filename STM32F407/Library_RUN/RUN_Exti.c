#include "RUN_header_file.h"

// ==============================================================================
// 全局变量管理
// ==============================================================================
// 回调函数指针数组，保存用户注册的中断处理函数 (索引 0~15 对应 EXTI_Line0 ~ EXTI_Line15)
static ExtiCallback_t exti_callbacks[16] = {0};

// ==============================================================================
// 内部辅助函数
// ==============================================================================

/**
 * @brief  根据 GPIO_Pin 计算引脚源索引 (0~15)
 */
static uint8_t RUN_get_pin_source(uint16_t gpio_pin)
{
    uint8_t source = 0;
    while ((gpio_pin >> source) != 1) {
        source++;
        if(source > 15) return 0; 
    }
    return source;
}

/**
 * @brief  根据 GPIO_Port 地址计算端口源索引 (适配 F4 的 GPIOA~GPIOI)
 */
static uint8_t RUN_get_port_source(GPIO_TypeDef* gpio_port)
{
    if      (gpio_port == GPIOA) return 0;
    else if (gpio_port == GPIOB) return 1;
    else if (gpio_port == GPIOC) return 2;
    else if (gpio_port == GPIOD) return 3;
    else if (gpio_port == GPIOE) return 4;
    else if (gpio_port == GPIOF) return 5;
    else if (gpio_port == GPIOG) return 6;
    else if (gpio_port == GPIOH) return 7;
    else if (gpio_port == GPIOI) return 8;
    return 0;
}

/**
 * @brief  根据引脚号分配 NVIC 中断通道
 */
static IRQn_Type RUN_get_irqn(uint8_t pin_source)
{
    if (pin_source == 0) return EXTI0_IRQn;
    if (pin_source == 1) return EXTI1_IRQn;
    if (pin_source == 2) return EXTI2_IRQn;
    if (pin_source == 3) return EXTI3_IRQn;
    if (pin_source == 4) return EXTI4_IRQn;
    if (pin_source >= 5 && pin_source <= 9) return EXTI9_5_IRQn;
    return EXTI15_10_IRQn;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      外部中断初始化配置 (自带 GPIO 自动初始化版)
// 参数说明      line_pin        外部中断引脚枚举 (例如: EXTI_Line0_PA0)
// 参数说明      gpio_mode       引脚输入模式 (GPI:浮空, GPI_PU:上拉, GPI_PD:下拉)
// 参数说明      trigger         触发方式 (EXTI_Trigger_Rising, EXTI_Trigger_Falling, EXTI_Trigger_Rising_Falling)
// 参数说明      callback        中断触发时调用的回调函数指针
// 使用示例      RUN_exti_init(EXTI_Line0_PA0, GPI_PU, EXTI_Trigger_Falling, Key_Callback); 
// 备注信息      内部已自动调用 RUN_gpio_init，无需再在外部单独初始化引脚。
//-------------------------------------------------------------------------------------------------------------------
void RUN_exti_init(RUN_EXTI_enum line_pin, RUN_GPIO_Mode gpio_mode, EXTITrigger_TypeDef trigger, ExtiCallback_t callback)
{
    if (line_pin >= RUN_GPIO_MAX) return;

    // =========================================================================
    // 【新增】: 自动进行 GPIO 物理层初始化！
    // 强制将 EXTI 枚举转换为 GPIO 枚举 (因为你的底层枚举值是一一对应的)
    // 默认电平参数填 1，仅对输出模式有效，输入模式填什么无所谓
    RUN_gpio_init((RUN_GPIO_enum)line_pin, gpio_mode, 1); 
    // =========================================================================

    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 获取端口和引脚信息
    uint16_t gpio_pin = gpio_cfg[line_pin].pin;
    GPIO_TypeDef* gpio_port = gpio_cfg[line_pin].port;
    
    uint8_t port_source = RUN_get_port_source(gpio_port);
    uint8_t pin_source  = RUN_get_pin_source(gpio_pin);

    // 1. 记录回调函数
    exti_callbacks[pin_source] = callback;

    // 2. 开启 SYSCFG 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    // 3. 映射 EXTI 线路
    SYSCFG_EXTILineConfig(port_source, pin_source);

    // 4. 配置 EXTI
    uint32_t exti_line = (uint32_t)(1 << pin_source); 
    EXTI_InitStructure.EXTI_Line = exti_line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = trigger;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // 5. 配置 NVIC
    NVIC_InitStructure.NVIC_IRQChannel = RUN_get_irqn(pin_source);
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;        
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      使能或禁用某个外部中断线
//-------------------------------------------------------------------------------------------------------------------
void RUN_exti_cmd(RUN_EXTI_enum line_pin, FunctionalState state)
{
    if (line_pin >= RUN_GPIO_MAX) return;

    uint8_t pin_source = RUN_get_pin_source(gpio_cfg[line_pin].pin);
    uint32_t exti_line = (uint32_t)(1 << pin_source);

    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = exti_line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // 状态切换时触发边沿并不重要
    EXTI_InitStructure.EXTI_LineCmd = state;
    EXTI_Init(&EXTI_InitStructure);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      软件触发一次外部中断
//-------------------------------------------------------------------------------------------------------------------
void RUN_exti_sw_trigger(RUN_EXTI_enum line_pin)
{
    if (line_pin >= RUN_GPIO_MAX) return;
    
    uint8_t pin_source = RUN_get_pin_source(gpio_cfg[line_pin].pin);
    uint32_t exti_line = (uint32_t)(1 << pin_source);
    
    EXTI_GenerateSWInterrupt(exti_line);
}

// ==============================================================================
// 中断服务函数 (IRQ Handlers) - 与 F1 完全一致
// ==============================================================================

/**
 * @brief  通用中断处理分发器
 */
static void RUN_EXTI_Handler(uint32_t line, uint8_t index)
{
    // 1. 检查标志位 (确认该线是否真的产生了中断)
    if (EXTI_GetITStatus(line) != RESET)
    {
        // 2. 执行用户注册的回调函数
        if (exti_callbacks[index]) exti_callbacks[index]();
        
        // 3. 清除挂起标志位 (必须清除，否则会无限进入中断)
        EXTI_ClearITPendingBit(line);
    }
}

// --- 独立中断处理 (Line 0 ~ 4) ---
void EXTI0_IRQHandler(void) { RUN_EXTI_Handler(EXTI_Line0, 0); }
void EXTI1_IRQHandler(void) { RUN_EXTI_Handler(EXTI_Line1, 1); }
void EXTI2_IRQHandler(void) { RUN_EXTI_Handler(EXTI_Line2, 2); }
void EXTI3_IRQHandler(void) { RUN_EXTI_Handler(EXTI_Line3, 3); }
void EXTI4_IRQHandler(void) { RUN_EXTI_Handler(EXTI_Line4, 4); }

// --- 共享中断处理 (Line 5~9, 10~15) ---
void EXTI9_5_IRQHandler(void)
{
    RUN_EXTI_Handler(EXTI_Line5, 5);
    RUN_EXTI_Handler(EXTI_Line6, 6);
    RUN_EXTI_Handler(EXTI_Line7, 7);
    RUN_EXTI_Handler(EXTI_Line8, 8);
    RUN_EXTI_Handler(EXTI_Line9, 9);
}

void EXTI15_10_IRQHandler(void)
{
    RUN_EXTI_Handler(EXTI_Line10, 10);
    RUN_EXTI_Handler(EXTI_Line11, 11);
    RUN_EXTI_Handler(EXTI_Line12, 12);
    RUN_EXTI_Handler(EXTI_Line13, 13);
    RUN_EXTI_Handler(EXTI_Line14, 14);
    RUN_EXTI_Handler(EXTI_Line15, 15);
}