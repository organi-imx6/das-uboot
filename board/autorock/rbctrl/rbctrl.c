#include <common.h>
#ifdef CONFIG_SPL_BUILD
#include <spl.h>
#endif
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
#include <asm/imx-common/regs-gpmi.h>
#include <mmc.h>
#include <fsl_esdhc.h>
#include <miiphy.h>
#include <netdev.h>
#include <asm/arch/sys_proto.h>
#include <i2c.h>
#include <nand.h>
#include <packimg.h>
#include <aes-packimg.h>

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

#if defined(CONFIG_SPL_SERIAL_SUPPORT) || !defined(CONFIG_SPL_BUILD)
static iomux_v3_cfg_t const uart2_pads[] = {
	MX6_PAD_EIM_D26__UART2_TX_DATA | MUX_PAD_CTRL(UART_PAD_CTRL),
	MX6_PAD_EIM_D27__UART2_RX_DATA | MUX_PAD_CTRL(UART_PAD_CTRL),
};
static inline void setup_iomux_uart(void)
{
	imx_iomux_v3_setup_multiple_pads(uart2_pads, ARRAY_SIZE(uart2_pads));
}
#else
#define setup_iomux_uart()
#endif

#if defined(CONFIG_FSL_ESDHC) || (defined(CONFIG_SPL_BUILD)&&defined(CONFIG_SPL_MMC_SUPPORT))
static struct fsl_esdhc_cfg usdhc_cfg[2] = {
	{USDHC2_BASE_ADDR},
	{USDHC3_BASE_ADDR},
};

static iomux_v3_cfg_t const usdhc2_pads[] = {
	MX6_PAD_SD2_CLK__SD2_CLK   | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD2_CMD__SD2_CMD   | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD2_DAT0__SD2_DATA0 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD2_DAT1__SD2_DATA1 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD2_DAT2__SD2_DATA2 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD2_DAT3__SD2_DATA3 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
};

static iomux_v3_cfg_t const usdhc3_pads[] = {
	MX6_PAD_SD3_CLK__SD3_CLK   | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_CMD__SD3_CMD   | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT0__SD3_DATA0 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT1__SD3_DATA1 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT2__SD3_DATA2 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT3__SD3_DATA3 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT4__SD3_DATA4 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT5__SD3_DATA5 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT6__SD3_DATA6 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT7__SD3_DATA7 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
};

int board_mmc_getcd(struct mmc *mmc)
{
    /* always have card */
	return 1;
}

#ifdef CONFIG_SPL_BUILD
int board_mmc_init(bd_t *bis)
{
	int device = spl_boot_device();
	switch (device) {
	case BOOT_DEVICE_MMC1:
		imx_iomux_v3_setup_multiple_pads(usdhc2_pads, ARRAY_SIZE(usdhc2_pads));
		enable_usdhc_clk(1, 2);
		usdhc_cfg[0].sdhc_clk = mxc_get_clock(MXC_ESDHC2_CLK);
		usdhc_cfg[0].max_bus_width = 4;
		return fsl_esdhc_initialize(bis, &usdhc_cfg[0]);
	case BOOT_DEVICE_MMC2:
		imx_iomux_v3_setup_multiple_pads(usdhc3_pads, ARRAY_SIZE(usdhc3_pads));
		enable_usdhc_clk(1, 3);
		usdhc_cfg[1].sdhc_clk = mxc_get_clock(MXC_ESDHC3_CLK);
		usdhc_cfg[1].max_bus_width = 8;
		return fsl_esdhc_initialize(bis, &usdhc_cfg[1]);
	default:
		return -1;
	}
}
#else
int board_mmc_init(bd_t *bis)
{
	int status = 0;

	imx_iomux_v3_setup_multiple_pads(usdhc2_pads, ARRAY_SIZE(usdhc2_pads));
	enable_usdhc_clk(1, 2);
	usdhc_cfg[0].sdhc_clk = mxc_get_clock(MXC_ESDHC2_CLK);
	usdhc_cfg[0].max_bus_width = 4;
	status = fsl_esdhc_initialize(bis, &usdhc_cfg[0]);

	imx_iomux_v3_setup_multiple_pads(usdhc3_pads, ARRAY_SIZE(usdhc3_pads));
	enable_usdhc_clk(1, 3);
	usdhc_cfg[1].sdhc_clk = mxc_get_clock(MXC_ESDHC3_CLK);
	usdhc_cfg[1].max_bus_width = 8;
	status |= fsl_esdhc_initialize(bis, &usdhc_cfg[1]);

	return status;
}
#endif	//#ifdef CONFIG_SPL_BUILD
#endif

