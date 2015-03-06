#ifndef __RBCTRL_CONFIG_H
#define __RBCTRL_CONFIG_H

#define CONFIG_MACH_TYPE	3529
#define CONFIG_MXC_UART_BASE	UART2_BASE
#define CONFIG_CONSOLE_DEV		"ttymxc1"

#define CONFIG_DEFAULT_KERNEL_FILE "uImage"
#define CONFIG_DEFAULT_FDT_FILE "rbctrl.dtb"
#define PHYS_SDRAM_SIZE		(CONFIG_DDR_MB * 1024 * 1024)

#include "rbctrl_common.h"

#endif /* __WISEHMI_CONFIG_H */
