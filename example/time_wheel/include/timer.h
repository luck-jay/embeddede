#ifndef __TIMER_H__
#define __TIMER_H__

#include <types.h>
#include <list.h>
#include <clock.h>

struct timer_list {
    struct list list;
    ubase_t expires;

    void (*func)(ubase_t);
    ubase_t data;
};

void check_timer_timeout(void);
void add_timer_on(struct timer_list *timer);
void time_wheel_init(void);

#endif /* __TIMER_H__ */
