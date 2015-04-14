#include <common.h>
#include <asm/io.h>
#include <div64.h>
#include <asm/arch/imx-regs.h>
#include <asm/arch/clock.h>

struct mxc_private_timer {
	uint32_t load;
	uint32_t counter;
	uint32_t control;
	uint32_t int_status;
};

static struct mxc_private_timer *cur_ptimer = (struct mxc_private_timer *)PRIVATE_TIMERS_WD_BASE_ADDR;

DECLARE_GLOBAL_DATA_PTR;

#define MXC_PTIMER_CTRL_EN         0x01
#define MXC_PTIMER_CTRL_AUTORELOAD 0x02
#define MXC_PTIMER_CTRL_IRQ_EN     0x04
#define MXC_PTIMER_CTRL_PRESCALER_SHIFT 8

#define MXC_PTIMER_PRESCALER 3
#define MXC_PTIMER_CLK (mxc_get_clock(MXC_ARM_CLK) / ((MXC_PTIMER_PRESCALER + 1) * 2))

static inline unsigned long long tick_to_time(unsigned long long tick)
{
	tick *= CONFIG_SYS_HZ;
	do_div(tick, MXC_PTIMER_CLK);

	return tick;
}

static inline unsigned long long us_to_tick(unsigned long long usec)
{
	usec = usec * MXC_PTIMER_CLK  + 999999;
	do_div(usec, 1000000);

	return usec;
}

int timer_init(void)
{
	writel(0xffffffff, &cur_ptimer->load);
	writel((MXC_PTIMER_PRESCALER << MXC_PTIMER_CTRL_PRESCALER_SHIFT) | 
		   MXC_PTIMER_CTRL_AUTORELOAD | MXC_PTIMER_CTRL_EN, 
		   &cur_ptimer->control);

	gd->arch.tbl = ~readl(&cur_ptimer->counter);
	gd->arch.tbu = 0;

	return 0;
}

unsigned long long get_ticks(void)
{
	ulong now = ~readl(&cur_ptimer->counter); /* current tick value */

	/* increment tbu if tbl has rolled over */
	if (now < gd->arch.tbl)
		gd->arch.tbu++;
	gd->arch.tbl = now;
	return (((unsigned long long)gd->arch.tbu) << 32) | gd->arch.tbl;
}

ulong get_timer_masked(void)
{
	return tick_to_time(get_ticks());
}

ulong get_timer(ulong base)
{
	return get_timer_masked() - base;
}

/* delay x useconds AND preserve advance timstamp value */
void __udelay(unsigned long usec)
{
	unsigned long long tmp;
	ulong tmo;

	tmo = us_to_tick(usec);
	tmp = get_ticks() + tmo;	/* get current timestamp */

	while (get_ticks() < tmp)	/* loop till event */
		 /*NOP*/;
}

/*
 * This function is derived from PowerPC code (timebase clock frequency).
 * On ARM it returns the number of timer ticks per second.
 */
ulong get_tbclk(void)
{
	return MXC_PTIMER_CLK;
}


