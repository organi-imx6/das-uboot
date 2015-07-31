#include <common.h>
#include <asm/io.h>
#include <asm/arch/imx-regs.h>
#include <asm/smp.h>
#include <mmc.h>
#include <packimg.h>

DECLARE_GLOBAL_DATA_PTR;

#define BP_SRC_SCR_CORE1_RST		14
#define BP_SRC_SCR_CORE1_ENABLE		22

void smp_entry(void);

uint32_t imx_get_boot_arg(void)
{
	struct src *src_regs = (struct src *)SRC_BASE_ADDR;
	return readl(&src_regs->gpr4);
}

void imx_set_boot_arg(uint32_t value)
{
	struct src *src_regs = (struct src *)SRC_BASE_ADDR;
	writel(value, &src_regs->gpr4);
}

void imx_boot_secondary(void)
{
	uint32_t val;
	struct src *src_regs = (struct src *)SRC_BASE_ADDR;
	writel((uint32_t)smp_entry, &src_regs->gpr3);
	writel(0, &src_regs->gpr4);

	val = readl(&src_regs->scr);
	val |= (1 << BP_SRC_SCR_CORE1_ENABLE) | (1 << BP_SRC_SCR_CORE1_RST);
	writel(val, &src_regs->scr);
}

void imx_kill_secondary(void)
{
	uint32_t val;
	struct src *src_regs = (struct src *)SRC_BASE_ADDR;
	writel(0, &src_regs->gpr3);
	writel(0, &src_regs->gpr4);

	val = readl(&src_regs->scr);
	val &= ~(1 << BP_SRC_SCR_CORE1_ENABLE);
	val |= (1 << BP_SRC_SCR_CORE1_RST);
	writel(val, &src_regs->scr);
}

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

	// SCU CPU status, 0=normal, 2=dormant, 3=poweroff
	//writeb(3, (void *)(SCU_BASE_ADDR + 0x08 + 1));
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

	puts("smp init\n");
	imx_set_boot_arg(SMP_BOOT_DONE_SIGNATURE);
	while (1) {
		uint32_t ret = imx_get_boot_arg();
		if (ret == SMP_START_LOAD_INITRD_SIGNATURE) {
			imx_set_boot_arg(0);
			break;
		}
		if (ret == SMP_ABORT_BOOT_SIGNATURE) {
			puts("smp abort\n");
			err = SMP_SECONDARY_DIE_SIGNATURE;
			goto die;
		}
	}

	timer_init();

	mmc = find_mmc_device(0);

	ph = mmc_get_packimg_header();
	pe = (struct pack_entry *)(ph + 1);
	err = mmc_load_packimg_entry(mmc, CONFIG_SYS_MMCSD_RAW_MODE_INITRD_SECTOR, pe);
	if (!err)
		err = pe->size;

die:
	cpu_enter_lowpower();
	imx_set_boot_arg(err);

	while(1)
		arch_hlt();
}
