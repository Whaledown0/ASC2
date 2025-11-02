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

#define ENCODER1_PPR         2500      // 电机1：编码器每圈脉冲数
#define ENCODER1_MULTIPLY    4.0f
#define ENCODER1_PULSES_REV  ((uint32_t)(ENCODER1_PPR * ENCODER1_MULTIPLY)/10*2)  // 10000

// ======================
// 电机2：TIM4 (PB6=A, PB7=B)
// ======================
#define ENCODER2_TIM         TIM4
#define ENCODER2_TIM_RCC     RCC_APB1Periph_TIM4
#define ENCODER2_GPIO_RCC    RCC_APB2Periph_GPIOB
#define ENCODER2_GPIO_PORT   GPIOB
#define ENCODER2_CHA_PIN     GPIO_Pin_6   // A相
#define ENCODER2_CHB_PIN     GPIO_Pin_7   // B相

#define ENCODER2_PPR         2500      // 电机2：编码器每圈脉冲数
#define ENCODER2_MULTIPLY    4.0f
#define ENCODER2_PULSES_REV  ((uint32_t)(ENCODER2_PPR * ENCODER2_MULTIPLY)/10*2)  // 10000

// ======================
// 函数声明
// ======================
void Encoder1_Init(void);
int16_t Encoder1_Get(void);
int16_t Encoder1_GetCount(void);
float Encoder1_GetRPM(int16_t pulse_10ms);

void Encoder2_Init(void);
int16_t Encoder2_Get(void);
int16_t Encoder2_GetCount(void);
float Encoder2_GetRPM(int16_t pulse_10ms);

#endif
