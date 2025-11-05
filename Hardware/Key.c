#include "Key.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

// ======================
// 按键引脚配置（只使用 PA0）
// ======================
#define KEY0_GPIO_PORT        GPIOA
#define KEY0_GPIO_PIN         GPIO_Pin_0

// ======================
// 按键状态变量
// ======================
static uint8_t Key_Flag[KEY_COUNT];     // 按键事件标志，比如 KEY_DOWN
static uint8_t CurrState[KEY_COUNT];    // 当前按键状态（0=按下，1=松开）
static uint8_t PrevState[KEY_COUNT];    // 上一状态
static uint8_t S[KEY_COUNT];            // 状态机辅助变量（可扩展用）
static uint16_t Time[KEY_COUNT];        // 按键计时（比如长按延时）

// ======================
// 按键初始化函数
// ======================
void Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 只开启 GPIOA 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 配置 PA0 为上拉输入（按键按下时为低电平 0）
    GPIO_InitStructure.GPIO_Pin = KEY0_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(KEY0_GPIO_PORT, &GPIO_InitStructure);
}

// ======================
// 获取某个按键的物理状态
// 返回：0 = 按下，1 = 松开
// ======================
uint8_t Key_GetState(uint8_t n)
{
    if (n == KEY_0)
    {
        // PA0: 按下时为低电平 0，松开为高电平 1
        if (GPIO_ReadInputDataBit(KEY0_GPIO_PORT, KEY0_GPIO_PIN) == 0)
            return 0;  // 按下
        else
            return 1;  // 松开
    }
    return 1;  // 其它按键（如果有）默认返回松开
}

// ======================
// 按键扫描主函数（必须周期调用，比如每 5~10ms）
// ======================
void Key_Tick(void)
{
    static uint8_t i = 0;

    // 只处理 KEY_0（i = 0）
    if (Time[i] > 0)
        Time[i]--;

    // 保存上一状态
    PrevState[i] = CurrState[i];
    // 读取当前物理状态
    CurrState[i] = Key_GetState(i);

    // 按键按下 = 0，松开 = 1
    if (CurrState[i] == 0)  // 按下状态
    {
        Key_Flag[i] |= KEY_HOLD;  // 标记为“按住中”

        if (PrevState[i] == 1)  // 上一次是松开，这次是按下 → 按下事件
        {
            Key_Flag[i] |= KEY_DOWN;
        }
    }
    else  // 松开状态
    {
        Key_Flag[i] &= ~KEY_HOLD;  // 不再按住

        if (PrevState[i] == 0)  // 上一次是按下，这次是松开 → 松开事件
        {
            Key_Flag[i] |= KEY_UP;
        }
    }
}

// ======================
// 检测某个按键是否发生某个事件
// 比如：Key_Check(KEY_0, KEY_DOWN) → 是否刚按下
// 注意：检测到后，如果你不手动清除标志，它可能会多次触发
// 可以自行在检测到后清除，或者我们在函数里清除（下面提供优化版）
// ======================
uint8_t Key_Check(uint8_t n, uint8_t Flag)
{
    if (n < KEY_COUNT && (Key_Flag[n] & Flag))
    {
        Key_Flag[n] &= ~Flag;  // 检测到后清除该标志（避免重复触发）
        return 1;
    }
    return 0;
}
