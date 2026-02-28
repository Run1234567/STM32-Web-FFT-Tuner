#ifndef _RUN_SOFT_I2C_H_
#define _RUN_SOFT_I2C_H_

#include "stm32f4xx.h"
#include "RUN_Gpio.h"
#include "RUN_Delay.h"

// ==============================================================================
// 软件 I2C 总线结构体
// 作用：定义一个 I2C 对象，包含 SCL 和 SDA 所使用的引脚
// ==============================================================================
typedef struct {
    RUN_GPIO_enum SCL_Pin; // 时钟线引脚 (例如: B6)
    RUN_GPIO_enum SDA_Pin; // 数据线引脚 (例如: B7)
} RUN_SoftI2C_Bus_t;

// ==============================================================================
// 函数声明
// ==============================================================================

// 初始化与信号控制
void RUN_i2c_soft_init(RUN_SoftI2C_Bus_t* bus, RUN_GPIO_enum scl, RUN_GPIO_enum sda);
void RUN_i2c_soft_start(RUN_SoftI2C_Bus_t* bus);
void RUN_i2c_soft_stop(RUN_SoftI2C_Bus_t* bus);
uint8_t RUN_i2c_soft_wait_ack(RUN_SoftI2C_Bus_t* bus);

// 字节处理
void RUN_i2c_soft_send_byte(RUN_SoftI2C_Bus_t* bus, uint8_t byte);
uint8_t RUN_i2c_soft_read_byte(RUN_SoftI2C_Bus_t* bus, uint8_t ack);

// 高级应用接口 (直接读写寄存器)
void RUN_i2c_soft_write_reg(RUN_SoftI2C_Bus_t* bus, uint8_t dev_addr, uint8_t reg_addr, uint8_t data);
uint8_t RUN_i2c_soft_read_reg(RUN_SoftI2C_Bus_t* bus, uint8_t dev_addr, uint8_t reg_addr);

#endif