#if defined(CONFIG_MXC_SPI) || (defined(CONFIG_SPL_BUILD)&&defined(CONFIG_BOOT_SPI))
static iomux_v3_cfg_t const ecspi1_pads[] = {
	MX6_PAD_EIM_EB2__GPIO2_IO30 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_EIM_D17__ECSPI1_MISO | MUX_PAD_CTRL(SPI_PAD_CTRL),
	MX6_PAD_EIM_D18__ECSPI1_MOSI | MUX_PAD_CTRL(SPI_PAD_CTRL),
	MX6_PAD_EIM_D16__ECSPI1_SCLK | MUX_PAD_CTRL(SPI_PAD_CTRL),
};

static void setup_spi(void)
{
	imx_iomux_v3_setup_multiple_pads(ecspi1_pads, ARRAY_SIZE(ecspi1_pads));
	enable_cspi_clock(1, CONFIG_SF_DEFAULT_BUS);
}

int board_spi_cs_gpio(unsigned bus, unsigned cs)
{
	return (bus == CONFIG_SF_DEFAULT_BUS && cs == CONFIG_SF_DEFAULT_CS) 
		? (IMX_GPIO_NR(2, 30)) : -1;
}
#endif

#if defined(CONFIG_SPL_BUILD)&&defined(CONFIG_BOOT_SPI)
static int load_image_os(struct spi_flash *flash, struct image_header *header)
{
	init_aes();
	nand_init();

	/* load linux */
	nand_spl_load_image(CONFIG_SYS_NAND_SPL_KERNEL_OFFS,
		sizeof(*header), (void *)header);

	aes_dec(header, sizeof(*header));

	spl_parse_image_header(header);
	if (header->ih_os == IH_OS_LINUX) {
		/* happy - was a linux */
		sf_load_packimg(flash, CONFIG_SYS_SPI_PACK_OFFS, NULL);

		nand_spl_load_image(CONFIG_SYS_NAND_SPL_KERNEL_OFFS,
			spl_image.size, (void *)spl_image.load_addr);
		aes_dec(spl_image.load_addr, spl_image.size);

		nand_deselect();
		return 0;
	}
	puts("The Expected Linux image was not "
		"found. Please check your NAND "
		"configuration.\n");
	puts("Trying to start u-boot now...\n");
	return -1;
}

static void rbctrl_spl_spi_load_image(void)
{
	struct spi_flash *flash;
	struct image_header *header;

	flash = spi_flash_probe(CONFIG_SF_DEFAULT_BUS,
				CONFIG_SF_DEFAULT_CS,
				CONFIG_SF_DEFAULT_SPEED,
				CONFIG_SF_DEFAULT_MODE);
	if (!flash) {
		puts("SPI probe failed.\n");
		hang();
	}

	/* use CONFIG_SYS_TEXT_BASE as temporary storage area */
	header = (struct image_header *)(CONFIG_SYS_TEXT_BASE);

#ifdef CONFIG_SPL_OS_BOOT
	if (spl_start_uboot() || load_image_os(flash, header))
#endif
	{
		/* Load u-boot, mkimage header is 64 bytes. */
		spi_flash_read(flash, CONFIG_SYS_SPI_U_BOOT_OFFS, sizeof(*header),
			       (void *)header);
		spl_parse_image_header(header);
		spi_flash_read(flash, CONFIG_SYS_SPI_U_BOOT_OFFS,
			       spl_image.size, (void *)spl_image.load_addr);
	}
}

