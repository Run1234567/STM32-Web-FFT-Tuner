#ifndef _RUN_EXTI_H_
#define _RUN_EXTI_H_

#include "stm32f4xx.h"
#include "RUN_Gpio.h" // 必须包含，才能找到 RUN_GPIO_enum 定义，比如 A0, B5 等

// ==============================================================================
// 外部中断引脚全集枚举 (严格适配 STM32F407)
// 支持从 GPIOA 到 GPIOI 的所有 144 个引脚
// 备注：枚举值直接等于 RUN_Gpio.h 中的引脚序号，方便底层数组直接取值
// ==============================================================================
typedef enum
{
    // --- Line 0 ---
    EXTI_Line0_PA0 = A0, EXTI_Line0_PB0 = B0, EXTI_Line0_PC0 = C0, EXTI_Line0_PD0 = D0,
    EXTI_Line0_PE0 = E0, EXTI_Line0_PF0 = F0, EXTI_Line0_PG0 = G0, EXTI_Line0_PH0 = H0, EXTI_Line0_PI0 = I0,

    // --- Line 1 ---
    EXTI_Line1_PA1 = A1, EXTI_Line1_PB1 = B1, EXTI_Line1_PC1 = C1, EXTI_Line1_PD1 = D1,
    EXTI_Line1_PE1 = E1, EXTI_Line1_PF1 = F1, EXTI_Line1_PG1 = G1, EXTI_Line1_PH1 = H1, EXTI_Line1_PI1 = I1,

    // --- Line 2 ---
    EXTI_Line2_PA2 = A2, EXTI_Line2_PB2 = B2, EXTI_Line2_PC2 = C2, EXTI_Line2_PD2 = D2,
    EXTI_Line2_PE2 = E2, EXTI_Line2_PF2 = F2, EXTI_Line2_PG2 = G2, EXTI_Line2_PH2 = H2, EXTI_Line2_PI2 = I2,

    // --- Line 3 ---
    EXTI_Line3_PA3 = A3, EXTI_Line3_PB3 = B3, EXTI_Line3_PC3 = C3, EXTI_Line3_PD3 = D3,
    EXTI_Line3_PE3 = E3, EXTI_Line3_PF3 = F3, EXTI_Line3_PG3 = G3, EXTI_Line3_PH3 = H3, EXTI_Line3_PI3 = I3,

    // --- Line 4 ---
    EXTI_Line4_PA4 = A4, EXTI_Line4_PB4 = B4, EXTI_Line4_PC4 = C4, EXTI_Line4_PD4 = D4,
    EXTI_Line4_PE4 = E4, EXTI_Line4_PF4 = F4, EXTI_Line4_PG4 = G4, EXTI_Line4_PH4 = H4, EXTI_Line4_PI4 = I4,

    // --- Line 5 ---
    EXTI_Line5_PA5 = A5, EXTI_Line5_PB5 = B5, EXTI_Line5_PC5 = C5, EXTI_Line5_PD5 = D5,
    EXTI_Line5_PE5 = E5, EXTI_Line5_PF5 = F5, EXTI_Line5_PG5 = G5, EXTI_Line5_PH5 = H5, EXTI_Line5_PI5 = I5,

    // --- Line 6 ---
    EXTI_Line6_PA6 = A6, EXTI_Line6_PB6 = B6, EXTI_Line6_PC6 = C6, EXTI_Line6_PD6 = D6,
    EXTI_Line6_PE6 = E6, EXTI_Line6_PF6 = F6, EXTI_Line6_PG6 = G6, EXTI_Line6_PH6 = H6, EXTI_Line6_PI6 = I6,

    // --- Line 7 ---
    EXTI_Line7_PA7 = A7, EXTI_Line7_PB7 = B7, EXTI_Line7_PC7 = C7, EXTI_Line7_PD7 = D7,
    EXTI_Line7_PE7 = E7, EXTI_Line7_PF7 = F7, EXTI_Line7_PG7 = G7, EXTI_Line7_PH7 = H7, EXTI_Line7_PI7 = I7,

    // --- Line 8 ---
    EXTI_Line8_PA8 = A8, EXTI_Line8_PB8 = B8, EXTI_Line8_PC8 = C8, EXTI_Line8_PD8 = D8,
    EXTI_Line8_PE8 = E8, EXTI_Line8_PF8 = F8, EXTI_Line8_PG8 = G8, EXTI_Line8_PH8 = H8, EXTI_Line8_PI8 = I8,

    // --- Line 9 ---
    EXTI_Line9_PA9 = A9, EXTI_Line9_PB9 = B9, EXTI_Line9_PC9 = C9, EXTI_Line9_PD9 = D9,
    EXTI_Line9_PE9 = E9, EXTI_Line9_PF9 = F9, EXTI_Line9_PG9 = G9, EXTI_Line9_PH9 = H9, EXTI_Line9_PI9 = I9,

    // --- Line 10 ---
    EXTI_Line10_PA10 = A10, EXTI_Line10_PB10 = B10, EXTI_Line10_PC10 = C10, EXTI_Line10_PD10 = D10,
    EXTI_Line10_PE10 = E10, EXTI_Line10_PF10 = F10, EXTI_Line10_PG10 = G10, EXTI_Line10_PH10 = H10, EXTI_Line10_PI10 = I10,

    // --- Line 11 ---
    EXTI_Line11_PA11 = A11, EXTI_Line11_PB11 = B11, EXTI_Line11_PC11 = C11, EXTI_Line11_PD11 = D11,
    EXTI_Line11_PE11 = E11, EXTI_Line11_PF11 = F11, EXTI_Line11_PG11 = G11, EXTI_Line11_PH11 = H11, EXTI_Line11_PI11 = I11,

    // --- Line 12 ---
    EXTI_Line12_PA12 = A12, EXTI_Line12_PB12 = B12, EXTI_Line12_PC12 = C12, EXTI_Line12_PD12 = D12,
    EXTI_Line12_PE12 = E12, EXTI_Line12_PF12 = F12, EXTI_Line12_PG12 = G12, EXTI_Line12_PH12 = H12, EXTI_Line12_PI12 = I12,

    // --- Line 13 ---
    EXTI_Line13_PA13 = A13, EXTI_Line13_PB13 = B13, EXTI_Line13_PC13 = C13, EXTI_Line13_PD13 = D13,
    EXTI_Line13_PE13 = E13, EXTI_Line13_PF13 = F13, EXTI_Line13_PG13 = G13, EXTI_Line13_PH13 = H13, EXTI_Line13_PI13 = I13,

    // --- Line 14 ---
    EXTI_Line14_PA14 = A14, EXTI_Line14_PB14 = B14, EXTI_Line14_PC14 = C14, EXTI_Line14_PD14 = D14,
    EXTI_Line14_PE14 = E14, EXTI_Line14_PF14 = F14, EXTI_Line14_PG14 = G14, EXTI_Line14_PH14 = H14, EXTI_Line14_PI14 = I14,

    // --- Line 15 ---
    EXTI_Line15_PA15 = A15, EXTI_Line15_PB15 = B15, EXTI_Line15_PC15 = C15, EXTI_Line15_PD15 = D15,
    EXTI_Line15_PE15 = E15, EXTI_Line15_PF15 = F15, EXTI_Line15_PG15 = G15, EXTI_Line15_PH15 = H15, EXTI_Line15_PI15 = I15,

    EXTI_MAX = 0xFFFF
} RUN_EXTI_enum;

// 回调函数类型定义
typedef void (*ExtiCallback_t)(void);

// ==============================================================================
// 函数声明
// ==============================================================================
void RUN_exti_init(RUN_EXTI_enum line_pin, RUN_GPIO_Mode gpio_mode, EXTITrigger_TypeDef trigger, ExtiCallback_t callback);
void RUN_exti_cmd(RUN_EXTI_enum line_pin, FunctionalState state);
void RUN_exti_sw_trigger(RUN_EXTI_enum line_pin);

#endif