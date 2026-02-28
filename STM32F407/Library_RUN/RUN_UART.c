#include "RUN_header_file.h"

// ===============================================================================
// 核心配置表 (Lookup Table) - F407 适配版
// ===============================================================================
const uart_info_t uart_cfg[UART_PIN_MAX] = {
    // --- UART1 (挂载在 APB2, 映射为 AF7) ---
    {USART1, RCC_APB2Periph_USART1, 1, GPIOA, GPIO_Pin_9, GPIO_PinSource9, RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_10, GPIO_PinSource10, RCC_AHB1Periph_GPIOA, GPIO_AF_USART1}, 
    {USART1, RCC_APB2Periph_USART1, 1, GPIOB, GPIO_Pin_6, GPIO_PinSource6, RCC_AHB1Periph_GPIOB, GPIOB, GPIO_Pin_7,  GPIO_PinSource7,  RCC_AHB1Periph_GPIOB, GPIO_AF_USART1}, 
    
    // --- UART2 (挂载在 APB1, 映射为 AF7) ---
    {USART2, RCC_APB1Periph_USART2, 0, GPIOA, GPIO_Pin_2, GPIO_PinSource2, RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_3,  GPIO_PinSource3,  RCC_AHB1Periph_GPIOA, GPIO_AF_USART2},
    {USART2, RCC_APB1Periph_USART2, 0, GPIOD, GPIO_Pin_5, GPIO_PinSource5, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_6,  GPIO_PinSource6,  RCC_AHB1Periph_GPIOD, GPIO_AF_USART2},
    
    // --- UART3 (挂载在 APB1, 映射为 AF7) ---
    {USART3, RCC_APB1Periph_USART3, 0, GPIOB, GPIO_Pin_10,GPIO_PinSource10,RCC_AHB1Periph_GPIOB, GPIOB, GPIO_Pin_11, GPIO_PinSource11, RCC_AHB1Periph_GPIOB, GPIO_AF_USART3},
    {USART3, RCC_APB1Periph_USART3, 0, GPIOC, GPIO_Pin_10,GPIO_PinSource10,RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_11, GPIO_PinSource11, RCC_AHB1Periph_GPIOC, GPIO_AF_USART3},
    {USART3, RCC_APB1Periph_USART3, 0, GPIOD, GPIO_Pin_8, GPIO_PinSource8, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_9,  GPIO_PinSource9,  RCC_AHB1Periph_GPIOD, GPIO_AF_USART3},
    
    // --- UART4 (挂载在 APB1, 映射为 AF8) ---
    {UART4,  RCC_APB1Periph_UART4,  0, GPIOA, GPIO_Pin_0, GPIO_PinSource0, RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_1,  GPIO_PinSource1,  RCC_AHB1Periph_GPIOA, GPIO_AF_UART4},
    {UART4,  RCC_APB1Periph_UART4,  0, GPIOC, GPIO_Pin_10,GPIO_PinSource10,RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_11, GPIO_PinSource11, RCC_AHB1Periph_GPIOC, GPIO_AF_UART4},
    
    // --- UART5 (挂载在 APB1, 映射为 AF8) ---
    {UART5,  RCC_APB1Periph_UART5,  0, GPIOC, GPIO_Pin_12,GPIO_PinSource12,RCC_AHB1Periph_GPIOC, GPIOD, GPIO_Pin_2,  GPIO_PinSource2,  RCC_AHB1Periph_GPIOD, GPIO_AF_UART5},

    // --- USART6 (挂载在 APB2, 映射为 AF8) ---
    {USART6, RCC_APB2Periph_USART6, 1, GPIOC, GPIO_Pin_6, GPIO_PinSource6, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_7,  GPIO_PinSource7,  RCC_AHB1Periph_GPIOC, GPIO_AF_USART6},
    {USART6, RCC_APB2Periph_USART6, 1, GPIOG, GPIO_Pin_14,GPIO_PinSource14,RCC_AHB1Periph_GPIOG, GPIOG, GPIO_Pin_9,  GPIO_PinSource9,  RCC_AHB1Periph_GPIOG, GPIO_AF_USART6}
};

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      获取串口对应的 NVIC 中断通道编号 (静态内部函数)
// 参数说明      uart_base       串口硬件基地址 (如 USART1, UART4)
// 返回参数      uint8_t         对应的 IRQn_Type 中断号
// 备注信息      仅供 RUN_uart_init 内部调用，用于动态分配 NVIC 中断通道。
//-------------------------------------------------------------------------------------------------------------------
static uint8_t get_uart_irqn(USART_TypeDef* uart_base)
{
    if (uart_base == USART1) return USART1_IRQn;
    if (uart_base == USART2) return USART2_IRQn;
    if (uart_base == USART3) return USART3_IRQn;
    if (uart_base == UART4)  return UART4_IRQn;
    if (uart_base == UART5)  return UART5_IRQn;
    if (uart_base == USART6) return USART6_IRQn; // 增加 F4 的 USART6
    return 0; 
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      串口初始化配置 (适配 STM32F407)
// 参数说明      uart_pin        串口引脚方案枚举 (如 UART1_TX_PA9_RX_PA10)
// 参数说明      baud_rate       波特率 (如 115200, 9600)
// 参数说明      enable_it       是否开启接收中断 (1: 开启, 0: 关闭)
// 返回参数      void
// 使用示例      RUN_uart_init(UART1_TX_PA9_RX_PA10, 115200, 1);
// 备注信息      底层自动处理 F4 的 AHB1/APB1/APB2 时钟挂载，以及 GPIO_PinAFConfig 复用映射。
//-------------------------------------------------------------------------------------------------------------------
void RUN_uart_init(UART_PIN_enum uart_pin, uint32_t baud_rate, uint8_t enable_it)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    if (uart_pin >= UART_PIN_MAX) return;

    // 1. 开启 GPIO 时钟 (F4 是 AHB1)
    RCC_AHB1PeriphClockCmd(uart_cfg[uart_pin].tx_rcc | uart_cfg[uart_pin].rx_rcc, ENABLE);

    // 2. 开启 USART/UART 外设时钟
    if (uart_cfg[uart_pin].is_apb2) 
        RCC_APB2PeriphClockCmd(uart_cfg[uart_pin].uart_rcc, ENABLE);
    else                            
        RCC_APB1PeriphClockCmd(uart_cfg[uart_pin].uart_rcc, ENABLE);

    // 3. F4引脚复用映射 (替代了F1的Remap)
    GPIO_PinAFConfig(uart_cfg[uart_pin].tx_port, uart_cfg[uart_pin].tx_pin_src, uart_cfg[uart_pin].af_mapping);
    GPIO_PinAFConfig(uart_cfg[uart_pin].rx_port, uart_cfg[uart_pin].rx_pin_src, uart_cfg[uart_pin].af_mapping);

    // 4. GPIO 配置 (F4 的串口引脚必须全都配置为复用模式 AF)
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;      // 复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // 速度
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     // 推挽输出
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;      // 上拉(重要：稳定空闲电平)

    // 配置 TX
    GPIO_InitStructure.GPIO_Pin = uart_cfg[uart_pin].tx_pin;
    GPIO_Init(uart_cfg[uart_pin].tx_port, &GPIO_InitStructure);

    // 配置 RX (参数相同，改变引脚和端口即可)
    GPIO_InitStructure.GPIO_Pin = uart_cfg[uart_pin].rx_pin;
    GPIO_Init(uart_cfg[uart_pin].rx_port, &GPIO_InitStructure);

    // 5. USART 参数配置
    USART_InitStructure.USART_BaudRate = baud_rate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(uart_cfg[uart_pin].uart_base, &USART_InitStructure);

    // 6. 中断配置
    if (enable_it)
    {
        NVIC_InitStructure.NVIC_IRQChannel = get_uart_irqn(uart_cfg[uart_pin].uart_base);
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
        
        USART_ITConfig(uart_cfg[uart_pin].uart_base, USART_IT_RXNE, ENABLE);
    }
    else
    {
        USART_ITConfig(uart_cfg[uart_pin].uart_base, USART_IT_RXNE, DISABLE);
    }

    // 7. 使能串口
    USART_Cmd(uart_cfg[uart_pin].uart_base, ENABLE);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      串口发送单字节数据 (轮询阻塞方式)
// 参数说明      uart_pin        串口引脚方案枚举
// 参数说明      dat             要发送的 8 位数据
// 返回参数      void
// 使用示例      RUN_uart_putchar(UART1_TX_PA9_RX_PA10, 'A');
//-------------------------------------------------------------------------------------------------------------------
void RUN_uart_putchar(UART_PIN_enum uart_pin, uint8_t dat) {
    if (uart_pin >= UART_PIN_MAX) return;
    while (USART_GetFlagStatus(uart_cfg[uart_pin].uart_base, USART_FLAG_TXE) == RESET);
    USART_SendData(uart_cfg[uart_pin].uart_base, dat);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介      串口发送字符串
// 参数说明      uart_pin        串口引脚方案枚举
// 参数说明      str             以 '\0' 结尾的字符串指针
// 返回参数      void
// 使用示例      RUN_uart_putstr(UART1_TX_PA9_RX_PA10, "Hello World!\r\n");
//-------------------------------------------------------------------------------------------------------------------
void RUN_uart_putstr(UART_PIN_enum uart_pin, char *str) {
    if (uart_pin >= UART_PIN_MAX) return;
    while (*str) RUN_uart_putchar(uart_pin, *str++);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介      串口发送指定长度的数据帧 (Buffer)
// 参数说明      uart_pin        串口引脚方案枚举
// 参数说明      buff            数据缓冲区指针
// 参数说明      len             要发送的字节长度
// 返回参数      void
// 使用示例      RUN_uart_putbuff(UART1_TX_PA9_RX_PA10, data_array, 10);
// 备注信息      适用于发送包含 0x00 的十六进制数据包。
//-------------------------------------------------------------------------------------------------------------------
void RUN_uart_putbuff(UART_PIN_enum uart_pin, uint8_t *buff, uint32_t len) {
    if (uart_pin >= UART_PIN_MAX) return;
    for(uint32_t i = 0; i < len; i++) RUN_uart_putchar(uart_pin, buff[i]);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      内部静态计算次方函数 (参考江协科技底层逻辑)
// 参数说明      X               底数
// 参数说明      Y               指数
// 返回参数      uint32_t        X 的 Y 次方结果
// 备注信息      仅供内部数字提取逻辑使用，不对外暴露。
//-------------------------------------------------------------------------------------------------------------------
static uint32_t RUN_uart_pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while (Y--) 
    {
        Result *= X;
    }
    return Result;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      串口发送十进制整型数字 (纯数学计算版)
// 参数说明      uart_pin        串口引脚方案枚举
// 参数说明      num             要发送的整型数据 (int32_t，支持正负数)
// 返回参数      void
// 使用示例      RUN_uart_putint(UART1_TX_PA9_RX_PA10, -2048);
// 备注信息      抛弃 sprintf，采用江协科技的除余取整法，极大节省单片机 Flash 资源。支持自动计算长度。
//-------------------------------------------------------------------------------------------------------------------
void RUN_uart_putint(UART_PIN_enum uart_pin, int32_t num)
{
    if (uart_pin >= UART_PIN_MAX) return;

    // 处理负数
    uint32_t temp_num;
    if (num < 0) 
    {
        RUN_uart_putchar(uart_pin, '-');
        temp_num = -num;
    } 
    else 
    {
        temp_num = num;
    }

    // 特判 0 的情况
    if (temp_num == 0) 
    {
        RUN_uart_putchar(uart_pin, '0');
        return;
    }

    // 计算数字的实际位数
    uint32_t calc_temp = temp_num;
    uint8_t len = 0;
    while (calc_temp > 0) 
    {
        len++;
        calc_temp /= 10;
    }

    // 从高位到低位依次提取并发送 (江科大经典算法)
    for (uint8_t i = 0; i < len; i++) 
    {
        RUN_uart_putchar(uart_pin, (temp_num / RUN_uart_pow(10, len - i - 1)) % 10 + '0');
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      串口发送浮点型数据 (纯数学计算版)
// 参数说明      uart_pin        串口引脚方案枚举
// 参数说明      num             要发送的浮点数据
// 参数说明      decimals        需要保留的小数位数
// 返回参数      void
// 使用示例      RUN_uart_putfloat(UART1_TX_PA9_RX_PA10, 3.14159, 2); // 输出 "3.14"
// 备注信息      底层通过分离整数和小数部分进行纯数学转换，无需引入复杂的 MicroLIB。内置四舍五入。
//-------------------------------------------------------------------------------------------------------------------
void RUN_uart_putfloat(UART_PIN_enum uart_pin, double num, uint8_t decimals)
{
    if (uart_pin >= UART_PIN_MAX) return;

    // 处理负数
    if (num < 0) 
    {
        RUN_uart_putchar(uart_pin, '-');
        num = -num;
    }

    // 1. 提取并发送整数部分
    uint32_t int_part = (uint32_t)num;
    RUN_uart_putint(uart_pin, int_part); // 复用上面的整型发送函数

    // 如果不需要小数位，直接结束
    if (decimals == 0) return;

    // 2. 发送小数点
    RUN_uart_putchar(uart_pin, '.');

    // 3. 提取小数部分 (放大 decimals 倍，并加上 0.5 实现四舍五入)
    double frac_part = num - int_part;
    uint32_t frac_int = (uint32_t)(frac_part * RUN_uart_pow(10, decimals) + 0.5);

    // 4. 发送小数部分 (定长发送，保留前导 0，例如 0.05 不能变成 0.5)
    for (uint8_t i = 0; i < decimals; i++) 
    {
        RUN_uart_putchar(uart_pin, (frac_int / RUN_uart_pow(10, decimals - i - 1)) % 10 + '0');
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      串口接收单字节数据 (轮询阻塞方式)
// 参数说明      uart_pin        串口引脚方案枚举
// 返回参数      uint8_t         接收到的 8 位数据
// 备注信息      如果开启了接收中断，通常不需要调用此函数，直接在中断回调中处理即可。
//-------------------------------------------------------------------------------------------------------------------
uint8_t RUN_uart_getchar(UART_PIN_enum uart_pin) {
    if (uart_pin >= UART_PIN_MAX) return 0;
    while (USART_GetFlagStatus(uart_cfg[uart_pin].uart_base, USART_FLAG_RXNE) == RESET);
    return (uint8_t)USART_ReceiveData(uart_cfg[uart_pin].uart_base);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介      查询串口是否收到新数据
// 参数说明      uart_pin        串口引脚方案枚举
// 返回参数      uint8_t         状态 (1: 有新数据, 0: 暂无数据)
// 备注信息      读取 RXNE 标志位状态。
//-------------------------------------------------------------------------------------------------------------------
uint8_t RUN_uart_query(UART_PIN_enum uart_pin) {
    if (uart_pin >= UART_PIN_MAX) return 0;
    return (USART_GetFlagStatus(uart_cfg[uart_pin].uart_base, USART_FLAG_RXNE) != RESET);
}

// -----------------------------------------------------------
// Printf 重定向
// -----------------------------------------------------------
#if 1
#pragma import(__use_no_semihosting)
struct __FILE { int handle; };
FILE __stdout;
void _sys_exit(int x) { x = x; }

int fputc(int ch, FILE *f) {
    RUN_uart_putchar((UART_PIN_enum)0, (uint8_t)ch); // 默认映射到第0个枚举(UART1 PA9/10)
    return ch;
}
#endif