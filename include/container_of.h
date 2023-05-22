#ifndef __CONTAINER_OF_H__
#define __CONTAINER_OF_H__

#ifndef offsetof
#if !defined(__CC_ARM) && !defined(__CLANG_ARM)
#define offsetof(type, member) __builtin_offsetof(type, member)
#else
#define offsetof(type, member) \
		((unsigned long)(&((type *)0)->member))
#endif
#endif

#define container_of(ptr, type, member) ({\
		unsigned char *__ptr = (unsigned char *)(ptr); \
		(type *)(__ptr - offsetof(type, member));})

#endif /* __CONTAINER_OF_H__ */
