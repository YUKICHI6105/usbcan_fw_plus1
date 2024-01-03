#include "main.h"
#include "led.h"

struct led_status
{
    GPIO_TypeDef *gpio;
    uint16_t pin;
    uint32_t before_tick;
};

#define LED_LIST_SIZE 4
struct led_status led_list[] =
    {
        {LED_CAN_GPIO_Port, LED_CAN_Pin, 0},
        {LED_GREEN_GPIO_Port, LED_GREEN_Pin, 0},
        {LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, 0},
        {LED_RED_GPIO_Port, LED_RED_Pin, 0},
};

void led_on(led name)
{
    // if the led is off.
    HAL_GPIO_WritePin(led_list[name].gpio, led_list[name].pin, GPIO_PIN_SET);
    led_list[name].before_tick = HAL_GetTick();
}

void led_process(void)
{
    for (int i = 0; i < LED_LIST_SIZE; i++)
    {
        if (HAL_GetTick() - led_list[i].before_tick > LED_INTERVAL)
        {
            HAL_GPIO_WritePin(led_list[i].gpio, led_list[i].pin, GPIO_PIN_RESET);
        }
    }
}
