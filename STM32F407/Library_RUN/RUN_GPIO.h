#ifndef _RUN_GPIO_H_
#define _RUN_GPIO_H_

#include "stm32f4xx.h" 

// ==========================================================
// 1. 统一枚举名称为 RUN_GPIO_enum (包含STM32F407所有可能的GPIOA~I)
// ==========================================================
typedef enum {
    // GPIO A
    A0 = 0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15,
    // GPIO B
    B0, B1, B2, B3, B4, B5, B6, B7, B8, B9, B10, B11, B12, B13, B14, B15,
    // GPIO C
    C0, C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, C11, C12, C13, C14, C15,
    // GPIO D
    D0, D1, D2, D3, D4, D5, D6, D7, D8, D9, D10, D11, D12, D13, D14, D15,
    // GPIO E
    E0, E1, E2, E3, E4, E5, E6, E7, E8, E9, E10, E11, E12, E13, E14, E15,
    // GPIO F
    F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15,
    // GPIO G
    G0, G1, G2, G3, G4, G5, G6, G7, G8, G9, G10, G11, G12, G13, G14, G15,
    // GPIO H
    H0, H1, H2, H3, H4, H5, H6, H7, H8, H9, H10, H11, H12, H13, H14, H15,
    // GPIO I
    I0, I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12, I13, I14, I15,

    RUN_GPIO_MAX
} RUN_GPIO_enum;

// ==========================================================
// 2. 将结构体定义移到 .h 文件 (供 Exti 调用)
// ==========================================================
typedef struct {
    GPIO_TypeDef* port; // 端口 (GPIOA)
    uint16_t      pin;  // 引脚 (GPIO_Pin_0)
    uint32_t      rcc;  // 时钟 (RCC_AHB1Periph_GPIOA)
} gpio_info_t;

// ==========================================================
// 3. 公开 gpio_cfg 数组 (供 Exti 调用)
// ==========================================================
extern const gpio_info_t gpio_cfg[]; 

// --- 模式枚举 (保留原有接口设计) ---
typedef enum {
    GPI,        // 浮空输入
    GPI_PU,     // 上拉输入
    GPI_PD,     // 下拉输入
    GPO,        // 推挽输出
    GPO_OD,     // 开漏输出
    AIN         // 模拟输入
} RUN_GPIO_Mode;

// --- 函数声明 ---
void    RUN_gpio_init(RUN_GPIO_enum pin, RUN_GPIO_Mode mode, uint8_t default_level);
void    RUN_gpio_set (RUN_GPIO_enum pin, uint8_t level);
void    RUN_gpio_toggle(RUN_GPIO_enum pin);
uint8_t RUN_gpio_get (RUN_GPIO_enum pin);

#endif
