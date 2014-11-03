#ifndef __WISEHMI_CONFIG_H
#define __WISEHMI_CONFIG_H

#define CONFIG_MACH_TYPE	3529
#define CONFIG_MXC_UART_BASE	UART4_BASE
#define CONFIG_CONSOLE_DEV		"ttymxc3"
#if defined CONFIG_MX6Q
#define CONFIG_DEFAULT_FDT_FILE	"imx6q-wisehmi.dtb"
#elif defined CONFIG_MX6DL
#define CONFIG_DEFAULT_FDT_FILE	"imx6dl-wisehmi.dtb"
#endif
#define CONFIG_DEFAULT_KERNEL_FILE "uImage"
#define CONFIG_MMCROOT			"/dev/mmcblk0p2"
#define PHYS_SDRAM_SIZE		(1u * 1024 * 1024 * 1024)

#ifndef CONFIG_SPL_BUILD
/* USB Configs */
#define CONFIG_CMD_USB
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_MX6
#define CONFIG_USB_STORAGE
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_ETHER_ASIX
#define CONFIG_USB_MAX_CONTROLLER_COUNT 2
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET	/* For OTG port */
#define CONFIG_MXC_USB_PORTSC	(PORT_PTS_UTMI | PORT_PTS_PTW)
#define CONFIG_MXC_USB_FLAGS	0

/* I2C Configs */
#define CONFIG_CMD_I2C
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_I2C_SPEED		100000
#endif

#ifdef CONFIG_SPL
#define CONFIG_SPL_MMC_SUPPORT
#define CONFIG_SPL_FAT_SUPPORT
#endif

#include "wisehmi_common.h"

#define CONFIG_SYS_FSL_USDHC_NUM	2
#if defined(CONFIG_ENV_IS_IN_MMC)
#define CONFIG_SYS_MMC_ENV_DEV		0
#endif

#endif /* __WISEHMI_CONFIG_H */
