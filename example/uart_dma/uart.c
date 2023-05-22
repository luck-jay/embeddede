#include "stm32f10x.h"
#include "misc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"

static void bsp_uart1_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
 * @brief  uart1 dma接收通道配置
 * @param
 * @retval
 */
void bsp_uart1_dmarx_config(uint8_t *mem_addr, uint32_t mem_size)
{
  	DMA_InitTypeDef DMA_InitStructure;

	DMA_DeInit(DMA1_Channel5);
	DMA_Cmd(DMA1_Channel5, DISABLE);
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= (uint32_t)&(USART1->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr 		= (uint32_t)mem_addr;
	DMA_InitStructure.DMA_DIR 					= DMA_DIR_PeripheralSRC; 	/* 传输方向:外设->内存 */
	DMA_InitStructure.DMA_BufferSize 			= mem_size;
	DMA_InitStructure.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize 		= DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode 					= DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority 				= DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M 					= DMA_M2M_Disable;
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);

	DMA_ITConfig(DMA1_Channel5, DMA_IT_TC|DMA_IT_HT|DMA_IT_TE, ENABLE);/* 使能DMA半满、溢满、错误中断 */
	DMA_ClearFlag(DMA1_IT_TC5);
	DMA_ClearFlag(DMA1_IT_HT5);
	DMA_Cmd(DMA1_Channel5, ENABLE);
}

/**
 * @brief  uart1初始化配置
 * @param
 * @retval
 */
void bsp_uart1_init(void)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	bsp_uart1_gpio_init();

	/* 使能串口和DMA时钟 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	USART_InitStructure.USART_BaudRate            = 115200;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);	/* 使能空闲中断 */
	USART_Cmd(USART1, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE); /* 使能DMA收发 */

	/* 串口中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* DMA中断 */
	NVIC_InitStructure.NVIC_IRQChannel    = DMA1_Channel4_IRQn;  /* UART1 DMA1Tx*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel    = DMA1_Channel5_IRQn; /* UART1 DMA1Rx*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
}

void bsp_uart1_dmatx_config(unsigned char *mem_addr, unsigned int mem_size)
{
  	DMA_InitTypeDef DMA_InitStructure;

	DMA_DeInit(DMA1_Channel4);
	DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= (uint32_t)&(USART1->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr 		= (uint32_t)mem_addr;
	DMA_InitStructure.DMA_DIR 					= DMA_DIR_PeripheralDST; 	/* 传输方向:内存->外设 */
	DMA_InitStructure.DMA_BufferSize 			= mem_size;
	DMA_InitStructure.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize 		= DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode 					= DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority 				= DMA_Priority_High;
	DMA_InitStructure.DMA_M2M 					= DMA_M2M_Disable;
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC|DMA_IT_TE, ENABLE);
	DMA_ClearFlag(DMA1_IT_TC4);	/* 清除发送完成标识 */
	DMA_Cmd(DMA1_Channel4, ENABLE);
}

unsigned short get_dmarx_buf_remain_size(void)
{
	return DMA_GetCurrDataCounter(DMA1_Channel5);
}
