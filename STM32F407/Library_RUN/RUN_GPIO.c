#include "RUN_GPIO.h" 

// ===============================================================================
// 硬件映射表 (注意：类型必须是 gpio_info_t，顺序必须对应枚举)
// F4 系列 GPIO 挂载在 AHB1 总线
// ===============================================================================
const gpio_info_t gpio_cfg[RUN_GPIO_MAX] = {
    // --- GPIO A (0~15) ---
    {GPIOA, GPIO_Pin_0, RCC_AHB1Periph_GPIOA}, {GPIOA, GPIO_Pin_1, RCC_AHB1Periph_GPIOA},
    {GPIOA, GPIO_Pin_2, RCC_AHB1Periph_GPIOA}, {GPIOA, GPIO_Pin_3, RCC_AHB1Periph_GPIOA},
    {GPIOA, GPIO_Pin_4, RCC_AHB1Periph_GPIOA}, {GPIOA, GPIO_Pin_5, RCC_AHB1Periph_GPIOA},
    {GPIOA, GPIO_Pin_6, RCC_AHB1Periph_GPIOA}, {GPIOA, GPIO_Pin_7, RCC_AHB1Periph_GPIOA},
    {GPIOA, GPIO_Pin_8, RCC_AHB1Periph_GPIOA}, {GPIOA, GPIO_Pin_9, RCC_AHB1Periph_GPIOA},
    {GPIOA, GPIO_Pin_10,RCC_AHB1Periph_GPIOA}, {GPIOA, GPIO_Pin_11,RCC_AHB1Periph_GPIOA},
    {GPIOA, GPIO_Pin_12,RCC_AHB1Periph_GPIOA}, {GPIOA, GPIO_Pin_13,RCC_AHB1Periph_GPIOA},
    {GPIOA, GPIO_Pin_14,RCC_AHB1Periph_GPIOA}, {GPIOA, GPIO_Pin_15,RCC_AHB1Periph_GPIOA},

    // --- GPIO B (16~31) ---
    {GPIOB, GPIO_Pin_0, RCC_AHB1Periph_GPIOB}, {GPIOB, GPIO_Pin_1, RCC_AHB1Periph_GPIOB},
    {GPIOB, GPIO_Pin_2, RCC_AHB1Periph_GPIOB}, {GPIOB, GPIO_Pin_3, RCC_AHB1Periph_GPIOB},
    {GPIOB, GPIO_Pin_4, RCC_AHB1Periph_GPIOB}, {GPIOB, GPIO_Pin_5, RCC_AHB1Periph_GPIOB},
    {GPIOB, GPIO_Pin_6, RCC_AHB1Periph_GPIOB}, {GPIOB, GPIO_Pin_7, RCC_AHB1Periph_GPIOB},
    {GPIOB, GPIO_Pin_8, RCC_AHB1Periph_GPIOB}, {GPIOB, GPIO_Pin_9, RCC_AHB1Periph_GPIOB},
    {GPIOB, GPIO_Pin_10,RCC_AHB1Periph_GPIOB}, {GPIOB, GPIO_Pin_11,RCC_AHB1Periph_GPIOB},
    {GPIOB, GPIO_Pin_12,RCC_AHB1Periph_GPIOB}, {GPIOB, GPIO_Pin_13,RCC_AHB1Periph_GPIOB},
    {GPIOB, GPIO_Pin_14,RCC_AHB1Periph_GPIOB}, {GPIOB, GPIO_Pin_15,RCC_AHB1Periph_GPIOB},

    // --- GPIO C (32~47) ---
    {GPIOC, GPIO_Pin_0, RCC_AHB1Periph_GPIOC}, {GPIOC, GPIO_Pin_1, RCC_AHB1Periph_GPIOC},
    {GPIOC, GPIO_Pin_2, RCC_AHB1Periph_GPIOC}, {GPIOC, GPIO_Pin_3, RCC_AHB1Periph_GPIOC},
    {GPIOC, GPIO_Pin_4, RCC_AHB1Periph_GPIOC}, {GPIOC, GPIO_Pin_5, RCC_AHB1Periph_GPIOC},
    {GPIOC, GPIO_Pin_6, RCC_AHB1Periph_GPIOC}, {GPIOC, GPIO_Pin_7, RCC_AHB1Periph_GPIOC},
    {GPIOC, GPIO_Pin_8, RCC_AHB1Periph_GPIOC}, {GPIOC, GPIO_Pin_9, RCC_AHB1Periph_GPIOC},
    {GPIOC, GPIO_Pin_10,RCC_AHB1Periph_GPIOC}, {GPIOC, GPIO_Pin_11,RCC_AHB1Periph_GPIOC},
    {GPIOC, GPIO_Pin_12,RCC_AHB1Periph_GPIOC}, {GPIOC, GPIO_Pin_13,RCC_AHB1Periph_GPIOC},
    {GPIOC, GPIO_Pin_14,RCC_AHB1Periph_GPIOC}, {GPIOC, GPIO_Pin_15,RCC_AHB1Periph_GPIOC},

    // --- GPIO D (48~63) ---
    {GPIOD, GPIO_Pin_0, RCC_AHB1Periph_GPIOD}, {GPIOD, GPIO_Pin_1, RCC_AHB1Periph_GPIOD},
    {GPIOD, GPIO_Pin_2, RCC_AHB1Periph_GPIOD}, {GPIOD, GPIO_Pin_3, RCC_AHB1Periph_GPIOD},
    {GPIOD, GPIO_Pin_4, RCC_AHB1Periph_GPIOD}, {GPIOD, GPIO_Pin_5, RCC_AHB1Periph_GPIOD},
    {GPIOD, GPIO_Pin_6, RCC_AHB1Periph_GPIOD}, {GPIOD, GPIO_Pin_7, RCC_AHB1Periph_GPIOD},
    {GPIOD, GPIO_Pin_8, RCC_AHB1Periph_GPIOD}, {GPIOD, GPIO_Pin_9, RCC_AHB1Periph_GPIOD},
    {GPIOD, GPIO_Pin_10,RCC_AHB1Periph_GPIOD}, {GPIOD, GPIO_Pin_11,RCC_AHB1Periph_GPIOD},
    {GPIOD, GPIO_Pin_12,RCC_AHB1Periph_GPIOD}, {GPIOD, GPIO_Pin_13,RCC_AHB1Periph_GPIOD},
    {GPIOD, GPIO_Pin_14,RCC_AHB1Periph_GPIOD}, {GPIOD, GPIO_Pin_15,RCC_AHB1Periph_GPIOD},

    // --- GPIO E (64~79) ---
    {GPIOE, GPIO_Pin_0, RCC_AHB1Periph_GPIOE}, {GPIOE, GPIO_Pin_1, RCC_AHB1Periph_GPIOE},
    {GPIOE, GPIO_Pin_2, RCC_AHB1Periph_GPIOE}, {GPIOE, GPIO_Pin_3, RCC_AHB1Periph_GPIOE},
    {GPIOE, GPIO_Pin_4, RCC_AHB1Periph_GPIOE}, {GPIOE, GPIO_Pin_5, RCC_AHB1Periph_GPIOE},
    {GPIOE, GPIO_Pin_6, RCC_AHB1Periph_GPIOE}, {GPIOE, GPIO_Pin_7, RCC_AHB1Periph_GPIOE},
    {GPIOE, GPIO_Pin_8, RCC_AHB1Periph_GPIOE}, {GPIOE, GPIO_Pin_9, RCC_AHB1Periph_GPIOE},
    {GPIOE, GPIO_Pin_10,RCC_AHB1Periph_GPIOE}, {GPIOE, GPIO_Pin_11,RCC_AHB1Periph_GPIOE},
    {GPIOE, GPIO_Pin_12,RCC_AHB1Periph_GPIOE}, {GPIOE, GPIO_Pin_13,RCC_AHB1Periph_GPIOE},
    {GPIOE, GPIO_Pin_14,RCC_AHB1Periph_GPIOE}, {GPIOE, GPIO_Pin_15,RCC_AHB1Periph_GPIOE},

    // --- GPIO F (80~95) ---
    {GPIOF, GPIO_Pin_0, RCC_AHB1Periph_GPIOF}, {GPIOF, GPIO_Pin_1, RCC_AHB1Periph_GPIOF},
    {GPIOF, GPIO_Pin_2, RCC_AHB1Periph_GPIOF}, {GPIOF, GPIO_Pin_3, RCC_AHB1Periph_GPIOF},
    {GPIOF, GPIO_Pin_4, RCC_AHB1Periph_GPIOF}, {GPIOF, GPIO_Pin_5, RCC_AHB1Periph_GPIOF},
    {GPIOF, GPIO_Pin_6, RCC_AHB1Periph_GPIOF}, {GPIOF, GPIO_Pin_7, RCC_AHB1Periph_GPIOF},
    {GPIOF, GPIO_Pin_8, RCC_AHB1Periph_GPIOF}, {GPIOF, GPIO_Pin_9, RCC_AHB1Periph_GPIOF},
    {GPIOF, GPIO_Pin_10,RCC_AHB1Periph_GPIOF}, {GPIOF, GPIO_Pin_11,RCC_AHB1Periph_GPIOF},
    {GPIOF, GPIO_Pin_12,RCC_AHB1Periph_GPIOF}, {GPIOF, GPIO_Pin_13,RCC_AHB1Periph_GPIOF},
    {GPIOF, GPIO_Pin_14,RCC_AHB1Periph_GPIOF}, {GPIOF, GPIO_Pin_15,RCC_AHB1Periph_GPIOF},

    // --- GPIO G (96~111) ---
    {GPIOG, GPIO_Pin_0, RCC_AHB1Periph_GPIOG}, {GPIOG, GPIO_Pin_1, RCC_AHB1Periph_GPIOG},
    {GPIOG, GPIO_Pin_2, RCC_AHB1Periph_GPIOG}, {GPIOG, GPIO_Pin_3, RCC_AHB1Periph_GPIOG},
    {GPIOG, GPIO_Pin_4, RCC_AHB1Periph_GPIOG}, {GPIOG, GPIO_Pin_5, RCC_AHB1Periph_GPIOG},
    {GPIOG, GPIO_Pin_6, RCC_AHB1Periph_GPIOG}, {GPIOG, GPIO_Pin_7, RCC_AHB1Periph_GPIOG},
    {GPIOG, GPIO_Pin_8, RCC_AHB1Periph_GPIOG}, {GPIOG, GPIO_Pin_9, RCC_AHB1Periph_GPIOG},
    {GPIOG, GPIO_Pin_10,RCC_AHB1Periph_GPIOG}, {GPIOG, GPIO_Pin_11,RCC_AHB1Periph_GPIOG},
    {GPIOG, GPIO_Pin_12,RCC_AHB1Periph_GPIOG}, {GPIOG, GPIO_Pin_13,RCC_AHB1Periph_GPIOG},
    {GPIOG, GPIO_Pin_14,RCC_AHB1Periph_GPIOG}, {GPIOG, GPIO_Pin_15,RCC_AHB1Periph_GPIOG},

    // --- GPIO H (112~127) ---
    {GPIOH, GPIO_Pin_0, RCC_AHB1Periph_GPIOH}, {GPIOH, GPIO_Pin_1, RCC_AHB1Periph_GPIOH},
    {GPIOH, GPIO_Pin_2, RCC_AHB1Periph_GPIOH}, {GPIOH, GPIO_Pin_3, RCC_AHB1Periph_GPIOH},
    {GPIOH, GPIO_Pin_4, RCC_AHB1Periph_GPIOH}, {GPIOH, GPIO_Pin_5, RCC_AHB1Periph_GPIOH},
    {GPIOH, GPIO_Pin_6, RCC_AHB1Periph_GPIOH}, {GPIOH, GPIO_Pin_7, RCC_AHB1Periph_GPIOH},
    {GPIOH, GPIO_Pin_8, RCC_AHB1Periph_GPIOH}, {GPIOH, GPIO_Pin_9, RCC_AHB1Periph_GPIOH},
    {GPIOH, GPIO_Pin_10,RCC_AHB1Periph_GPIOH}, {GPIOH, GPIO_Pin_11,RCC_AHB1Periph_GPIOH},
    {GPIOH, GPIO_Pin_12,RCC_AHB1Periph_GPIOH}, {GPIOH, GPIO_Pin_13,RCC_AHB1Periph_GPIOH},
    {GPIOH, GPIO_Pin_14,RCC_AHB1Periph_GPIOH}, {GPIOH, GPIO_Pin_15,RCC_AHB1Periph_GPIOH},

    // --- GPIO I (128~143) ---
    {GPIOI, GPIO_Pin_0, RCC_AHB1Periph_GPIOI}, {GPIOI, GPIO_Pin_1, RCC_AHB1Periph_GPIOI},
    {GPIOI, GPIO_Pin_2, RCC_AHB1Periph_GPIOI}, {GPIOI, GPIO_Pin_3, RCC_AHB1Periph_GPIOI},
    {GPIOI, GPIO_Pin_4, RCC_AHB1Periph_GPIOI}, {GPIOI, GPIO_Pin_5, RCC_AHB1Periph_GPIOI},
    {GPIOI, GPIO_Pin_6, RCC_AHB1Periph_GPIOI}, {GPIOI, GPIO_Pin_7, RCC_AHB1Periph_GPIOI},
    {GPIOI, GPIO_Pin_8, RCC_AHB1Periph_GPIOI}, {GPIOI, GPIO_Pin_9, RCC_AHB1Periph_GPIOI},
    {GPIOI, GPIO_Pin_10,RCC_AHB1Periph_GPIOI}, {GPIOI, GPIO_Pin_11,RCC_AHB1Periph_GPIOI},
    {GPIOI, GPIO_Pin_12,RCC_AHB1Periph_GPIOI}, {GPIOI, GPIO_Pin_13,RCC_AHB1Periph_GPIOI},
    {GPIOI, GPIO_Pin_14,RCC_AHB1Periph_GPIOI}, {GPIOI, GPIO_Pin_15,RCC_AHB1Periph_GPIOI},
};

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      GPIO 初始化配置 (STM32F4 适配版)
// 参数说明      pin             GPIO 引脚枚举 (例如: A0, B5 等，对应 RUN_GPIO_enum)
// 参数说明      mode            引脚工作模式 (GPO:推挽输出, GPO_OD:开漏输出, GPI:浮空输入, GPI_PU:上拉输入, GPI_PD:下拉输入, AIN:模拟输入)
// 参数说明      default_level   默认输出电平 (1:高电平, 0:低电平)
// 返回参数      void
// 使用示例      RUN_gpio_init(A0, GPO, 1); // 将 A0 初始化为推挽输出模式，并默认输出高电平
// 备注信息      内部已自动处理 F407 特有的 AHB1 高速总线时钟使能，并将简化模式映射到了 F4 复杂的 OType/PuPd 寄存器组合中。
//               default_level 参数仅在配置为输出模式时才会生效。
//-------------------------------------------------------------------------------------------------------------------
void RUN_gpio_init(RUN_GPIO_enum pin, RUN_GPIO_Mode mode, uint8_t default_level)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    if (pin >= RUN_GPIO_MAX) return;

    // F4 使用 AHB1Periph 时钟开启函数
    RCC_AHB1PeriphClockCmd(gpio_cfg[pin].rcc, ENABLE);

    GPIO_InitStructure.GPIO_Pin = gpio_cfg[pin].pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // F4也可以选 GPIO_Speed_100MHz

    // 将你定义的模式映射到 F4 的复杂结构体
    switch (mode)
    {
        case GPO:    
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
            GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
            break;
        case GPO_OD: 
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
            GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
            break;
        case GPI: // 增加浮空输入对应的显式解析
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
            break;
        case GPI_PU: 
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
            break;
        case GPI_PD: 
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
            break;
        case AIN:    
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN; 
            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
            break;
        default:     
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
            break;
    }

    GPIO_Init(gpio_cfg[pin].port, &GPIO_InitStructure);

    if (mode == GPO || mode == GPO_OD)
    {
        RUN_gpio_set(pin, default_level);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      GPIO 输出电平设置
// 参数说明      pin             GPIO 引脚枚举
// 参数说明      level           目标输出电平 (1:高电平, 0:低电平)
// 返回参数      void
// 使用示例      RUN_gpio_set(A0, 0); // 设置 A0 引脚输出低电平
// 备注信息      调用此函数前，请确保该引脚已被 RUN_gpio_init 初始化为输出模式 (GPO 或 GPO_OD)。
//-------------------------------------------------------------------------------------------------------------------
void RUN_gpio_set(RUN_GPIO_enum pin, uint8_t level)
{
    if (pin >= RUN_GPIO_MAX) return;

    if (level)
        GPIO_SetBits(gpio_cfg[pin].port, gpio_cfg[pin].pin);
    else
        GPIO_ResetBits(gpio_cfg[pin].port, gpio_cfg[pin].pin);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      GPIO 输出电平翻转
// 参数说明      pin             GPIO 引脚枚举
// 返回参数      void
// 使用示例      RUN_gpio_toggle(A0); // 翻转 A0 引脚的当前电平 (常用于心跳灯或蜂鸣器控制)
// 备注信息      STM32F4 标准库原生提供了 ToggleBits 函数，直接操作 BSRR 寄存器，比 F1 系列的“先读后写”更加高效安全。
//-------------------------------------------------------------------------------------------------------------------
void RUN_gpio_toggle(RUN_GPIO_enum pin)
{
    if (pin >= RUN_GPIO_MAX) return;
    
    // F4 标准库直接提供了 Toggle 函数，更为简洁安全
    GPIO_ToggleBits(gpio_cfg[pin].port, gpio_cfg[pin].pin);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      GPIO 输入电平读取
// 参数说明      pin             GPIO 引脚枚举
// 返回参数      uint8_t         引脚当前的物理电平状态 (1:高电平, 0:低电平)
// 使用示例      if (RUN_gpio_get(A0) == 0) { /* 按键被按下 */ }
// 备注信息      主要用于读取按键状态或传感器(如红外循迹、光电开关)的数字信号。调用前需确保已配置为输入模式。
//-------------------------------------------------------------------------------------------------------------------
uint8_t RUN_gpio_get(RUN_GPIO_enum pin)
{
    if (pin >= RUN_GPIO_MAX) return 0;
    return GPIO_ReadInputDataBit(gpio_cfg[pin].port, gpio_cfg[pin].pin);
}
