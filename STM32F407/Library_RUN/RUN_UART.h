#ifndef _RUN_UART_H_
#define _RUN_UART_H_

#include "stm32f4xx.h" // 使用F4的头文件

// --- 1. 枚举定义 (涵盖STM32F407所有的串口引脚映射) ---
typedef enum {
    // UART1 (高速)
    UART1_TX_PA9_RX_PA10 = 0,
    UART1_TX_PB6_RX_PB7,

    // UART2
    UART2_TX_PA2_RX_PA3,
    UART2_TX_PD5_RX_PD6,

    // UART3
    UART3_TX_PB10_RX_PB11,
    UART3_TX_PC10_RX_PC11,
    UART3_TX_PD8_RX_PD9,

    // UART4
    UART4_TX_PA0_RX_PA1,
    UART4_TX_PC10_RX_PC11,

    // UART5
    UART5_TX_PC12_RX_PD2,

    // USART6 (高速, F4特有)
    UART6_TX_PC6_RX_PC7,
    UART6_TX_PG14_RX_PG9,

    UART_PIN_MAX
} UART_PIN_enum;

// --- 2. 硬件信息结构体 (适配 F4 AF复用机制) ---
typedef struct {
    USART_TypeDef* uart_base;   // 串口基地址
    uint32_t       uart_rcc;    // 串口时钟 (APB1或APB2)
    uint8_t        is_apb2;     // 时钟总线 1:APB2, 0:APB1

    GPIO_TypeDef* tx_port;      // TX 端口
    uint16_t       tx_pin;      // TX 引脚 (GPIO_Pin_x)
    uint8_t        tx_pin_src;  // TX 引脚源 (GPIO_PinSourcex, F4特有)
    uint32_t       tx_rcc;      // TX 时钟 (AHB1)

    GPIO_TypeDef* rx_port;      // RX 端口
    uint16_t       rx_pin;      // RX 引脚 (GPIO_Pin_x)
    uint8_t        rx_pin_src;  // RX 引脚源 (GPIO_PinSourcex, F4特有)
    uint32_t       rx_rcc;      // RX 时钟 (AHB1)

    uint8_t        af_mapping;  // AF 映射值 (如 GPIO_AF_USART1)
} uart_info_t;

extern const uart_info_t uart_cfg[UART_PIN_MAX];

// --- 3. 函数声明 ---

// 初始化串口
// enable_it: 1=开启接收中断(需要在stm32f4xx_it.c写处理函数), 0=仅轮询发送
void RUN_uart_init(UART_PIN_enum uart_pin, uint32_t baud_rate, uint8_t enable_it);

// 发送相关
void RUN_uart_putchar(UART_PIN_enum uart_pin, uint8_t dat);
void RUN_uart_putstr(UART_PIN_enum uart_pin, char *str);
void RUN_uart_putbuff(UART_PIN_enum uart_pin, uint8_t *buff, uint32_t len);
void RUN_uart_putint(UART_PIN_enum uart_pin, int32_t num);
void RUN_uart_putfloat(UART_PIN_enum uart_pin, double num, uint8_t decimals);
// 查询相关
uint8_t RUN_uart_getchar(UART_PIN_enum uart_pin);
uint8_t RUN_uart_query(UART_PIN_enum uart_pin);

#endif