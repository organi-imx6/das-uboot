#ifndef ARM_SMP_H
#define ARM_SMP_H

#define SMP_BOOT_DONE_SIGNATURE 0x10230927
extern unsigned int smp_boot_done;
#define SMP_START_LOAD_INITRD_SIGNATURE 0x12354623
extern unsigned int smp_start_load_initrd;
extern unsigned int smp_loop_count;

void smp_boot(void);

#endif
