#include "stm32f4xx.h"

volatile uint32_t msTicks = 0; // 毫秒计数

// SysTick中断，每1ms触发
void SysTick_Handler(void)
{
    msTicks++;
}

int main(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能GPIOD时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    // 配置SysTick，每1ms中断一次
    SysTick_Config(SystemCoreClock / 1000);

    // 配置PD11(蜂鸣器)和PD12(LED)为推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    // 初始状态: LED灭, 蜂鸣器OFF
    GPIO_ResetBits(GPIOD, GPIO_Pin_12);
    GPIO_ResetBits(GPIOD, GPIO_Pin_11);

    // 定时器变量
    uint32_t ledTimer = msTicks;
    uint32_t buzzerTimer = msTicks;
    uint8_t buzzerOn = 0;

		GPIO_SetBits(GPIOD, GPIO_Pin_11); 
		buzzerTimer = msTicks;            
		buzzerOn = 1;  

    while (1)
    {
        // LED: 1秒翻转一次
        if ((msTicks - ledTimer) >= 1000)
        {
            GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
            ledTimer = msTicks;
        }

        // 蜂鸣器: 
        if (!buzzerOn && (msTicks - buzzerTimer) >= 25 * 60 * 1000)
        {
            GPIO_SetBits(GPIOD, GPIO_Pin_11); // 高电平
            buzzerTimer = msTicks;            // 重置定时器
            buzzerOn = 1;                     // 标记蜂鸣器正在响
        }

        // 
        if (buzzerOn && (msTicks - buzzerTimer) >= 50)
        {
            GPIO_ResetBits(GPIOD, GPIO_Pin_11); // 低电平
            buzzerTimer = msTicks;              // 下一轮30秒计时
            buzzerOn = 0;                       // 标记蜂鸣器已关闭
        }
    }
}
