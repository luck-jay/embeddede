#include "stm32f10x.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_usart.h"
#include "uart.h"
#include "fifo_buf.h"

#define READ_BUF_SIZE 256
static unsigned char uart_tmp_buf[READ_BUF_SIZE];
#define USART1_BUF_SIZE 1024
static unsigned char usart1_buf_data[USART1_BUF_SIZE];

struct uart_buf {
	struct fifo_buf recv_buf;
	unsigned int buf_size;
	unsigned int read_index;
};

static struct uart_buf usart1_buf;
static struct fifo_buf *recv;

#define uart_fifo_free(fifo) \
    (FIFO_SPACE((fifo)->head, (fifo)->tail, USART1_BUF_SIZE))

#define uart_fifo_size(fifo) \
	(FIFO_CNT((fifo)->head, (fifo)->tail, USART1_BUF_SIZE))

#define DMA_READ_SIZE \
	(READ_BUF_SIZE - get_dmarx_buf_remain_size())

int main(void)
{
	bsp_uart1_init();
	bsp_uart1_dmarx_config(uart_tmp_buf, sizeof(uart_tmp_buf));

	recv = &usart1_buf.recv_buf;

	recv->buf = usart1_buf_data;
	recv->head = recv->tail = 0;

	usart1_buf.buf_size = USART1_BUF_SIZE;

	while(1)
	{
		/* 缓冲区中有数据就输出 */
		if (uart_fifo_size(recv))
		{
			/* 等待数据发送完成 */
			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
			USART_SendData(USART1, recv->buf[recv->tail]);

			recv->tail = (recv->tail + 1) & (usart1_buf.buf_size - 1);
		}
	}
}

static inline void fifo_write(struct fifo_buf *buf, unsigned short size)
{
	for (unsigned short i = 0; i < size; ++i)
	{
		/* 缓冲区没满就将数据加入到缓冲区 */
		if (uart_fifo_free(buf) != 0)
		{
			buf->buf[buf->head] = uart_tmp_buf[usart1_buf.read_index + i];
			buf->head = (buf->head + 1) & (usart1_buf.buf_size - 1);
		}
	}
}

void USART1_IRQHandler(void)
{
	unsigned short len;
	unsigned short recv_size;

	/* USART1空闲中断 */
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
		len = DMA_READ_SIZE;
		recv_size = len - usart1_buf.read_index;

		/* 将数据放入环形缓冲区 */
		fifo_write(recv, recv_size);

		usart1_buf.read_index = len;

		/* 为了清除RXN标志位 */
		USART_ReceiveData(USART1);
		USART_ClearITPendingBit(USART1, USART_IT_IDLE);
	}
}

void DMA1_Channel5_IRQHandler(void)
{
	unsigned short len;
	unsigned short recv_size;

	if(DMA_GetITStatus(DMA1_IT_TE5))
	{
		DMA_ClearFlag(DMA1_IT_TE5);
	}

	/* DMA接收半完成中断 */
    if(DMA_GetITStatus(DMA1_IT_HT5))
	{
		len = DMA_READ_SIZE;
		recv_size = len - usart1_buf.read_index;

		/* 将数据放入环形缓冲区 */
		fifo_write(recv, recv_size);

		usart1_buf.read_index = len;

		DMA_ClearFlag(DMA1_FLAG_HT5);
	}

	/* DMA接收完成中断 */
	if(DMA_GetITStatus(DMA1_IT_TC5))
	{
		recv_size = READ_BUF_SIZE - usart1_buf.read_index;

		/* 将数据放入环形缓冲区 */
		fifo_write(recv, recv_size);

		usart1_buf.read_index = 0;

		DMA_ClearFlag(DMA1_FLAG_TC5);
	}
}
