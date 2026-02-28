#include "RUN_header_file.h"

// ===============================================================================
// 全局变量定义 (1-6)
// ===============================================================================
char UART1_RxPacket[MAX_RX_LEN]; uint8_t UART1_RxFlag = 0;
char UART2_RxPacket[MAX_RX_LEN]; uint8_t UART2_RxFlag = 0;
char UART3_RxPacket[MAX_RX_LEN]; uint8_t UART3_RxFlag = 0;
char UART4_RxPacket[MAX_RX_LEN]; uint8_t UART4_RxFlag = 0;
char UART5_RxPacket[MAX_RX_LEN]; uint8_t UART5_RxFlag = 0;
char UART6_RxPacket[MAX_RX_LEN]; uint8_t UART6_RxFlag = 0;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      串口 1-6 数据包解析状态机 (内部函数)
// 参数说明      data            中断接收到的单字节数据
// 返回参数      void
// 备注信息      数据包格式约定为：以 '@' 开头，以 '\r\n' 结尾。解析成功后对应的 RxFlag 会被置 1。
//-------------------------------------------------------------------------------------------------------------------

// --- UART1 解析 ---
void uart1_rx_interrupt(uint8_t data)
{
    static uint8_t RxState = 0;
    static uint8_t pRxIndex = 0;
    if (RxState == 0) {
        if (data == '@' && UART1_RxFlag == 0) {
            RxState = 1; pRxIndex = 0;
            memset(UART1_RxPacket, 0, MAX_RX_LEN);
        }
    } else if (RxState == 1) {
        if (data == '\r') RxState = 2;
        else {
            if (pRxIndex < MAX_RX_LEN - 1) UART1_RxPacket[pRxIndex++] = data;
        }
    } else if (RxState == 2) {
        if (data == '\n') {
            RxState = 0; UART1_RxPacket[pRxIndex] = '\0';
            UART1_RxFlag = 1;
        } else RxState = 0;
    }
}

// --- UART2 解析 ---
void uart2_rx_interrupt(uint8_t data)
{
    static uint8_t RxState = 0;
    static uint8_t pRxIndex = 0;
    if (RxState == 0) {
        if (data == '@' && UART2_RxFlag == 0) {
            RxState = 1; pRxIndex = 0;
            memset(UART2_RxPacket, 0, MAX_RX_LEN);
        }
    } else if (RxState == 1) {
        if (data == '\r') RxState = 2;
        else {
            if (pRxIndex < MAX_RX_LEN - 1) UART2_RxPacket[pRxIndex++] = data;
        }
    } else if (RxState == 2) {
        if (data == '\n') {
            RxState = 0; UART2_RxPacket[pRxIndex] = '\0';
            UART2_RxFlag = 1;
        } else RxState = 0;
    }
}

// --- UART3 解析 ---
void uart3_rx_interrupt(uint8_t data)
{
    static uint8_t RxState = 0;
    static uint8_t pRxIndex = 0;
    if (RxState == 0) {
        if (data == '@' && UART3_RxFlag == 0) {
            RxState = 1; pRxIndex = 0;
            memset(UART3_RxPacket, 0, MAX_RX_LEN);
        }
    } else if (RxState == 1) {
        if (data == '\r') RxState = 2;
        else {
            if (pRxIndex < MAX_RX_LEN - 1) UART3_RxPacket[pRxIndex++] = data;
        }
    } else if (RxState == 2) {
        if (data == '\n') {
            RxState = 0; UART3_RxPacket[pRxIndex] = '\0';
            UART3_RxFlag = 1;
        } else RxState = 0;
    }
}

// --- UART4 解析 ---
void uart4_rx_interrupt(uint8_t data)
{
    static uint8_t RxState = 0;
    static uint8_t pRxIndex = 0;
    if (RxState == 0) {
        if (data == '@' && UART4_RxFlag == 0) {
            RxState = 1; pRxIndex = 0;
            memset(UART4_RxPacket, 0, MAX_RX_LEN);
        }
    } else if (RxState == 1) {
        if (data == '\r') RxState = 2;
        else {
            if (pRxIndex < MAX_RX_LEN - 1) UART4_RxPacket[pRxIndex++] = data;
        }
    } else if (RxState == 2) {
        if (data == '\n') {
            RxState = 0; UART4_RxPacket[pRxIndex] = '\0';
            UART4_RxFlag = 1;
        } else RxState = 0;
    }
}

// --- UART5 解析 ---
void uart5_rx_interrupt(uint8_t data)
{
    static uint8_t RxState = 0;
    static uint8_t pRxIndex = 0;
    if (RxState == 0) {
        if (data == '@' && UART5_RxFlag == 0) {
            RxState = 1; pRxIndex = 0;
            memset(UART5_RxPacket, 0, MAX_RX_LEN);
        }
    } else if (RxState == 1) {
        if (data == '\r') RxState = 2;
        else {
            if (pRxIndex < MAX_RX_LEN - 1) UART5_RxPacket[pRxIndex++] = data;
        }
    } else if (RxState == 2) {
        if (data == '\n') {
            RxState = 0; UART5_RxPacket[pRxIndex] = '\0';
            UART5_RxFlag = 1;
        } else RxState = 0;
    }
}

