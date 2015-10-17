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
#define CONFIG_FDT_FILE_SIZE 40960
#define CONFIG_DEFAULT_SPLASH_FILE "splash.bin"
#define CONFIG_DEFAULT_KERNEL_FILE "zImage"
#define CONFIG_DEFAULT_INITRD_FILE "initramfs.cpio"
#define CONFIG_MMCROOT			"/dev/mmcblk0p2"
#define PHYS_SDRAM_SIZE		(CONFIG_DDR_MB * 1024 * 1024)

#define CONFIG_IMAGE_FORMAT_LEGACY

#include "wisehmi_common.h"

#endif /* __WISEHMI_CONFIG_H */
