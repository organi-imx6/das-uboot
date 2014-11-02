#include <common.h>
#include <spl.h>
#include <asm/imx-common/iomux-v3.h>
#include <asm/arch/mx6-pins.h>
#include <mmc.h>
#include <fsl_esdhc.h>
#include <asm/gpio.h>
#include <asm/arch/clock.h>

DECLARE_GLOBAL_DATA_PTR;

#define UART_PAD_CTRL  (PAD_CTL_PUS_100K_UP |			\
	PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm |			\
	PAD_CTL_SRE_FAST  | PAD_CTL_HYS)

#define USDHC_PAD_CTRL (PAD_CTL_PUS_47K_UP |			\
	PAD_CTL_SPEED_LOW | PAD_CTL_DSE_80ohm |			\
	PAD_CTL_SRE_FAST  | PAD_CTL_HYS)

#ifdef CONFIG_SPL_SERIAL_SUPPORT
static iomux_v3_cfg_t const uart4_pads[] = {
	MX6_PAD_KEY_COL0__UART4_TX_DATA | MUX_PAD_CTRL(UART_PAD_CTRL),
	MX6_PAD_KEY_ROW0__UART4_RX_DATA | MUX_PAD_CTRL(UART_PAD_CTRL),
};
#endif

int board_early_init_f(void)
{
#ifdef CONFIG_SPL_SERIAL_SUPPORT
	imx_iomux_v3_setup_multiple_pads(uart4_pads, ARRAY_SIZE(uart4_pads));
#endif
	return 0;
}

#ifdef CONFIG_SPL_MMC_SUPPORT

struct fsl_esdhc_cfg usdhc_cfg[1] = {
	{USDHC2_BASE_ADDR},
};

static iomux_v3_cfg_t const usdhc2_pads[] = {
	MX6_PAD_SD2_CLK__SD2_CLK   | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD2_CMD__SD2_CMD   | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD2_DAT0__SD2_DATA0 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD2_DAT1__SD2_DATA1 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD2_DAT2__SD2_DATA2 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD2_DAT3__SD2_DATA3 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
};

int board_mmc_getcd(struct mmc *mmc)
{
	return 1;
}

int board_mmc_init(bd_t *bis)
{
	imx_iomux_v3_setup_multiple_pads(usdhc2_pads, ARRAY_SIZE(usdhc2_pads));
	enable_usdhc_clk(1, 2);
	usdhc_cfg[0].sdhc_clk = mxc_get_clock(MXC_ESDHC2_CLK);
	usdhc_cfg[0].max_bus_width = 4;
	return fsl_esdhc_initialize(bis, &usdhc_cfg[0]);
}

#endif /* CONFIG_SPL_MMC_SUPPORT */

void board_init_f(ulong dummy)
{
	arch_cpu_init();

	/* Clear the BSS. */
	memset(__bss_start, 0, __bss_end - __bss_start);

	/* Set global data pointer. */
	gd = &gdata;

	board_early_init_f();

	board_init_r(NULL, 0);
}

void spl_board_init(void)
{
	get_clocks();
#ifdef CONFIG_SPL_SERIAL_SUPPORT
	preloader_console_init();
#endif
}

int spl_start_uboot(void)
{
	// start uboot when console has input
	mdelay(1);
	return tstc();
}

void reset_cpu(ulong addr)
{

}
