#include "RUN_DAC.h"

// ==============================================================================
// 初始化函数
// ==============================================================================

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      DAC 模块初始化 (STM32F407 适配版)
// 参数说明      channel         DAC通道选择枚举 (RUN_DAC_CH1_PA4 或 RUN_DAC_CH2_PA5)
// 返回参数      void
// 使用示例      RUN_DAC_Init(RUN_DAC_CH1_PA4);
// 备注信息      内部已自动处理 F407 的 AHB1 时钟开启，并将引脚配置为纯模拟模式(AN)，无上下拉。
//               配置参数为：12位右对齐、开启输出缓冲、软件触发。
//-------------------------------------------------------------------------------------------------------------------
void RUN_DAC_Init(RUN_DAC_Channel_t channel)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    DAC_InitTypeDef  DAC_InitType;

    // 1. 开启时钟
    // F407 中，GPIOA 挂载在 AHB1 高速总线，DAC 挂载在 APB1 低速总线
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

    // 2. 配置引脚为模拟模式
    // STM32F407 使用 DAC 时，引脚必须设为 GPIO_Mode_AN，并且不能有任何上下拉
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    
    if (channel == RUN_DAC_CH1_PA4) {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
    } else if (channel == RUN_DAC_CH2_PA5) {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
    } else {
        return; // 非法通道
    }

    // 3. 配置 DAC 参数
    DAC_InitType.DAC_Trigger = DAC_Trigger_None;                      // 不使用外部硬件触发，使用软件写寄存器触发
    DAC_InitType.DAC_WaveGeneration = DAC_WaveGeneration_None;        // 不生成波形 (如噪声波/三角波)
    DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0; // 波形幅值(未使用，填默认)
    DAC_InitType.DAC_OutputBuffer = DAC_OutputBuffer_Enable;          // 开启输出缓冲 (增强驱动能力，但输出无法达到绝对的 0V，会有极小底噪)
    
    // 4. 初始化硬件
    if (channel == RUN_DAC_CH1_PA4) {
        DAC_Init(DAC_Channel_1, &DAC_InitType);
        DAC_Cmd(DAC_Channel_1, ENABLE);
        // 初始输出 0V
        DAC_SetChannel1Data(DAC_Align_12b_R, 0); 
    } else {
        DAC_Init(DAC_Channel_2, &DAC_InitType);
        DAC_Cmd(DAC_Channel_2, ENABLE);
        // 初始输出 0V
        DAC_SetChannel2Data(DAC_Align_12b_R, 0); 
    }
}

// ==============================================================================
// 数值控制函数
// ==============================================================================

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      按真实电压值设置 DAC 输出
// 参数说明      channel         DAC通道选择枚举
// 参数说明      vol             目标电压 (范围：0.0f ~ 3.3f，单位：伏特)
// 返回参数      void
// 使用示例      RUN_DAC_Set_Vol(RUN_DAC_CH1_PA4, 1.5f); // 在 PA4 输出 1.5V 真实电压
// 备注信息      底层自带防溢出限幅。得益于 F407 的硬件 FPU，浮点运算极快。
//               【注意】此函数的 3.3f 取决于你板子的 VDDA/Vref+ 引脚的实际供电电压。
//-------------------------------------------------------------------------------------------------------------------
void RUN_DAC_Set_Vol(RUN_DAC_Channel_t channel, float vol)
{
    uint16_t temp_val;
    
    // 软件限幅，防止计算溢出导致电压翻转
    if(vol > 3.3f) vol = 3.3f;
    if(vol < 0.0f) vol = 0.0f;
    
    // 线性变换：电压 -> 寄存器数值
    // 公式推导：
    // V_out = V_REF+ * (DOR / 4095)
    // DOR = (V_out / 3.3) * 4095
    temp_val = (uint16_t)(vol * 4095 / 3.3f);
    
    RUN_DAC_Set_Value(channel, temp_val);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      按寄存器底层数值设置 DAC 输出
// 参数说明      channel         DAC通道选择枚举
// 参数说明      val             12位 DAC 数值 (范围：0 ~ 4095)
// 返回参数      void
// 使用示例      RUN_DAC_Set_Value(RUN_DAC_CH1_PA4, 2048); // 输出约 1.65V (即 3.3V 的一半)
// 备注信息      直接操作数据寄存器 (DHR12R1 / DHR12R2)，执行效率最高，适合播放音频或生成高频信号源。
//-------------------------------------------------------------------------------------------------------------------
void RUN_DAC_Set_Value(RUN_DAC_Channel_t channel, uint16_t val)
{
    if(val > 4095) val = 4095; // 12位最大值为 4095，限幅防止位溢出

    if (channel == RUN_DAC_CH1_PA4) {
        DAC_SetChannel1Data(DAC_Align_12b_R, val);
    } else if (channel == RUN_DAC_CH2_PA5) {
        DAC_SetChannel2Data(DAC_Align_12b_R, val);
    }
}