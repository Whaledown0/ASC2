#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f10x.h"

// ======================
// 电机1：TIM3 (PA6=A, PA7=B)
// ======================
#define ENCODER1_TIM         TIM3
#define ENCODER1_TIM_RCC     RCC_APB1Periph_TIM3
#define ENCODER1_GPIO_RCC    RCC_APB2Periph_GPIOA
#define ENCODER1_GPIO_PORT   GPIOA
#define ENCODER1_CHA_PIN     GPIO_Pin_6   // A相
#define ENCODER1_CHB_PIN     GPIO_Pin_7   // B相

// 霍尔编码器参数（TT130 减速后实际值）
#define ENCODER1_PULSES_REV  2496  // 13 × 48 × 4 = 2496 脉冲/圈 ✅（直接使用实际值，不再用 PPR/MULTIPLY）

// ======================
// 电机2：TIM4 (PB6=A, PB7=B)
// ======================
#define ENCODER2_TIM         TIM4
#define ENCODER2_TIM_RCC     RCC_APB1Periph_TIM4
#define ENCODER2_GPIO_RCC    RCC_APB2Periph_GPIOB
#define ENCODER2_GPIO_PORT   GPIOB
#define ENCODER2_CHA_PIN     GPIO_Pin_6   // A相
#define ENCODER2_CHB_PIN     GPIO_Pin_7   // B相

// 霍尔编码器参数（TT130 减速后实际值）
#define ENCODER2_PULSES_REV  2496  // 13 × 48 × 4 = 2496 脉冲/圈 ✅（直接使用实际值）

// ======================
// 函数声明
// ======================
void Encoder1_Init(void);
int16_t Encoder1_Get(void);
int16_t Encoder1_GetCount(void);
int Encoder1_GetRPM(int16_t pulse_10ms);

void Encoder2_Init(void);
int16_t Encoder2_Get(void);
int16_t Encoder2_GetCount(void);
int Encoder2_GetRPM(int16_t pulse_10ms);

#endif
