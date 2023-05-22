#include <timer.h>
#include <clock.h>

#define TVN_BITS 6
#define TVR_BITS 8
#define TVN_SIZE (1 << TVN_BITS)
#define TVR_SIZE (1 << TVR_BITS)
#define TVN_MASK (TVN_SIZE - 1)
#define TVR_MASK (TVR_SIZE - 1)

struct tvec_s {
    struct list vec[TVN_SIZE];
};

struct tvec_root_s {
    struct list vec[TVR_SIZE];
};

struct timer_wheel {
    uint32_t timer_ticks;
    struct timer_list *running_timer;
    struct tvec_root_s tv1;
    struct tvec_s tv2;
    struct tvec_s tv3;
    struct tvec_s tv4;
    struct tvec_s tv5;
};

static struct timer_wheel timer_wheel_base;

static inline void set_runing_timer(struct timer_wheel *base,
                                    struct timer_list *timer)
{
    base->running_timer = timer;
}

static void add_timer(struct timer_wheel *base, struct timer_list *timer)
{
    ubase_t expires = timer->expires;
	ubase_t idx = expires - base->timer_ticks;
	struct list *vec;

    if (idx < TVR_SIZE) {
		int i = expires & TVR_MASK;
		vec = base->tv1.vec + i;
	} else if (idx < 1 << (TVR_BITS + TVN_BITS)) {
		int i = (expires >> TVR_BITS) & TVN_MASK;
		vec = base->tv2.vec + i;
	} else if (idx < 1 << (TVR_BITS + 2 * TVN_BITS)) {
		int i = (expires >> (TVR_BITS + TVN_BITS)) & TVN_MASK;
		vec = base->tv3.vec + i;
    } else if (idx < 1 << (TVR_BITS + 3 * TVN_BITS)) {
		int i = (expires >> (TVR_BITS + 2 * TVN_BITS)) & TVN_MASK;
		vec = base->tv4.vec + i;
	} else if ((int64_t)idx < 0) {
		vec = base->tv1.vec + (base->timer_ticks & TVR_MASK);
	} else {
		int i;
		if (idx > 0xffffffffUL) {
			idx = 0xffffffffUL;
			expires = idx + base->timer_ticks;
		}
		i = (expires >> (TVR_BITS + 3 * TVN_BITS)) & TVN_MASK;
		vec = base->tv5.vec + i;
	}
	/*
	 * Timers are FIFO:
	 */
	list_add_tail(&timer->list, vec);
}

/**
 * @brief 添加一个定时器并启动
 *
 * @param timer 需要添加的定时器
*/
void add_timer_on(struct timer_list *timer)
{
    struct timer_wheel *base = &timer_wheel_base;

    add_timer(base, timer);
}

static int cascade(struct timer_wheel *base, struct tvec_s *tv, int index)
{
    struct list *head, *current;

    head = tv->vec + index;
    current = head->next;

    while (current != head) {
        struct timer_list *tmp;

        tmp = list_entry(current, struct timer_list, list);
        current = current->next;
        add_timer(base, tmp);
    }
    init_list(head);

    return index;
}

#define INDEX(N) (base->timer_ticks >> (TVR_BITS + N * TVN_BITS)) & TVN_MASK

static inline void run_timers(struct timer_wheel *base)
{
    struct timer_list *timer;
    struct list work_list;
    struct list *head = &work_list;
    uint8_t index = base->timer_ticks & TVR_MASK;

    if (!index &&
        (!cascade(base, &base->tv2, INDEX(0))) &&
            (!cascade(base, &base->tv3, INDEX(1))) &&
                (!cascade(base, &base->tv4, INDEX(2))))
        cascade(base, &base->tv5, INDEX(3));

    ++base->timer_ticks;

    list_splice_init(base->tv1.vec + index, &work_list);

    while (!list_empty(head)) {
        void (*fn)(ubase_t);
        ubase_t data;

        timer = list_entry(head->next, struct timer_list, list);
        fn = timer->func;
        data = timer->data;

        list_del(&timer->list);
        set_runing_timer(base, timer);
        if (fn)
            fn(data);
    }

    set_runing_timer(base, NULL);
}

/**
 * @brief 检查定时器是否超时
*/
void check_timer_timeout(void)
{
    struct timer_wheel *base = &timer_wheel_base;

    if (time_after_eq(ticks, base->timer_ticks))
        run_timers(base);
}

/**
 * @brief 时间轮初始化
*/
void time_wheel_init(void)
{
    for (int i = 0; i < TVR_SIZE; ++i)
        init_list(&timer_wheel_base.tv1.vec[i]);
    for (int i = 0; i < TVN_SIZE; ++i) {
        init_list(&timer_wheel_base.tv2.vec[i]);
        init_list(&timer_wheel_base.tv3.vec[i]);
        init_list(&timer_wheel_base.tv4.vec[i]);
        init_list(&timer_wheel_base.tv5.vec[i]);
    }
}
