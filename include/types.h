#ifndef __TYPS_H__
#define __TYPS_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef ARCH_CPU_64BIT
typedef long long            base_t;
typedef unsigned long long   ubase_t;
#else
typedef long                 base_t;
typedef unsigned long        ubase_t;
#endif

typedef int err_t;

#endif /* __TYPS_H__ */
