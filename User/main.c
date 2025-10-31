#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"

int main(void)
{
	OLED_Init();
	
	OLED_ShowBinNum(1, 1, 1, 2);
	OLED_ShowString(2, 1, "Hello world");
	while (1)
	{
		
	}
}
