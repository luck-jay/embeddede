#ifndef __UART_H__
#define __UART_H__

void bsp_uart1_init(void);
void bsp_uart1_dmarx_config(unsigned char *mem_addr, unsigned int mem_size);
unsigned short get_dmarx_buf_remain_size(void);

#endif 
