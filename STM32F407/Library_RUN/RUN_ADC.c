#include "RUN_ADC.h"

// ==============================================================================
// 初始化函数
// ==============================================================================

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      ADC1 模块初始化 (STM32F407 适配版)
// 参数说明      void
// 返回参数      void
// 使用示例      RUN_ADC_Init(); // 必须在引脚初始化 (AIN 模式) 之后调用
// 备注信息      配置为：独立模式、12位分辨率、单次转换、软件触发。
//               【关键】F407 取消了 F103 的软件校准步骤；新增了 CommonInit 通用配置结构体。
//-------------------------------------------------------------------------------------------------------------------
void RUN_ADC_Init(void)
{
    ADC_InitTypeDef       ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;

    // 1. 开启 ADC1 外设时钟 (挂载在 APB2 上的时钟，F407 通常 APB2 为 84MHz)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    // 2. ADC 通用配置 (STM32F4 新增部分)
    // F407 要求统一设置 ADC 时钟分频和多重 ADC 模式
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;                      // 独立模式
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;                   // 时钟分频: APB2(84MHz)/4 = 21MHz (ADC最大允许36MHz)
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;       // 禁用 DMA 通用访问
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;  // 两个采样阶段之间的延迟
    ADC_CommonInit(&ADC_CommonInitStructure);

    // 3. ADC1 核心参数配置
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;                        // 12位分辨率 (F4特有选项)
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                                 // 禁用扫描模式 (单通道采集)
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                           // 禁用连续转换模式 (单次触发采集)
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;   // F407 改用边沿触发控制软件触发
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;         // (无外部边沿时此参数无效)
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                        // 数据右对齐
    ADC_InitStructure.ADC_NbrOfConversion = 1;                                    // 规则通道序列长度为 1
    ADC_Init(ADC1, &ADC_InitStructure);

    // 4. 开启 ADC 硬件
    ADC_Cmd(ADC1, ENABLE);

    // 【注】: STM32F407 硬件优化，去除了 F103 中的 ADC_ResetCalibration 和 ADC_StartCalibration 过程
}

// ==============================================================================
// 采集读取函数
// ==============================================================================

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      触发一次 ADC 转换并读取结果
// 参数说明      ch              ADC通道号枚举 (例如: RUN_ADC_CH0_PA0)
// 返回参数      uint16_t        12位 ADC 转换结果 (范围: 0 ~ 4095)
// 使用示例      uint16_t val = RUN_ADC_Get(RUN_ADC_CH0_PA0);
// 备注信息      轮询等待方式，采集耗时取决于采样周期和时钟频率。
// 注意：F407 必须配成 AIN，否则 ADC 读不出正常数据 RUN_gpio_init(A0, GPIO_MODE_AIN, GPIO_Speed_50MHz);
//-------------------------------------------------------------------------------------------------------------------
uint16_t RUN_ADC_Get(RUN_ADC_CH_enum ch)
{
    // 1. 配置规则组通道 (Rank 1)
    // 意思是：马上要转换的第一个通道就是传入的 ch。
    // F407 采样时间枚举改变，这里选择 84 个周期，兼顾速度与抗输入阻抗抖动
    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_84Cycles);

    // 2. 软件触发转换 (Software Start)
    // 相当于按下了 "开始" 按钮 (F407 的标准库函数名与 F103 略有不同)
    ADC_SoftwareStartConv(ADC1);

    // 3. 等待转换结束 (Polling EOC Flag)
    // 轮询 EOC (End Of Conversion) 标志位
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);

    // 4. 读取数据寄存器 (DR)
    // 读取操作会自动清除 EOC 标志位
    return ADC_GetConversionValue(ADC1);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      多次采样取平均值 (软件滤波)
// 参数说明      ch              ADC通道号枚举 (例如: RUN_ADC_CH0_PA0)
// 参数说明      times           采样次数 (推荐 10~50 之间，避免 uint32_t 溢出)
// 返回参数      uint16_t        平滑后的 12位 ADC 结果
// 使用示例   RUN_gpio_init(A0, AIN, GPIO_Speed_50MHz);   uint16_t val_avg = RUN_ADC_Get_Average(RUN_ADC_CH0_PA0, 20);
// 备注信息      有效过滤随机高频噪波干扰，提升数据读取稳定性。
// 注意：F407 必须配成 AIN，否则 ADC 读不出正常数据 RUN_gpio_init(A0, AIN, 0);
//-------------------------------------------------------------------------------------------------------------------
uint16_t RUN_ADC_Get_Average(RUN_ADC_CH_enum ch, uint8_t times)
{
    uint32_t temp_val = 0;
    uint8_t t;
    
    // 特判容错，防止除以 0 导致硬件 HardFault
    if (times == 0) return RUN_ADC_Get(ch);

    for(t = 0; t < times; t++)
    {
        temp_val += RUN_ADC_Get(ch);
    }
    
    return temp_val / times;
}