#ifndef __RBCTRL_CONFIG_H
#define __RBCTRL_CONFIG_H

#define CONFIG_MACH_TYPE	3529
#define CONFIG_MXC_UART_BASE	UART2_BASE
#define CONFIG_CONSOLE_DEV		"ttymxc1"
#if defined CONFIG_MX6Q
#define CONFIG_DEFAULT_FDT_FILE	"imx6q-rbctrl.dtb"
#elif defined CONFIG_MX6DL
#define CONFIG_DEFAULT_FDT_FILE	"imx6dl-rbctrl.dtb"
#endif
#define CONFIG_FDT_FILE_SIZE 40960
#define CONFIG_DEFAULT_SPLASH_FILE "splash.bin"
#define CONFIG_DEFAULT_KERNEL_FILE "uImage"
#define CONFIG_DEFAULT_INITRD_FILE "initramfs.cpio"
#define CONFIG_MMCROOT			"/dev/mmcblk0p2"
#define PHYS_SDRAM_SIZE		(CONFIG_DDR_MB * 1024 * 1024)

#include "rbctrl_common.h"

#endif /* __WISEHMI_CONFIG_H */
