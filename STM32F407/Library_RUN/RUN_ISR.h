#ifndef _RUN_ISR_H_
#define _RUN_ISR_H_

/* 定义最大接收字节数，防止溢出 */
#define MAX_RX_LEN  100  

/* --- 全局变量定义区域 --- */
// 串口1 变量
extern char UART1_RxPacket[MAX_RX_LEN];
extern uint8_t UART1_RxFlag;

// 串口2 变量
extern char UART2_RxPacket[MAX_RX_LEN];
extern uint8_t UART2_RxFlag;


// 串口3 变量
extern char UART3_RxPacket[MAX_RX_LEN];
extern uint8_t UART3_RxFlag;

// 串口4 变量
extern char UART4_RxPacket[MAX_RX_LEN];
extern uint8_t UART4_RxFlag;

// 串口5 变量
extern char UART5_RxPacket[MAX_RX_LEN];
extern uint8_t UART5_RxFlag;

#endif