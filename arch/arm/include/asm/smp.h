#ifndef ARM_SMP_H
#define ARM_SMP_H

#define SMP_BOOT_DONE_SIGNATURE         0x10230927
#define SMP_START_LOAD_INITRD_SIGNATURE 0x12354623
#define SMP_ABORT_BOOT_SIGNATURE        0x78336325
#define SMP_SECONDARY_DIE_SIGNATURE     0x89118291

void imx_boot_secondary(void);
void imx_kill_secondary(void);
uint32_t imx_get_boot_arg(void);
void imx_set_boot_arg(uint32_t value);

#endif
