#include <common.h>
#include <asm/io.h>
#include <asm/arch/imx-regs.h>
#include <asm/smp.h>
#include <mmc.h>
#include <packimg.h>

DECLARE_GLOBAL_DATA_PTR;

#define BP_SRC_SCR_CORE1_RST		14
#define BP_SRC_SCR_CORE1_ENABLE		22

unsigned int smp_boot_done = 0;
unsigned int smp_start_load_initrd = 0;

void smp_entry(void);

static inline void cpu_enter_lowpower(void)
{
	unsigned int v;

	asm volatile(
		"mcr	p15, 0, %1, c7, c5, 0\n"
	"	mcr	p15, 0, %1, c7, c10, 4\n"
	/*
	 * Turn off coherency
	 */
	"	mrc	p15, 0, %0, c1, c0, 1\n"
	"	bic	%0, %0, %3\n"
	"	mcr	p15, 0, %0, c1, c0, 1\n"
	"	mrc	p15, 0, %0, c1, c0, 0\n"
	"	bic	%0, %0, %2\n"
	"	mcr	p15, 0, %0, c1, c0, 0\n"
	  : "=&r" (v)
	  : "r" (0), "Ir" (CR_C), "Ir" (0x40)
	  : "cc");
}

static inline void arch_hlt(void)
{
    asm volatile ("dsb\n"
		  "wfi\n"
		  : : : "memory");
}

void smp_init(void)
{
	int err;
	struct mmc *mmc;
	struct pack_header *ph;
	struct pack_entry *pe;
	struct src *src_regs = (struct src *)SRC_BASE_ADDR;

	puts("smp init\n");
	smp_boot_done = SMP_BOOT_DONE_SIGNATURE;
	while (readl(&smp_start_load_initrd) != SMP_START_LOAD_INITRD_SIGNATURE);

	timer_init();

	mmc = find_mmc_device(0);

	ph = mmc_get_packimg_header();
	pe = (struct pack_entry *)(ph + 1);
	err = mmc_load_packimg_entry(mmc, CONFIG_SYS_MMCSD_RAW_MODE_INITRD_SECTOR, pe);
	if (!err)
		err = pe->size;

	cpu_enter_lowpower();

	writel(err, &src_regs->gpr4);

	while(1)
		arch_hlt();
}

void smp_boot(void)
{
	unsigned int val;
	struct src *src_regs = (struct src *)SRC_BASE_ADDR;
	writel((uint32_t)smp_entry, &src_regs->gpr3);
	writel(0, &src_regs->gpr4);

	val = readl(&src_regs->scr);
	val |= (1 << BP_SRC_SCR_CORE1_ENABLE) | (1 << BP_SRC_SCR_CORE1_RST);
	writel(val, &src_regs->scr);
}
