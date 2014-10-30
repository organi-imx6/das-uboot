#include <common.h>
#include <asm/io.h>
#include <asm/arch/clock.h>
#include <asm/arch/imx-regs.h>
#include <asm/arch/iomux.h>
#include <asm/arch/crm_regs.h>
#include <asm/arch/mx6-pins.h>
#include <asm/errno.h>
#include <asm/gpio.h>
#include <asm/imx-common/iomux-v3.h>
#include <asm/imx-common/mxc_i2c.h>
#include <asm/imx-common/boot_mode.h>
#include <mmc.h>
#include <fsl_esdhc.h>
#include <miiphy.h>
#include <netdev.h>
#include <asm/arch/sys_proto.h>
#include <i2c.h>

DECLARE_GLOBAL_DATA_PTR;

#define UART_PAD_CTRL  (PAD_CTL_PUS_100K_UP |			\
	PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm |			\
	PAD_CTL_SRE_FAST  | PAD_CTL_HYS)

#define USDHC_PAD_CTRL (PAD_CTL_PUS_47K_UP |			\
	PAD_CTL_SPEED_LOW | PAD_CTL_DSE_80ohm |			\
	PAD_CTL_SRE_FAST  | PAD_CTL_HYS)

#define ENET_PAD_CTRL  (PAD_CTL_PUS_100K_UP |			\
	PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_HYS)

#define SPI_PAD_CTRL (PAD_CTL_HYS | PAD_CTL_SPEED_MED |		\
	PAD_CTL_DSE_40ohm     | PAD_CTL_SRE_FAST)

int dram_init(void)
{
	gd->ram_size = get_ram_size((void *)PHYS_SDRAM, PHYS_SDRAM_SIZE);

	return 0;
}

static iomux_v3_cfg_t const uart4_pads[] = {
	MX6_PAD_KEY_COL0__UART4_TX_DATA | MUX_PAD_CTRL(UART_PAD_CTRL),
	MX6_PAD_KEY_ROW0__UART4_RX_DATA | MUX_PAD_CTRL(UART_PAD_CTRL),
};

static iomux_v3_cfg_t const enet_pads[] = {
	// RMII
	MX6_PAD_ENET_MDIO__ENET_MDIO      | MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET_MDC__ENET_MDC        | MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET_CRS_DV__ENET_RX_EN   | MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET_RX_ER__ENET_RX_ER    | MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET_TX_EN__ENET_TX_EN    | MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET_RXD0__ENET_RX_DATA0  | MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET_RXD1__ENET_RX_DATA1  | MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET_TXD0__ENET_TX_DATA0  | MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET_TXD1__ENET_TX_DATA1  | MUX_PAD_CTRL(ENET_PAD_CTRL),
	// reset
	MX6_PAD_EIM_EB3__GPIO2_IO31       | MUX_PAD_CTRL(NO_PAD_CTRL),
	// interrupt
	MX6_PAD_ENET_REF_CLK__GPIO1_IO23  | MUX_PAD_CTRL(NO_PAD_CTRL),
	// reference clock
	MX6_PAD_GPIO_16__ENET_REF_CLK     | MUX_PAD_CTRL(NO_PAD_CTRL),
};

static void setup_iomux_enet(void)
{
	imx_iomux_v3_setup_multiple_pads(enet_pads, ARRAY_SIZE(enet_pads));
}

static iomux_v3_cfg_t const usdhc2_pads[] = {
	MX6_PAD_SD2_CLK__SD2_CLK   | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD2_CMD__SD2_CMD   | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD2_DAT0__SD2_DATA0 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD2_DAT1__SD2_DATA1 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD2_DAT2__SD2_DATA2 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD2_DAT3__SD2_DATA3 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
};

static void setup_iomux_uart(void)
{
	imx_iomux_v3_setup_multiple_pads(uart4_pads, ARRAY_SIZE(uart4_pads));
}

#ifdef CONFIG_FSL_ESDHC
static struct fsl_esdhc_cfg usdhc_cfg[1] = {
	{USDHC2_BASE_ADDR},
};

int board_mmc_getcd(struct mmc *mmc)
{
    /* always have card */
	return 1;
}

int board_mmc_init(bd_t *bis)
{
	imx_iomux_v3_setup_multiple_pads(usdhc2_pads, ARRAY_SIZE(usdhc2_pads));

	usdhc_cfg[0].sdhc_clk = mxc_get_clock(MXC_ESDHC2_CLK);
	return fsl_esdhc_initialize(bis, &usdhc_cfg[0]);
}
#endif

#ifdef CONFIG_MXC_SPI
static iomux_v3_cfg_t const ecspi2_pads[] = {
	/* SS2 */
	MX6_PAD_CSI0_DAT11__GPIO5_IO29 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_CSI0_DAT10__ECSPI2_MISO | MUX_PAD_CTRL(SPI_PAD_CTRL),
	MX6_PAD_CSI0_DAT9__ECSPI2_MOSI | MUX_PAD_CTRL(SPI_PAD_CTRL),
	MX6_PAD_CSI0_DAT8__ECSPI2_SCLK | MUX_PAD_CTRL(SPI_PAD_CTRL),
};

