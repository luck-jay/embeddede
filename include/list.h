#ifndef __PLT_LIST_H__
#define __PLT_LIST_H__

#include "container_of.h"

struct list {
    struct list *next, *prev;
};

/* 初始化list */
#define LIST_INIT(name) { &(name), &(name) }

/* 定义并初始化list */
#define LIST(name) \
    struct list name = LIST_INIT(name)

/* 初始化list函数 */
static inline void init_list(struct list *list)
{
    list->next = list;
    list->prev = list;
}

/* 在已知的前后指针的情况下插入新节点，将节点插入到中间 */
static inline void __list_add(struct list *new_node,
                struct list *prev,
                struct list *next)
{
    next->prev = new_node;
    new_node->next = next;
    new_node->prev = prev;
    prev->next = new_node;
}

/**
 * 在指定的节点后面添加一个新的节点
 *
 * @new_node: 要添加的新节点
 * @head:     在head后面添加这个新节点
*/
static inline void list_add(struct list *new_node, struct list *head)
{
    __list_add(new_node, head, head->next);
}

/**
 * 在head之前插入新的节点
 *
 * @new_node: 要添加的新节点
 * @head:     将节点添加到head之前
 */
static inline void list_add_tail(struct list *new_node, struct list *head)
{
    __list_add(new_node, head->prev, head);
}

/* 在已知前后指针的情况下删除一个节点 */
static inline void __list_del(struct list *prev, struct list *next)
{
    next->prev = prev;
    prev->next = next;
}

/**
 *  从元素所在的list中移除该元素
 *
 * @entry: 需要从list中移除的节点
 */
static inline void list_del(struct list *entry)
{
    __list_del(entry->prev, entry->next);
}

/**
 * 将原来的节点替换为新节点
 *
 * @old:      需要被替换的节点
 * @new_node: 插入的新节点
 */
static inline void list_replace(struct list *old, struct list *new_node)
{
    new_node->next = old->next;
    new_node->next->prev = new_node;
    new_node->prev = old->prev;
    new_node->prev->next = new_node;
}

/**
 * 将@list节点移动到@head这个list中，并将该节点放到@head之前
 *
 * @list: 需要移动的节点
 * @head: list中原来的节点
 */
static inline void list_move(struct list *list, struct list *head)
{
    __list_del(list->prev, list->next);
    list_add(list, head);
}

/**
 * 从一个list中删除并添加到另一个list的尾部
 *
 * @list: 要移动的节点
 * @head: 另一个list的head
 */
static inline void list_move_tail(struct list *list, struct list *head)
{
    __list_del(list->prev, list->next);
    list_add_tail(list, head);
}

/**
 * 判断节点是否为list最后一个节点
 *
 * @list: 需要判断的节点
 * @head: 该list的头节点
 */
static inline int list_is_last(const struct list *list, const struct list *head)
{
    return list->next == head;
}

/**
 * 判断list是否为空
 *
 * @head: 需要判断的list
 */
static inline int list_empty(const struct list *head)
{
    return head->next == head;
}

/**
 * 判断这个列表是否只有一个元素
 * @head: 需要测试的列表
 */
static inline int list_is_singular(const struct list *head)
{
	return !list_empty(head) && (head->next == head->prev);
}

static inline void __list_cut_position(struct list *list,
		        struct list *head,
                struct list *entry)
{
	struct list *new_first = entry->next;
	list->next = head->next;
	list->next->prev = list;
	list->prev = entry;
	entry->next = list;
	head->next = new_first;
	new_first->prev = head;
}

/**
 * 将list从entry位置分割为两个部分，这两个部分的头分别为list和head
 *
 * @list:  一个空列表
 * @head:  需要分割的list
 * @entry: @list中的一个元素，@list将从这个位置被分割
 */
static inline void list_cut_position(struct list *list,
		struct list *head, struct list *entry)
{
	if (list_empty(head))
		return;
	if (list_is_singular(head) &&
		(head->next != entry && head != entry))
		return;
	if (entry == head)
		init_list(list);
	else
		__list_cut_position(list, head, entry);
}

static inline void __list_splice(const struct list *list,
				 struct list *prev,
				 struct list *next)
{
	struct list *first = list->next;
	struct list *last = list->prev;

	first->prev = prev;
	prev->next = first;

	last->next = next;
	next->prev = last;
}

/**
 * 拼接两个列表
 *
 * @list: 要添加的新列表
 * @head: 新列表添加的位置
 */
static inline void list_splice(const struct list *list,
				struct list *head)
{
	if (!list_empty(list))
		__list_splice(list, head, head->next);
}

/**
 * 将新的列表添加到原列表的尾部
 *
 * @list: 要添加的新列表
 * @head: 原来的列表
 */
static inline void list_splice_tail(struct list *list,
				struct list *head)
{
	if (!list_empty(list))
		__list_splice(list, head->prev, head);
}

/**
 * 拼接两个列表，并重新初始化
 *
 * @list: 要添加的新列表
 * @head: 新列表添加的位置
 */
static inline void list_splice_init(struct list *list,
						struct list *head)
{
	if (!list_empty(list)) {
		__list_splice(list, head, head->next);
		init_list(list);
	}
}

/**
 * 获取该节点的具体内容
 *
 * @ptr:    需要获取内容的list指针
 * @type:   列表数据的类型
 * @member: @ptr指针在列表类型中的名称
 */
#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

/**
 * 获得列表的第一个节点内容
 *
 * @ptr:	需要获取的列表头
 * @type:	该列表的数据结构
 * @member:	@ptr在列表结构中的名称
 *
 * 注意，这个列表不能是空列表
 */
#define list_first_entry(ptr, type, member) \
	list_entry((ptr)->next, type, member)

/**
 * 获得列表的第最后一个节点内容
 *
 * @ptr:	需要获取的列表头
 * @type:	该列表的数据结构
 * @member:	@ptr在列表结构中的名称
 *
 * 注意，这个列表不能是空列表
 */
#define list_last_entry(ptr, type, member) \
	list_entry((ptr)->prev, type, member)

/**
 * 遍历整个列表
 *
 * @pos:  list指针
 * @head: 列表头
 */
#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * 遍历整个列表并得到列表中的具体内容
 *
 * @pos:	列表对应数据结构的指针
 * @head:	对应列表的头
 * @member:	在@pos数据结构中的名称
 */
#define list_for_each_entry(pos, head, member)				\
	for (pos = list_entry((head)->next, typeof(*pos), member);	\
	    &pos->member != (head);	\
	     pos = list_entry(pos->member.next, typeof(*pos), member))

#endif /* __PLT_LIST_H__ */
