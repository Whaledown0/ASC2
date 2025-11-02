#include "Encoder.h"

// --------------------------
// 电机1：TIM3 (PA6/PA7)
// --------------------------
void Encoder1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_ICInitTypeDef TIM_ICInitStruct;

    RCC_APB2PeriphClockCmd(ENCODER1_GPIO_RCC, ENABLE);
    RCC_APB1PeriphClockCmd(ENCODER1_TIM_RCC, ENABLE);

    GPIO_InitStruct.GPIO_Pin = ENCODER1_CHA_PIN | ENCODER1_CHB_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ENCODER1_GPIO_PORT, &GPIO_InitStruct);

    TIM_TimeBaseInitStruct.TIM_Period = 65535;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(ENCODER1_TIM, &TIM_TimeBaseInitStruct);

    TIM_EncoderInterfaceConfig(ENCODER1_TIM, TIM_EncoderMode_TI12,
                               TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    TIM_SetCounter(ENCODER1_TIM, 0);
    TIM_Cmd(ENCODER1_TIM, ENABLE);
}

int16_t Encoder1_Get(void)
{
    int16_t count = (int16_t)TIM_GetCounter(ENCODER1_TIM);
    TIM_SetCounter(ENCODER1_TIM, 0);
    return count;
}

int16_t Encoder1_GetCount(void)
{
    return (int16_t)TIM_GetCounter(ENCODER1_TIM);
}

float Encoder1_GetRPM(int16_t pulse_10ms)
{
    if (pulse_10ms == 0) return 0.0f;
    return (float)pulse_10ms / ENCODER1_PULSES_REV * 6000.0f;
}

// --------------------------
// 电机2：TIM4 (PB6/PB7)
// --------------------------
void Encoder2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_ICInitTypeDef TIM_ICInitStruct;

    RCC_APB2PeriphClockCmd(ENCODER2_GPIO_RCC, ENABLE);
    RCC_APB1PeriphClockCmd(ENCODER2_TIM_RCC, ENABLE);

    GPIO_InitStruct.GPIO_Pin = ENCODER2_CHA_PIN | ENCODER2_CHB_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ENCODER2_GPIO_PORT, &GPIO_InitStruct);

    TIM_TimeBaseInitStruct.TIM_Period = 65535;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(ENCODER2_TIM, &TIM_TimeBaseInitStruct);

    TIM_EncoderInterfaceConfig(ENCODER2_TIM, TIM_EncoderMode_TI12,
                               TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    TIM_SetCounter(ENCODER2_TIM, 0);
    TIM_Cmd(ENCODER2_TIM, ENABLE);
}

int16_t Encoder2_Get(void)
{
    int16_t count = (int16_t)TIM_GetCounter(ENCODER2_TIM);
    TIM_SetCounter(ENCODER2_TIM, 0);
    return count;
}

int16_t Encoder2_GetCount(void)
{
    return (int16_t)TIM_GetCounter(ENCODER2_TIM);
}

float Encoder2_GetRPM(int16_t pulse_10ms)
{
    if (pulse_10ms == 0) return 0.0f;
    return (float)pulse_10ms / ENCODER2_PULSES_REV * 6000.0f;
}
