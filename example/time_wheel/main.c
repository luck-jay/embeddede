#include <stm32f10x.h>
#include <misc.h>
#include <stm32f10x_exti.h>

#include <log.h>
#include <timer.h>

static struct timer_list timer;

static void timeout(unsigned long data)
{
    static uint32_t cnt = 0;
    log_i("%d", cnt);
    cnt = cnt + 1;

    /* 再次定时1000ms */
    timer.expires = ticks + 1000;
    add_timer_on(&timer);
}

static void systick_init(void)
{
	SysTick_Config(SystemCoreClock / 1000);
	NVIC_SetPriority(SysTick_IRQn, 0xFF);
}

int main(void)
{
    /* 设置中断分组为4 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    systick_init();


    time_wheel_init();

	init_list(&timer.list);
    /* 定时1000ms */
	timer.expires = ticks + 1000;
	timer.func = timeout;
    /* 添加并启动定时器 */
	add_timer_on(&timer);

	while(1) {
		check_timer_timeout();
	}
}

void SysTick_Handler(void)
{
	tick_increase();
}