static void setup_spi(void)
{
	imx_iomux_v3_setup_multiple_pads(ecspi2_pads, ARRAY_SIZE(ecspi2_pads));
	enable_cspi_clock(1, 1);
}

int board_spi_cs_gpio(unsigned bus, unsigned cs)
{
	return (bus == 1 && cs == 0) ? (IMX_GPIO_NR(5, 29)) : -1;
}
#endif

#ifdef CONFIG_NAND_MXS
iomux_v3_cfg_t nfc_pads[] = {
	MX6_PAD_NANDF_CLE__NAND_CLE		| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_NANDF_ALE__NAND_ALE		| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_NANDF_WP_B__NAND_WP_B	| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_NANDF_RB0__NAND_READY_B	| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_NANDF_CS0__NAND_CE0_B		| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_NANDF_CS1__NAND_CE1_B		| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_NANDF_CS2__NAND_CE2_B		| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_NANDF_CS3__NAND_CE3_B		| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_SD4_CMD__NAND_RE_B		| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_SD4_CLK__NAND_WE_B		| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_NANDF_D0__NAND_DATA00		| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_NANDF_D1__NAND_DATA01		| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_NANDF_D2__NAND_DATA02		| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_NANDF_D3__NAND_DATA03		| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_NANDF_D4__NAND_DATA04		| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_NANDF_D5__NAND_DATA05		| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_NANDF_D6__NAND_DATA06		| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_NANDF_D7__NAND_DATA07		| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_SD4_DAT0__NAND_DQS		| MUX_PAD_CTRL(NO_PAD_CTRL),
};

static void setup_gpmi_nand(void)
{
	struct mxc_ccm_reg *mxc_ccm = (struct mxc_ccm_reg *)CCM_BASE_ADDR;

	/* config gpmi nand iomux */
	imx_iomux_v3_setup_multiple_pads(nfc_pads, ARRAY_SIZE(nfc_pads));

	/* config gpmi and bch clock to 100 MHz */
	clrsetbits_le32(&mxc_ccm->cs2cdr,
			MXC_CCM_CS2CDR_ENFC_CLK_PODF_MASK |
			MXC_CCM_CS2CDR_ENFC_CLK_PRED_MASK |
			MXC_CCM_CS2CDR_ENFC_CLK_SEL_MASK,
			MXC_CCM_CS2CDR_ENFC_CLK_PODF(0) |
			MXC_CCM_CS2CDR_ENFC_CLK_PRED(3) |
			MXC_CCM_CS2CDR_ENFC_CLK_SEL(3));

	/* enable gpmi and bch clock gating */
	setbits_le32(&mxc_ccm->CCGR4,
		     MXC_CCM_CCGR4_RAWNAND_U_BCH_INPUT_APB_MASK |
		     MXC_CCM_CCGR4_RAWNAND_U_GPMI_BCH_INPUT_BCH_MASK |
		     MXC_CCM_CCGR4_RAWNAND_U_GPMI_BCH_INPUT_GPMI_IO_MASK |
		     MXC_CCM_CCGR4_RAWNAND_U_GPMI_INPUT_APB_MASK |
		     MXC_CCM_CCGR4_PL301_MX6QPER1_BCH_OFFSET);

	/* enable apbh clock gating */
	setbits_le32(&mxc_ccm->CCGR0, MXC_CCM_CCGR0_APBHDMA_MASK);
}
#endif

int board_eth_init(bd_t *bis)
{
	struct iomuxc *const iomuxc_regs = (struct iomuxc *)IOMUXC_BASE_ADDR;

	int ret = enable_fec_anatop_clock(ENET_50MHz);
	if (ret)
		return ret;

	// get enet tx reference clk from internal clock from anatop
	setbits_le32(&iomuxc_regs->gpr[1], IOMUXC_GPR1_ENET_CLK_SEL_MASK);

	setup_iomux_enet();

	// set interrupt GPIO
	gpio_direction_input(IMX_GPIO_NR(1, 23));

	// hard reset phy
	gpio_direction_output(IMX_GPIO_NR(2, 31), 0);
	mdelay(1);
	gpio_set_value(IMX_GPIO_NR(2, 31), 1);

	return cpu_eth_init(bis);
}

u32 get_board_rev(void)
{
	return 0;
}

int board_early_init_f(void)
{
	setup_iomux_uart();

	return 0;
}

int board_init(void)
{
	/* address of boot parameters */
	gd->bd->bi_boot_params = PHYS_SDRAM + 0x100;

#ifdef CONFIG_MXC_SPI
	setup_spi();
#endif

#ifdef CONFIG_NAND_MXS
	setup_gpmi_nand();
#endif

	return 0;
}

#ifdef CONFIG_CMD_BMODE
static const struct boot_mode board_boot_modes[] = {
	/* 4 bit bus width */
	{"mmc0", MAKE_CFGVAL(0x40, 0x30, 0x00, 0x00)},
	{NULL,   0},
};
#endif

int board_late_init(void)
{
#ifdef CONFIG_CMD_BMODE
	add_board_boot_modes(board_boot_modes);
#endif

	return 0;
}

int checkboard(void)
{
	return 0;
}