// --- UART6 解析 ---
void uart6_rx_interrupt(uint8_t data)
{
    static uint8_t RxState = 0;
    static uint8_t pRxIndex = 0;
    if (RxState == 0) {
        if (data == '@' && UART6_RxFlag == 0) {
            RxState = 1; pRxIndex = 0;
            memset(UART6_RxPacket, 0, MAX_RX_LEN);
        }
    } else if (RxState == 1) {
        if (data == '\r') RxState = 2;
        else {
            if (pRxIndex < MAX_RX_LEN - 1) UART6_RxPacket[pRxIndex++] = data;
        }
    } else if (RxState == 2) {
        if (data == '\n') {
            RxState = 0; UART6_RxPacket[pRxIndex] = '\0';
            UART6_RxFlag = 1;
        } else RxState = 0;
    }
}

// ==========================================================
// 中断服务函数 (IRQ Handlers)
// ==========================================================

void USART1_IRQHandler(void) {
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        uart1_rx_interrupt((uint8_t)USART_ReceiveData(USART1));
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}

void USART2_IRQHandler(void) {
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        uart2_rx_interrupt((uint8_t)USART_ReceiveData(USART2));
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}

void USART3_IRQHandler(void) {
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
        uart3_rx_interrupt((uint8_t)USART_ReceiveData(USART3));
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }
}

void UART4_IRQHandler(void) {
    if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET) {
        uart4_rx_interrupt((uint8_t)USART_ReceiveData(UART4));
        USART_ClearITPendingBit(UART4, USART_IT_RXNE);
    }
}

void UART5_IRQHandler(void) {
    if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET) {
        uart5_rx_interrupt((uint8_t)USART_ReceiveData(UART5));
        USART_ClearITPendingBit(UART5, USART_IT_RXNE);
    }
}

void USART6_IRQHandler(void) {
    if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET) {
        uart6_rx_interrupt((uint8_t)USART_ReceiveData(USART6));
        USART_ClearITPendingBit(USART6, USART_IT_RXNE);
    }
}


__weak void TIM1_UP_Callback(void) {}
__weak void TIM2_Callback(void) {}
__weak void TIM3_Callback(void) {}
__weak void TIM4_Callback(void) {}
__weak void TIM5_Callback(void) {}
__weak void TIM6_Callback(void) {}
__weak void TIM7_Callback(void) {}
__weak void TIM8_UP_Callback(void) {}
__weak void TIM9_Callback(void) {}
__weak void TIM10_Callback(void) {}
__weak void TIM11_Callback(void) {}
__weak void TIM12_Callback(void) {}
__weak void TIM13_Callback(void) {}
__weak void TIM14_Callback(void) {}

// ==========================================================
// 硬件中断服务函数 (IRQ Handlers) - 适配 F407 映射
// ==========================================================

// --- TIM1 和 TIM10 共用中断线 ---
void TIM1_UP_TIM10_IRQHandler(void) {
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
        TIM1_UP_Callback();
    }
    if (TIM_GetITStatus(TIM10, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM10, TIM_IT_Update);
        TIM10_Callback();
    }
}

// --- TIM8 和 TIM13 共用中断线 ---
void TIM8_UP_TIM13_IRQHandler(void) {
    if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
        TIM8_UP_Callback();
    }
    if (TIM_GetITStatus(TIM13, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM13, TIM_IT_Update);
        TIM13_Callback();
    }
}

// --- TIM9 处理 ---
void TIM1_BRK_TIM9_IRQHandler(void) {
    if (TIM_GetITStatus(TIM9, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM9, TIM_IT_Update);
        TIM9_Callback();
    }
}

// --- TIM11 处理 ---
void TIM1_TRG_COM_TIM11_IRQHandler(void) {
    if (TIM_GetITStatus(TIM11, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM11, TIM_IT_Update);
        TIM11_Callback();
    }
}

// --- TIM12 处理 ---
void TIM8_BRK_TIM12_IRQHandler(void) {
    if (TIM_GetITStatus(TIM12, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM12, TIM_IT_Update);
        TIM12_Callback();
    }
}

// --- TIM14 处理 ---
void TIM8_TRG_COM_TIM14_IRQHandler(void) {
    if (TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM14, TIM_IT_Update);
        TIM14_Callback();
    }
}

// --- 独立中断线的定时器 ---

void TIM2_IRQHandler(void) {
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        TIM2_Callback();
    }
}

void TIM3_IRQHandler(void) {
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        TIM3_Callback();
    }
}

void TIM4_IRQHandler(void) {
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
        TIM4_Callback();
    }
}

void TIM5_IRQHandler(void) {
    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
        TIM5_Callback();
    }
}

// TIM6 采用核心逻辑写法以增强兼容性
void RUN_TIM6_Handler_Core(void) {
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
        TIM6_Callback();
    }
}
void TIM6_IRQHandler(void) { RUN_TIM6_Handler_Core(); }
void TIM6_DAC_IRQHandler(void) { RUN_TIM6_Handler_Core(); }

void TIM7_IRQHandler(void) {
    if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
        TIM7_Callback();
    }
}

