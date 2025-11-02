#include "stm32f10x.h"
#include "Encoder.h"
#include "OLED.h"
#include "Serial.h"
#include "Delay.h"
#include "Timer.h"   // 包含 TIM2 中断（10ms）
#include "Motor.h"   // 包含 Motor1_SetSpeed() / Motor2_SetSpeed()

// ========== PID 参数（两个电机共用，也可分开） ==========
float Kp = 0.5f, Ki = 0.05f, Kd = 0.01f;

// ========== 电机1 增量式 PID 变量 ==========
float Target1 = 0;      // 电机1 目标转速（RPM）
float Speed1 = 0.0f;       // 电机1 实际转速（RPM，由中断更新）
float Out1 = 0.0f;         // 电机1 当前控制输出 [-100, 100]
float Error1_k   = 0.0f;   // e(k)
float Error1_k_1 = 0.0f;   // e(k-1)
float Error1_k_2 = 0.0f;   // e(k-2)

// ========== 电机2 增量式 PID 变量 ==========
float Target2 = 0;      // 电机2 目标转速（RPM）
float Speed2 = 0.0f;       // 电机2 实际转速（RPM）
float Out2 = 0.0f;         // 电机2 当前控制输出
float Error2_k   = 0.0f;
float Error2_k_1 = 0.0f;
float Error2_k_2 = 0.0f;

// ========== 辅助函数：限制输出范围 ==========
float constrain(float x, float min, float max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

int main(void)
{
    // ----- 初始化所有外设 -----
    OLED_Init();          // OLED 显示屏
    Serial_Init();        // 串口（可选，调试用）
    Timer_Init();         // 定时器（包含 TIM2，每 10ms 中断一次）
    Encoder1_Init();      // 电机1 编码器（TIM3 / PA6/PA7）
    Encoder2_Init();      // 电机2 编码器（TIM4 / PB6/PB7）
    Motor_Init();         // 电机驱动初始化

    // ----- 主循环：只负责显示，不参与控制（控制在中断里）-----
    while (1)
    {
        // --- 电机1 显示信息 ---
        OLED_ShowNum(1, 1, (int)Target1, 3);      // 目标转速
        OLED_ShowNum(2, 1, (int)Speed1, 4);       // 实际转速
        OLED_ShowNum(3, 1, (int)Out1, 5);         // 控制输出
        OLED_ShowNum(1, 6, (int)Error1_k, 5);     // 当前误差 e(k)
        OLED_ShowNum(2, 6, (int)Error1_k_1, 5);   // 上次误差 e(k-1)
        OLED_ShowNum(4, 6, (int)Error1_k_2, 10);  // 上上次误差 e(k-2)

        Delay_ms(100);  // 降低 OLED 刷新率，提升显示稳定性
    }
}

void TIM2_IRQHandler(void)
{
    static uint16_t Count;
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        Count++;
        if (Count >= 10)  // 10ms 控制周期
        {
            Count = 0;

            // ========== 电机1 增量式 PID 控制 ==========
            int16_t pulse1 = Encoder1_Get();               // 10ms 脉冲增量
            Speed1 = Encoder1_GetRPM(pulse1);              // 计算 RPM

            float e1_k = Target1 - Speed1;                 // e(k)
            float deltaOut1 = Kp * (e1_k - Error1_k_1) 
                            + Ki * e1_k 
                            + Kd * (e1_k - 2 * Error1_k_1 + Error1_k_2);

            Out1 += deltaOut1;
            Out1 = constrain(Out1, -150.0f, 150.0f);       // 限幅

            // 更新历史误差
            Error1_k_2 = Error1_k_1;
            Error1_k_1 = e1_k;

            Motor1_SetSpeed((int)Out1);                    // 控制电机1


            // ========== 电机2 增量式 PID 控制 ==========
            int16_t pulse2 = Encoder2_Get();               // 10ms 脉冲增量
            Speed2 = Encoder2_GetRPM(pulse2);

            float e2_k = Target2 - Speed2;
            float deltaOut2 = Kp * (e2_k - Error2_k_1) 
                            + Ki * e2_k 
                            + Kd * (e2_k - 2 * Error2_k_1 + Error2_k_2);

            Out2 += deltaOut2;
            Out2 = constrain(Out2, -100.0f, 100.0f);

            Error2_k_2 = Error2_k_1;
            Error2_k_1 = e2_k;

            Motor2_SetSpeed((int)Out2);                    // 控制电机2
        }

        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
