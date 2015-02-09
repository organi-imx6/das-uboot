#ifndef __WISEHMI_COMMON_CONFIG_H
#define __WISEHMI_COMMON_CONFIG_H

#ifdef CONFIG_BOOT_MMC
#define CONFIG_ENV_IS_IN_MMC
#define CONFIG_SYS_MMC_ENV_DEV	0
#define CONFIG_ENV_OFFSET       (6 * 64 * 1024)
#ifdef CONFIG_SPL_BUILD
#define CONFIG_SPL_MMC_SUPPORT
#define CONFIG_SPL_FAT_SUPPORT
#endif
#endif

#ifdef CONFIG_BOOT_NAND
#define CONFIG_ENV_IS_IN_NAND
#define CONFIG_ENV_OFFSET       (7 * 128 * 1024)
#define CONFIG_ENV_RANGE        (3 * 128 * 1024)
#ifdef CONFIG_SPL_BUILD
#define CONFIG_SPL_NAND_SUPPORT
#endif
#endif

#if !defined CONFIG_ENV_IS_IN_MMC && \
    !defined CONFIG_ENV_IS_IN_NAND && \
    !defined CONFIG_ENV_IS_IN_FAT && \
    !defined CONFIG_ENV_IS_IN_SPI_FLASH
#define CONFIG_ENV_IS_NOWHERE
#endif

#define CONFIG_MX6

#include "mx6_common.h"
#include <linux/sizes.h>

#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

#include <asm/arch/imx-regs.h>
#include <asm/imx-common/gpio.h>

#define CONFIG_CMDLINE_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG
#define CONFIG_REVISION_TAG

#define CONFIG_SYS_GENERIC_BOARD

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(10 * SZ_1M)

#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_BOARD_LATE_INIT

#define CONFIG_MXC_UART

#define CONFIG_CMD_FUSE
#ifdef CONFIG_CMD_FUSE
#define CONFIG_MXC_OCOTP
#endif

/* MMC Configs */
#define CONFIG_FSL_ESDHC
#define CONFIG_FSL_USDHC
#define CONFIG_SYS_FSL_ESDHC_ADDR  USDHC2_BASE_ADDR
#define CONFIG_SYS_FSL_USDHC_NUM   1

#define CONFIG_MMC
#define CONFIG_CMD_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_CMD_EXT2
#define CONFIG_CMD_FAT
#define CONFIG_DOS_PARTITION

#define CONFIG_CMD_PING
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_MII
#define CONFIG_CMD_NET
#define CONFIG_FEC_MXC
#define CONFIG_MII
#define IMX_FEC_BASE			ENET_BASE_ADDR
#define CONFIG_FEC_XCV_TYPE		RMII
#define CONFIG_ETHPRIME			"FEC"
#define CONFIG_FEC_MXC_PHYADDR		1

#define CONFIG_PHYLIB
#define CONFIG_PHY_ATHEROS

#define CONFIG_CMD_SF
#ifdef CONFIG_CMD_SF
#define CONFIG_SPI
#define CONFIG_SPI_FLASH
#define CONFIG_SPI_FLASH_STMICRO
#define CONFIG_MXC_SPI
#define CONFIG_SF_DEFAULT_BUS		1
#define CONFIG_SF_DEFAULT_CS		0
#define CONFIG_SF_DEFAULT_SPEED		20000000
#define CONFIG_SF_DEFAULT_MODE		SPI_MODE_0
#endif

#define CONFIG_CMD_NAND
#ifdef CONFIG_CMD_NAND
#define CONFIG_MTD_DEVICE
#define CONFIG_CMD_MTDPARTS
#define CONFIG_NAND_MXS
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_NAND_BASE		0x40000000
#define CONFIG_SYS_NAND_5_ADDR_CYCLE
#define CONFIG_SYS_NAND_ONFI_DETECTION

/* DMA stuff, needed for GPMI/MXS NAND support */
#if !defined(CONFIG_SPL_BUILD) || defined(CONFIG_SPL_NAND_SUPPORT)
#define CONFIG_APBH_DMA
#define CONFIG_APBH_DMA_BURST
#define CONFIG_APBH_DMA_BURST8
#endif

#endif

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_CONS_INDEX              1
#define CONFIG_BAUDRATE                115200

/* Command definition */
#include <config_cmd_default.h>

