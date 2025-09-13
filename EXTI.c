#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"
#include "misc.h"


void delay_ms(int time)
{
    for (int i = 0; i < time; i++)
    {
        for (int j = 0; j < 0x2AFF; j++); 
    }
}


void GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

   
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);


    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);


    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);


    GPIO_ResetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_7);
}


void EXTI_Config(void)
{
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

  
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);

 
    EXTI_ClearITPendingBit(EXTI_Line6);
    EXTI_InitStruct.EXTI_Line = EXTI_Line6;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; 
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);

 
    NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}


void EXTI9_5_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line6) == SET)
    {
        
        if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_5) == Bit_SET)
            GPIO_ResetBits(GPIOA, GPIO_Pin_5);
        else
            GPIO_SetBits(GPIOA, GPIO_Pin_5);

       
        EXTI_ClearITPendingBit(EXTI_Line6);
    }
}

int main(void)
{
    GPIO_Config();
    EXTI_Config();

 
    while (1)
    {
      
        if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_7) == Bit_SET)
            GPIO_ResetBits(GPIOA, GPIO_Pin_7);
        else
            GPIO_SetBits(GPIOA, GPIO_Pin_7);

        delay_ms(500); 
    }
}
