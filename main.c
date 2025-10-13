#include "stm32f4xx.h"

// LD4~LD7 は PD12~PD15 に接続
#define LED_START_PIN 12
#define LED_NUM 4

// 簡単なディレイ関数
void delay(volatile uint32_t count)
{
    while(count--) __NOP(); // 空命令で待機
}

int main(void)
{
    // 1️GPIOD のクロックを有効にする
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

    // 2️PD12~PD15 を汎用プッシュプル出力として設定
    for(int i = 0; i < LED_NUM; i++)
    {
        uint32_t pin = LED_START_PIN + i;

        // モードレジスタ設定
        GPIOD->MODER &= ~(3 << (pin * 2));  // まずモードビットをクリア
        GPIOD->MODER |=  (1 << (pin * 2));  // 出力モードに設定

        GPIOD->OTYPER &= ~(1 << pin);       // プッシュプル
        GPIOD->OSPEEDR |= (3 << (pin * 2)); // 高速
        GPIOD->PUPDR &= ~(3 << (pin * 2));  // プルアップ/プルダウンなし
    }

    // 3 無限ループでrunning water lamp
    while(1)
    {
        for(int i = 0; i < LED_NUM; i++)
        {
            // 現在のLEDを点灯
            GPIOD->ODR |= (1 << (LED_START_PIN + i));
            delay(2000000); // 遅延

            // 現在のLEDを消灯
            GPIOD->ODR &= ~(1 << (LED_START_PIN + i));
        }
    }
}