static void board_boot(void)
{
	extern void __noreturn jump_to_image_no_args(struct spl_image_info *);
	u32 boot_device;

#ifdef CONFIG_SYS_SPL_MALLOC_START
	mem_malloc_init(CONFIG_SYS_SPL_MALLOC_START,
			CONFIG_SYS_SPL_MALLOC_SIZE);
#endif
	timer_init();

#ifdef CONFIG_SPL_BOARD_INIT
	spl_board_init();
#endif

	boot_device = spl_boot_device();
	debug("boot device - %d\n", boot_device);
	switch (boot_device){
#ifdef CONFIG_SPL_RAM_DEVICE
	case BOOT_DEVICE_RAM:
		spl_ram_load_image();
		break;
#endif
#ifdef CONFIG_SPL_MMC_SUPPORT
	case BOOT_DEVICE_MMC1:
	case BOOT_DEVICE_MMC2:
	case BOOT_DEVICE_MMC2_2:
		spl_mmc_load_image();
		break;
#endif
#ifdef CONFIG_SPL_NAND_SUPPORT
	case BOOT_DEVICE_NAND:
		spl_nand_load_image();
		break;
#endif
#ifdef CONFIG_SPL_ONENAND_SUPPORT
	case BOOT_DEVICE_ONENAND:
		spl_onenand_load_image();
		break;
#endif
#ifdef CONFIG_SPL_NOR_SUPPORT
	case BOOT_DEVICE_NOR:
		spl_nor_load_image();
		break;
#endif
#ifdef CONFIG_SPL_YMODEM_SUPPORT
	case BOOT_DEVICE_UART:
		spl_ymodem_load_image();
		break;
#endif
#ifdef CONFIG_SPL_SPI_SUPPORT
	case BOOT_DEVICE_SPI:
		rbctrl_spl_spi_load_image();
		break;
#endif
#ifdef CONFIG_SPL_ETH_SUPPORT
	case BOOT_DEVICE_CPGMAC:
#ifdef CONFIG_SPL_ETH_DEVICE
		spl_net_load_image(CONFIG_SPL_ETH_DEVICE);
#else
		spl_net_load_image(NULL);
#endif
		break;
#endif
#ifdef CONFIG_SPL_USBETH_SUPPORT
	case BOOT_DEVICE_USBETH:
		spl_net_load_image("usb_ether");
		break;
#endif
#ifdef CONFIG_SPL_USB_SUPPORT
	case BOOT_DEVICE_USB:
		spl_usb_load_image();
		break;
#endif
#ifdef CONFIG_SPL_SATA_SUPPORT
	case BOOT_DEVICE_SATA:
		spl_sata_load_image();
		break;
#endif
	default:
		debug("SPL: Un-supported Boot Device\n");
		hang();
	}

	switch (spl_image.os) {
	case IH_OS_U_BOOT:
		debug("Jumping to U-Boot\n");
		break;
#ifdef CONFIG_SPL_OS_BOOT
	case IH_OS_LINUX:
		debug("Jumping to Linux\n");
		spl_board_prepare_for_linux();
		jump_to_image_linux((void *)CONFIG_SYS_SPL_ARGS_ADDR);
#endif
	default:
		debug("Unsupported OS image.. Jumping nevertheless..\n");
	}
	jump_to_image_no_args(&spl_image);
}
#endif //#if defined(CONFIG_SPL_BUILD)&&defined(CONFIG_BOOT_SPI)

#if defined(CONFIG_NAND_MXS) || (defined(CONFIG_SPL_BUILD)&&defined(CONFIG_SPL_NAND_SUPPORT))
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

/**
 * struct nand_timing - Fundamental timing attributes for NAND.
 * @data_setup_in_ns:         The data setup time, in nanoseconds. Usually the
 *                            maximum of tDS and tWP. A negative value
 *                            indicates this characteristic isn't known.
 * @data_hold_in_ns:          The data hold time, in nanoseconds. Usually the
 *                            maximum of tDH, tWH and tREH. A negative value
 *                            indicates this characteristic isn't known.
 * @address_setup_in_ns:      The address setup time, in nanoseconds. Usually
 *                            the maximum of tCLS, tCS and tALS. A negative
 *                            value indicates this characteristic isn't known.
 * @gpmi_sample_delay_in_ns:  A GPMI-specific timing parameter. A negative value
 *                            indicates this characteristic isn't known.
 * @tREA_in_ns:               tREA, in nanoseconds, from the data sheet. A
 *                            negative value indicates this characteristic isn't
 *                            known.
 * @tRLOH_in_ns:              tRLOH, in nanoseconds, from the data sheet. A
 *                            negative value indicates this characteristic isn't
 *                            known.
 * @tRHOH_in_ns:              tRHOH, in nanoseconds, from the data sheet. A
 *                            negative value indicates this characteristic isn't
 *                            known.
 */
struct nand_timing {
	int8_t  data_setup_in_ns;
	int8_t  data_hold_in_ns;
	int8_t  address_setup_in_ns;
	int8_t  gpmi_sample_delay_in_ns;
	int8_t  tREA_in_ns;
	int8_t  tRLOH_in_ns;
	int8_t  tRHOH_in_ns;
};

/* Converts time in nanoseconds to cycles. */
static inline unsigned int ns_to_cycles(unsigned int time,
			unsigned int period, unsigned int min)
{
	unsigned int k;

	k = (time + period - 1) / period;
	return max(k, min);
}

