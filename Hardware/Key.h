#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"

// ======================
// 按键数量与编号
// ======================
#define KEY_COUNT             1   // 只使用 1 个按键

#define KEY_0                 0   // 按键0，对应 PA0

// ======================
// 按键事件标志
// ======================
#define KEY_HOLD              0x01  // 长按中
#define KEY_DOWN              0x02  // 按下事件（按下瞬间）
#define KEY_UP                0x04  // 松开事件
#define KEY_SINGLE            0x08  // 单击（可扩展）
#define KEY_DOUBLE            0x10  // 双击（暂不使用）
#define KEY_LONG              0x20  // 长按触发（可扩展）
#define KEY_REPEAT            0x40  // 重复触发（可扩展）

// ======================
// 函数接口
// ======================
void Key_Init(void);                    // 按键初始化（GPIO配置）
uint8_t Key_Check(uint8_t n, uint8_t Flag);  // 检测按键事件，比如 KEY_DOWN
uint8_t Key_GetState(uint8_t n);        // 获取按键物理状态（一般不用直接调用）
void Key_Tick(void);                    // 按键扫描与状态处理（需周期调用，比如每5~10ms）

#endif
