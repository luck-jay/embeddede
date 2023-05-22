#ifndef __PLT_CLOCK_H__
#define __PLT_CLOCK_H__

#include <types.h>
#include <limits.h>

extern volatile ubase_t ticks;

#define MAX_TICK ((LONG_MAX >> 1) - 1)

#define _ms_to_tick(ms)  ((1000 / 10 * (ms) - 1) / 100)

/**
 * @brief 将ms数转为系统tick数
 *
 * @param ms 需要转换的ms数
 *
 * @return 转换后的系统tick数
 */
static inline ubase_t ms_to_ticks(const uint32_t ms)
{
    if ((int)ms < 0)
        return MAX_TICK;
    else
        return _ms_to_tick(ms);
}

/**
 * @brief 比较两个系统tick数那一个更靠后
 *
 * @param a 比较的系统tick数
 *
 * @param b 被比较的系统tick数
 *
 * @return a 在 b 的后面，则返回true
 *
 * Note: 两个系统tick数的差值不能超过baset_t 类型的最大大小
 */
#define time_after(a, b) \
    ((base_t)(b) - (base_t)(a) < 0)
#define time_before(a,b)	time_after(b,a)

#define time_after_eq(a, b) \
    ((base_t)(a) - (base_t)(b) >= 0)
#define time_before_eq(a,b)	time_after_eq(b,a)

void mdelay(uint32_t ms);
void tick_increase(void);

#endif /* __PLT_CLOCK_H__ */
