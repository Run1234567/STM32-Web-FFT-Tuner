#include "RUN_SoftI2C.h"

// ------------------------------------------------------------------------------
// 内部私有宏定义：控制电平翻转
// 使用 RUN_Gpio 库提供的 set/get 函数，适配 F407 寄存器操作
// ------------------------------------------------------------------------------
#define SCL_H(bus)      RUN_gpio_set((bus)->SCL_Pin, 1)
#define SCL_L(bus)      RUN_gpio_set((bus)->SCL_Pin, 0)
#define SDA_H(bus)      RUN_gpio_set((bus)->SDA_Pin, 1)
#define SDA_L(bus)      RUN_gpio_set((bus)->SDA_Pin, 0)
#define SDA_READ(bus)   RUN_gpio_get((bus)->SDA_Pin)

// 速度控制：4us 延时约等于 100KHz 频率 (标准模式)
#define I2C_DELAY()     RUN_delay_us(4)

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      软件 I2C 引脚初始化 (F407 适配)
// 参数说明      bus             I2C总线对象指针
// 参数说明      scl/sda         指定的引脚枚举值 (来自 RUN_Gpio.h)
// 返回参数      void
// 使用示例      RUN_SoftI2C_Bus_t myI2C; RUN_i2c_soft_init(&myI2C, B6, B7);
// 备注信息      F407 引脚需配置为“开漏输出”以实现线与逻辑及双向通讯
//-------------------------------------------------------------------------------------------------------------------
void RUN_i2c_soft_init(RUN_SoftI2C_Bus_t* bus, RUN_GPIO_enum scl, RUN_GPIO_enum sda)
{
    bus->SCL_Pin = scl;
    bus->SDA_Pin = sda;

    // 初始化 GPIO 为开漏输出 (GPO_OD)
    // 软件 I2C 必须用开漏，否则在读取设备数据时可能发生短路
    RUN_gpio_init(bus->SCL_Pin, GPO_OD, GPIO_Speed_50MHz);
    RUN_gpio_init(bus->SDA_Pin, GPO_OD, GPIO_Speed_50MHz);

    // 释放总线，初始化为高电平
    SCL_H(bus);
    SDA_H(bus);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      发送起始信号 (START)
// 备注信息      时序：SCL 高电平时，SDA 由高变低
//-------------------------------------------------------------------------------------------------------------------
void RUN_i2c_soft_start(RUN_SoftI2C_Bus_t* bus)
{
    SDA_H(bus);
    SCL_H(bus);
    I2C_DELAY();
    SDA_L(bus);
    I2C_DELAY();
    SCL_L(bus); // 钳住总线，准备发送数据
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      发送停止信号 (STOP)
// 备注信息      时序：SCL 高电平时，SDA 由低变高
//-------------------------------------------------------------------------------------------------------------------
void RUN_i2c_soft_stop(RUN_SoftI2C_Bus_t* bus)
{
    SDA_L(bus);
    I2C_DELAY();
    SCL_H(bus);
    I2C_DELAY();
    SDA_H(bus);
    I2C_DELAY();
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      等待从机应答信号 (ACK)
// 返回参数      0: 接收到应答; 1: 未接收到应答
//-------------------------------------------------------------------------------------------------------------------
uint8_t RUN_i2c_soft_wait_ack(RUN_SoftI2C_Bus_t* bus)
{
    uint8_t ack;
    SDA_H(bus);     // 主机释放数据线
    I2C_DELAY();
    SCL_H(bus);     // 驱动 SCL 高，开始采样
    I2C_DELAY();
    ack = SDA_READ(bus); // 读取从机拉低的 SDA
    SCL_L(bus);     // 结束采样
    return ack;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      通过总线发送一个字节
// 参数说明      byte            待发送的 8位数据
//-------------------------------------------------------------------------------------------------------------------
void RUN_i2c_soft_send_byte(RUN_SoftI2C_Bus_t* bus, uint8_t byte)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        // 高位先行 (MSB)
        if (byte & (0x80 >> i)) SDA_H(bus);
        else SDA_L(bus);
        I2C_DELAY();
        SCL_H(bus);
        I2C_DELAY();
        SCL_L(bus);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      从总线读取一个字节
// 参数说明      ack             1: 发送应答, 0: 发送非应答 (用于连续读)
// 返回参数      uint8_t         收到的数据
//-------------------------------------------------------------------------------------------------------------------
uint8_t RUN_i2c_soft_read_byte(RUN_SoftI2C_Bus_t* bus, uint8_t ack)
{
    uint8_t byte = 0x00;
    SDA_H(bus); // 读取前必须释放 SDA
    for (uint8_t i = 0; i < 8; i++)
    {
        I2C_DELAY();
        SCL_H(bus);
        I2C_DELAY();
        if (SDA_READ(bus)) byte |= (0x80 >> i);
        SCL_L(bus);
    }
    // 发送应答位
    if (ack) SDA_L(bus); else SDA_H(bus);
    I2C_DELAY();
    SCL_H(bus);
    I2C_DELAY();
    SCL_L(bus);
    return byte;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      写寄存器封装 (最常用的应用层接口)
// 参数说明      dev_addr        设备 7位地址
// 参数说明      reg_addr        目标寄存器地址
// 参数说明      data            待写入数据
//-------------------------------------------------------------------------------------------------------------------
void RUN_i2c_soft_write_reg(RUN_SoftI2C_Bus_t* bus, uint8_t dev_addr, uint8_t reg_addr, uint8_t data)
{
    RUN_i2c_soft_start(bus);
    RUN_i2c_soft_send_byte(bus, (dev_addr << 1) | 0); // 写模式
    RUN_i2c_soft_wait_ack(bus);
    RUN_i2c_soft_send_byte(bus, reg_addr);
    RUN_i2c_soft_wait_ack(bus);
    RUN_i2c_soft_send_byte(bus, data);
    RUN_i2c_soft_wait_ack(bus);
    RUN_i2c_soft_stop(bus);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      读寄存器封装
// 返回参数      uint8_t         寄存器数值
//-------------------------------------------------------------------------------------------------------------------
uint8_t RUN_i2c_soft_read_reg(RUN_SoftI2C_Bus_t* bus, uint8_t dev_addr, uint8_t reg_addr)
{
    uint8_t data;
    // Step 1: 先写地址告诉设备我们要读哪个寄存器
    RUN_i2c_soft_start(bus);
    RUN_i2c_soft_send_byte(bus, (dev_addr << 1) | 0);
    RUN_i2c_soft_wait_ack(bus);
    RUN_i2c_soft_send_byte(bus, reg_addr);
    RUN_i2c_soft_wait_ack(bus);

    // Step 2: 重复起始信号，切换为读模式
    RUN_i2c_soft_start(bus);
    RUN_i2c_soft_send_byte(bus, (dev_addr << 1) | 1); // 读模式
    RUN_i2c_soft_wait_ack(bus);
    data = RUN_i2c_soft_read_byte(bus, 0); // 读取单字节不应答
    RUN_i2c_soft_stop(bus);
    
    return data;
}