#define CONFIG_CMD_BOOTZ
#define CONFIG_CMD_SETEXPR
#undef CONFIG_CMD_IMLS

#define CONFIG_BOOTDELAY               3

#define CONFIG_LOADADDR                0x12000000
#define CONFIG_SYS_TEXT_BASE           0x17800000

#define CONFIG_EXTRA_ENV_SETTINGS \
	"image=" CONFIG_DEFAULT_KERNEL_FILE "\0" \
	"fdt_file=" CONFIG_DEFAULT_FDT_FILE "\0" \
	"fdt_addr=0x18000000\0" \
	"ethaddr=11:12:13:14:15:16\0" \
	"ipaddr=172.10.11.190\0" \
	"gateway=172.10.0.1\0" \
	"netmask=255.255.0.0\0" \
	"serverip=172.10.11.15\0" \
	"nfsroot=/home/yuq/tmp/rootfs\0" \
	"console=" CONFIG_CONSOLE_DEV "\0" \
	"mmcdev=0\0" \
	"mmcpart=1\0" \
	"mmcroot=" CONFIG_MMCROOT " rootwait rw\0" \
	"mmcargs=setenv bootargs console=${console},${baudrate} root=${mmcroot}\0" \
	"loadimage=fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${image}\0" \
	"loadfdt=fatload mmc ${mmcdev}:${mmcpart} ${fdt_addr} ${fdt_file}\0" \
	"mmcboot=run mmcargs && bootm ${loadaddr} - ${fdt_addr}\0" \
    \
	"netargs=setenv bootargs console=${console},${baudrate} root=/dev/nfs rw " \
		"ip=${ipaddr}:${serverip}:${gateway}:${netmask}:wisehmi:eth0:off " \
	    "nfsroot=${serverip}:${nfsroot},v3,tcp\0" \
	"netboot=run netargs && tftp ${image} && tftp ${fdt_addr} ${fdt_file} && " \
		"bootm ${loadaddr} - ${fdt_addr}\0" \
	\
	"mtdids=nand0=gpmi-nand\0" \
    "mtdparts=mtdparts=gpmi-nand:128k(spl),768k(uboot),384k(env)," \
        "384k(dtb),7680k(kernel),-(rootfs)\0" \
    \
	"fl_spl=mw.b ${loadaddr} 0xff 0x400 && setexpr tmpvar ${filesize} + 0x400 && " \
        "nand erase.part spl && nand write ${loadaddr} 0 ${tmpvar}\0" \
	"fl_uboot=nand erase.part uboot && nand write ${loadaddr} uboot ${filesize}\0" \
    "fl_env=nand erase.part env && nand write ${loadaddr} env ${filesize}\0" \
	"fl_dtb=nand erase.part dtb && nand write ${loadaddr} dtb ${filesize}\0" \
	"fl_kernel=nand erase.part kernel && nand write ${loadaddr} kernel ${filesize}\0" \
    "fl_rootfs=nand erase.part rootfs && " \
	"  ubi part rootfs && " \
	"  ubi create rootfs && " \
	"  ubi write ${loadaddr} rootfs ${filesize}\0" \
	\
    "tf_spl=setexpr tmpvar ${loadaddr} + 0x400 && tftp ${tmpvar} SPL && run fl_spl\0" \
	"tf_uboot=tftp ${loadaddr} u-boot.img && run fl_uboot\0" \
	"tf_env=tftp ${loadaddr} wisehmi.env && run fl_env\0" \
	"tf_dtb=tftp ${loadaddr} ${fdt_file} && run fl_dtb\0" \
	"tf_kernel=tftp ${loadaddr} ${image} && run fl_kernel\0" \
	"tf_rootfs=tftp ${loadaddr} rootfs.img && run fl_rootfs\0" \
    \
	"mf_spl=fatload mmc 0 ${loadaddr} SPL && run fl_spl\0" \
	"mf_uboot=fatload mmc 0 ${loadaddr} u-boot.img && run fl_uboot\0" \
	"mf_env=fatload mmc 0 ${loadaddr} wisehmi.env && run fl_env\0" \
	"mf_dtb=fatload mmc 0 ${loadaddr} ${fdt_file} && run fl_dtb\0" \
	"mf_kernel=fatload mmc 0 ${loadaddr} ${image} && run fl_kernel\0" \
	"mf_rootfs=fatload mmc 0 ${loadaddr} rootfs.img && run fl_rootfs\0" \
	"mf_all=run mf_spl && run mf_uboot && run mf_dtb && run mf_kernel && run mf_rootfs\0" \
	\
	"sffile=SPL\0" \
	"burn_sf=tftp ${sffile} && sf probe && setexpr tmpvar ${filesize} + 0x400 && " \
        "sf erase 0 +${tmpvar} && sf write ${loadaddr} 0x400 ${filesize}\0"

