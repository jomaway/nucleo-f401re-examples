
#include "stm32f4xx_hal.h"

#define LED_PIN                                GPIO_PIN_5
#define LED_GPIO_PORT                          GPIOA

void LED_Init();

int main(void)
{
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the LED pin */
    LED_Init();

    while (1)
    {
        HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_SET);
        HAL_Delay(1000);

        HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_RESET);
        HAL_Delay(1000);

        // You could also use this function instead of the 4 lines above
        // HAL_GPIO_TogglePin(LED_GPIO_PORT,LED_PIN); 
        // HAL_Delay(1000);
    }
}

void LED_Init() {
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /*Configure GPIO pin : LED_Pin */
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);
}

void SysTick_Handler(void) {
    HAL_IncTick();
}