static void set_gpmi_nand_timing(unsigned long clock_frequency_in_hz)
{
	struct mxs_gpmi_regs *gpmi_regs =
		(struct mxs_gpmi_regs *)MXS_GPMI_BASE;

	// Samsung K9F1G08U0C
	const struct nand_timing  nand_timing = {
		.data_setup_in_ns        = 12,
		.data_hold_in_ns         = 5,
		.address_setup_in_ns     = 12,
		.gpmi_sample_delay_in_ns =  6,
		.tREA_in_ns              = 20,
		.tRLOH_in_ns             = 5,
		.tRHOH_in_ns             = 15,
	};

	unsigned int clock_period_in_ns = 1000000000L / clock_frequency_in_hz;
	unsigned int data_setup_in_cycles = ns_to_cycles(nand_timing.data_setup_in_ns,
							clock_period_in_ns, 1);
	unsigned int data_hold_in_cycles = ns_to_cycles(nand_timing.data_hold_in_ns,
							clock_period_in_ns, 1);
	unsigned int address_setup_in_cycles = ns_to_cycles(nand_timing.address_setup_in_ns,
							clock_period_in_ns, 0);

	writel((address_setup_in_cycles<<16)|
		(data_hold_in_cycles<<8)|(data_setup_in_cycles), 
		&gpmi_regs->hw_gpmi_timing0);
}

static void setup_gpmi_nand(void)
{
	struct mxc_ccm_reg *mxc_ccm = (struct mxc_ccm_reg *)CCM_BASE_ADDR;

	/* config gpmi nand iomux */
	imx_iomux_v3_setup_multiple_pads(nfc_pads, ARRAY_SIZE(nfc_pads));

	/* Enable clock roots */
	enable_usdhc_clk(1, 3);
	enable_usdhc_clk(1, 4);

	// GPMICLK select PLL2 307M, divided by 1 x 2
	setup_gpmi_io_clk(MXC_CCM_CS2CDR_ENFC_CLK_PODF(0x1) |
			  MXC_CCM_CS2CDR_ENFC_CLK_PRED(1)   |
			  MXC_CCM_CS2CDR_ENFC_CLK_SEL(0));

	/* enable apbh clock gating */
	setbits_le32(&mxc_ccm->CCGR0, MXC_CCM_CCGR0_APBHDMA_MASK);
	set_gpmi_nand_timing(307000000/2);
}
#endif

int board_early_init_f(void)
{
	setup_iomux_uart();
	return 0;
}

#ifdef CONFIG_SPL_BUILD
void board_init_f(ulong dummy)
{
	arch_cpu_init();

	/* Clear the BSS. */
	memset(__bss_start, 0, __bss_end - __bss_start);

	/* Set global data pointer. */
	gd = &gdata;

	board_early_init_f();

#ifdef CONFIG_BOOT_SPI
	board_boot();
#else
	board_init_r(NULL, 0);
#endif
}

void spl_board_init(void)
{
	get_clocks();
#ifdef CONFIG_SPL_SERIAL_SUPPORT
	preloader_console_init();
#endif

#ifdef CONFIG_SPL_TLB_ADDRESS
	gd->arch.tlb_addr = CONFIG_SPL_TLB_ADDRESS;
	gd->arch.tlb_size = PGTABLE_SIZE;
	gd->bd->bi_dram[0].start = CONFIG_SYS_SDRAM_BASE;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_SIZE;
	enable_caches();
#endif
#ifdef CONFIG_MXC_SPI
	setup_spi();
#endif

#ifdef CONFIG_SPL_NAND_SUPPORT
	setup_gpmi_nand();
#endif
}

int spl_start_uboot(void)
{
	// start uboot when console has input
	mdelay(10);
	return tstc();
}

void reset_cpu(ulong addr)
{

}

#else

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
	MX6_PAD_GPIO_9__GPIO1_IO09        | MUX_PAD_CTRL(NO_PAD_CTRL),
	// interrupt
	MX6_PAD_GPIO_2__GPIO1_IO02        | MUX_PAD_CTRL(NO_PAD_CTRL),
	// reference clock
	MX6_PAD_GPIO_16__ENET_REF_CLK     | MUX_PAD_CTRL(NO_PAD_CTRL),
};

static void setup_iomux_enet(void)
{
	imx_iomux_v3_setup_multiple_pads(enet_pads, ARRAY_SIZE(enet_pads));
}

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
	gpio_direction_input(IMX_GPIO_NR(1, 2));

	// hard reset phy
	gpio_direction_output(IMX_GPIO_NR(1, 9), 0);
	mdelay(1);
	gpio_set_value(IMX_GPIO_NR(1, 9), 1);

	// enable clock
	enable_enet_clk(1);

	return cpu_eth_init(bis);
}

u32 get_board_rev(void)
{
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

int dram_init(void)
{
	gd->ram_size = get_ram_size((void *)PHYS_SDRAM, PHYS_SDRAM_SIZE);

	return 0;
}
#endif