#define CONFIG_BOOTCOMMAND \
	"mmc dev ${mmcdev};" \
	"if mmc rescan; then " \
		"if run loadbootscript; then " \
		"run bootscript; " \
		"else " \
			"if run loadimage; then " \
				"run mmcboot; " \
			"else run netboot; " \
			"fi; " \
		"fi; " \
	"else run netboot; fi"

#define CONFIG_ARP_TIMEOUT     200UL

/* Miscellaneous configurable options */
#define CONFIG_SYS_LONGHELP
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2     "> "
#define CONFIG_AUTO_COMPLETE
#define CONFIG_SYS_CBSIZE              256

/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS             16
#define CONFIG_SYS_BARGSIZE CONFIG_SYS_CBSIZE

#define CONFIG_SYS_MEMTEST_START       0x10000000
#define CONFIG_SYS_MEMTEST_END         0x10010000
#define CONFIG_SYS_MEMTEST_SCRATCH     0x10800000

#define CONFIG_SYS_LOAD_ADDR           CONFIG_LOADADDR

#define CONFIG_CMDLINE_EDITING
#define CONFIG_STACKSIZE               (128 * 1024)

/* Physical Memory Map */
#define CONFIG_NR_DRAM_BANKS           1
#define PHYS_SDRAM                     MMDC0_ARB_BASE_ADDR

#define CONFIG_SYS_SDRAM_BASE          PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR       IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE       IRAM_SIZE

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

/* FLASH and environment organization */
#define CONFIG_SYS_NO_FLASH

#define CONFIG_ENV_SIZE			(8 * 1024)

#ifndef CONFIG_SYS_DCACHE_OFF
#define CONFIG_CMD_CACHE
#endif

#ifndef CONFIG_SPL_BUILD
#define CONFIG_OF_LIBFDT
#define CONFIG_MXC_GPIO
#define CONFIG_CMD_BMODE
#endif

/* SPL */
#ifdef CONFIG_SPL

#include "imx6_spl.h"

#define CONFIG_SYS_SPL_ARGS_ADDR        CONFIG_SYS_SDRAM_BASE + 0x2000000

#define CONFIG_SPL_LOAD_SPLASH
#define CONFIG_SYS_SPL_SPLASH_ADDR      CONFIG_SYS_SDRAM_BASE + 0x3000000

#define CONFIG_SPL_OS_BOOT
#define CONFIG_SPL_BOARD_INIT

#if defined(CONFIG_SPL_MMC_SUPPORT)
#define CONFIG_SYS_MMCSD_RAW_MODE_KERNEL_SECTOR	4096  /* offset 2M */
#define CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTOR	2048  /* offset 1M */
#define CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTORS	(CONFIG_FDT_FILE_SIZE/512)
#endif

#if defined(CONFIG_SPL_FAT_SUPPORT) || defined(CONFIG_SPL_EXT_SUPPORT)
#define CONFIG_SPL_FAT_LOAD_KERNEL_NAME CONFIG_DEFAULT_KERNEL_FILE
#define CONFIG_SPL_FAT_LOAD_ARGS_NAME   CONFIG_DEFAULT_FDT_FILE
#define CONFIG_SPL_FAT_LOAD_SPLASH_NAME CONFIG_DEFAULT_SPLASH_FILE
#endif

#if defined(CONFIG_SPL_NAND_SUPPORT)
#define CONFIG_CMD_SPL_NAND_OFS         (10 * 128 * 1024)
#define CONFIG_CMD_SPL_WRITE_SIZE       CONFIG_FDT_FILE_SIZE
#define CONFIG_SYS_NAND_SPL_KERNEL_OFFS (13 * 128 * 1024)
#define CONFIG_SYS_NAND_U_BOOT_OFFS     (1 * 128 * 1024)
#endif

#endif /* CONFIG_SPL */

#endif                         /* __WISEHMI_COMMON_CONFIG_H */
