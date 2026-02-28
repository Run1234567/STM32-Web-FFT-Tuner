#include "RUN_header_file.h"
#include "ADC_DMA.h"
#include <stdio.h>

// ====== 引入 DSP 库必要的头文件 ======
#include "arm_math.h"
#include "arm_const_structs.h"

// ====== FFT 计算专用的数组 ======
// 输入数组：FFT 要求输入复数，所以长度必须是 1024 的 2 倍。
// (偶数位存实部，奇数位存虚部)
float32_t FFT_InputBuf[FFT_LENGTH * 2]; 

// 输出数组：存放计算出来的各个频率的“高度”(幅值)，长度 1024 即可
float32_t FFT_OutputMag[FFT_LENGTH]; 

int main (void)
{ 
	RUN_delay_init(168); 
	RUN_gpio_init(B2,GPO,1);
    
    // 注意：如果你想让下面的 UART1_RxFlag 逻辑生效，这里的 0 可能要改成 1 (开启接收中断)
    // 测试 FFT 阶段建议先保持 0，防止接收中断干扰波形打印
	RUN_uart_init(UART1_TX_PA9_RX_PA10, 1152000, 0); 
	
    Audio_ADC_DMA_Init(); // 启动音频采集！
	RUN_timer_init(RUN_TIM4, 500);
    
    while (1) 
    {
		if (UART1_RxFlag == 1) 
        {
            // 收到串口1的数据包，转发到串口6
            RUN_uart_putstr(UART1_TX_PA9_RX_PA10, "UART1 Say: ");
            RUN_uart_putstr(UART1_TX_PA9_RX_PA10, UART1_RxPacket);
            RUN_uart_putstr(UART1_TX_PA9_RX_PA10, "\r\n");

            // 处理完毕，必须手动清零标志位，否则逻辑会一直触发
            UART1_RxFlag = 0; 
        }   
        
		if(FFT_Ready_Flag == 1)
        {
            FFT_Ready_Flag = 0; // 必须马上清零标志位
            
            // ========================================================
            // 【第 1 步】：准备数据 (消灭直流偏置 + 组装复数)
            // ========================================================
            for(int i = 0; i < FFT_LENGTH; i++)
            {
                // 减去 1500 左右的直流偏置，让波形在 0 轴上下震荡
                FFT_InputBuf[2 * i]     = (float32_t)FFT_Process_Ptr[i] - 1500.0f; // 实部存波形
                FFT_InputBuf[2 * i + 1] = 0.0f;                                    // 虚部存0
            }

            // ========================================================
            // 【第 2 步】：执行 FFT 极速运算！
            // ========================================================
            arm_cfft_f32(&arm_cfft_sR_f32_len1024, FFT_InputBuf, 0, 1);

            // ========================================================
            // 【第 3 步】：求取复数的模值 (计算各频率的真实高度)
            // ========================================================
            arm_cmplx_mag_f32(FFT_InputBuf, FFT_OutputMag, FFT_LENGTH);

            // ========================================================
            // 【第 4 步】：打印频谱数据发给电脑绘图
            // ========================================================
            // 根据奈奎斯特定理，只有前一半(512个点)是有用频率
            // 第 0 个点是 0Hz (直流分量)，直接忽略，从第 1 个点开始
						printf("START\n");
            for(int i = 0; i < FFT_LENGTH / 2; i++) 
            {
                printf("%d\n", (uint32_t)FFT_OutputMag[i]); 
            }
            
            // 稍微延时，防止串口疯狂发送导致助手卡死
            // 如果觉得波形刷新慢，可以把 100 减小到 50 甚至 20
            RUN_delay_ms(100); 
						 
        }
    }
}

void TIM4_Callback(void)
{
	RUN_gpio_toggle(B2);
}