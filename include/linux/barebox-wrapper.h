#ifndef __INCLUDE_LINUX_BAREBOX_WRAPPER_H
#define __INCLUDE_LINUX_BAREBOX_WRAPPER_H

#include <malloc.h>
#include <xfuncs.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <printf.h>

#define vmalloc(len)		malloc(len)
#define __vmalloc(len, mode, pgsz)	malloc(len)
#define vzalloc(len)		kzalloc(len, 0)
static inline void vfree(const void *addr)
{
	free((void *)addr);
}

#define __user
#define __init
#define __exit

#define kthread_create(...)	__builtin_return_address(0)
#define kthread_stop(...)	do { } while (0)
#define wake_up_process(...)	do { } while (0)

typedef int irqreturn_t;
#define IRQ_NONE 0
#define IRQ_HANDLED 0

/* To ease clk drivers porting from Linux kernel */
#define __clk_get_name(clk)		(clk->name)
#define __clk_lookup			clk_lookup
#define __clk_get_rate			clk_get_rate
#define __clk_get_parent		clk_get_parent

#define cpu_relax()	barrier()

#endif /* __INCLUDE_LINUX_BAREBOX_WRAPPER_H */
