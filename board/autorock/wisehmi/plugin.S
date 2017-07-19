/*
 * Copyright (C) 2012-2013 Freescale Semiconductor, Inc.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <config.h>

/* DDR script */
.macro imx6dqsabresd_ddr_setting
	ldr r0, =IOMUXC_BASE_ADDR
	ldr r1, =0x000C0000
	str r1, [r0, #0x798]
	ldr r1, =0x00000000
	str r1, [r0, #0x758]

	ldr r1, =0x00000030
	str r1, [r0, #0x588]
	str r1, [r0, #0x594]
	str r1, [r0, #0x56c]
	str r1, [r0, #0x578]
	str r1, [r0, #0x74c]
	str r1, [r0, #0x57c]

	ldr r1, =0x00000000
	str r1, [r0, #0x58c]

	ldr r1, =0x00000028
	str r1, [r0, #0x59c]
	str r1, [r0, #0x5a0]
	str r1, [r0, #0x78c]

	ldr r1, =0x00020000
	str r1, [r0, #0x750]

	ldr r1, =0x00000028
	str r1, [r0, #0x5a8]
	str r1, [r0, #0x5b0]
	str r1, [r0, #0x524]
	str r1, [r0, #0x51c]
	str r1, [r0, #0x518]
	str r1, [r0, #0x50c]
	str r1, [r0, #0x5b8]
	str r1, [r0, #0x5c0]

	ldr r1, =0x00020000
	str r1, [r0, #0x774]

	ldr r1, =0x00000028
	str r1, [r0, #0x784]
	str r1, [r0, #0x788]
	str r1, [r0, #0x794]
	str r1, [r0, #0x79c]
	str r1, [r0, #0x7a0]
	str r1, [r0, #0x7a4]
	str r1, [r0, #0x7a8]
	str r1, [r0, #0x748]
	str r1, [r0, #0x5ac]
	str r1, [r0, #0x5b4]
	str r1, [r0, #0x528]
	str r1, [r0, #0x520]
	str r1, [r0, #0x514]
	str r1, [r0, #0x510]
	str r1, [r0, #0x5bc]
	str r1, [r0, #0x5c4]

	ldr r0, =MMDC_P0_BASE_ADDR
	ldr r2, =0xA1390003
	str r2, [r0, #0x800]

	//ldr r2, =0x0058005F
	ldr r2, =0x001F001D
	str r2, [r0, #0x80c]
	//ldr r2, =0x005B0056
	ldr r2, =0x0024001D
	str r2, [r0, #0x810]
	//ldr r1, =MMDC_P1_BASE_ADDR//wanggang
	ldr r1, =0x021b4000
	//ldr r2, =0x002F0043
	ldr r2, =0x001D0027
	str r2, [r1, #0x80c]
	//ldr r2, =0x00180023
	ldr r2, =0x00120024
	str r2, [r1, #0x810]

	ldr r2, =0x03340344
	str r2, [r0, #0x83c]
	ldr r2, =0x03300324
	str r2, [r0, #0x840]

	ldr r2, =0x032C033C
	str r2, [r1, #0x83c]
	ldr r2, =0x03300304
	str r2, [r1, #0x840]

	ldr r2, =0x3A2E3032
	str r2, [r0, #0x848]
	ldr r2, =0x38342E46
	str r2, [r1, #0x848]

	ldr r2, =0x32383E38
	str r2, [r0, #0x850]
	ldr r2, =0x4432463A
	str r2, [r1, #0x850]

	ldr r2, =0x33333333
	str r2, [r0, #0x81c]
	str r2, [r0, #0x820]
	str r2, [r0, #0x824]
	str r2, [r0, #0x828]
	str r2, [r1, #0x81c]
	str r2, [r1, #0x820]
	str r2, [r1, #0x824]
	str r2, [r1, #0x828]

	ldr r2, =0x00000800
	str r2, [r0, #0x8b8]
	str r2, [r1, #0x8b8]

	ldr r2, =0x00020036
	str r2, [r0, #0x004]
	ldr r2, =0x09444040
	str r2, [r0, #0x008]

	ldr r2, =0x8A8F7955
	str r2, [r0, #0x00c]
	ldr r2, =0xFF328F64
	str r2, [r0, #0x010]

	ldr r2, =0x01FF00DB
	str r2, [r0, #0x014]
	ldr r2, =0x00011740
	str r2, [r0, #0x018]

	ldr r2, =0x00008000
	str r2, [r0, #0x01c]
	ldr r2, =0x000026D2
	str r2, [r0, #0x02c]
	ldr r2, =0x008F1023
	str r2, [r0, #0x030]
	ldr r2, =0x00000047
	str r2, [r0, #0x040]

	ldr r2, =0x841A0000
	str r2, [r0, #0x000]

	ldr r2, =0x04088032
	str r2, [r0, #0x01c]
	ldr r2, =0x00008033
	str r2, [r0, #0x01c]
	ldr r2, =0x00048031
	str r2, [r0, #0x01c]
	ldr r2, =0x09408030
	str r2, [r0, #0x01c]
	ldr r2, = 0x04008040
	str r2, [r0, #0x01c]

	ldr r2, = 0x00005800
	str r2, [r0, #0x020]
	ldr r2, =0x00011117
	str r2, [r0, #0x818]
	ldr r2, =0x00011117
	str r2, [r1, #0x818]
	ldr r2, =0x00025576
	str r2, [r0, #0x004]
	ldr r2, =0x00011006
	str r2, [r0, #0x404]
	ldr r2, =0x00000000
	str r2, [r0, #0x01c]




.endm

.macro imx6dlsabresd_ddr_setting
	ldr r0, =IOMUXC_BASE_ADDR
	ldr r1, =0x000c0000
	str r1, [r0, #0x774]

	ldr r1, =0x00000000
	str r1, [r0, #0x754]


	ldr r1, =0x00000030
	str r1, [r0, #0x4ac]
	str r1, [r0, #0x4b0]
	str r1, [r0, #0x464]
	str r1, [r0, #0x490]
	str r1, [r0, #0x74c]
	str r1, [r0, #0x494]


	ldr r1, =0x00000000
	str r1, [r0, #0x4a0]

	ldr r1, =0x00000030
	str r1, [r0, #0x4b4]
	str r1, [r0, #0x4b8]
	str r1, [r0, #0x76c]


	ldr r1, =0x00020000
	str r1, [r0, #0x750]


	ldr r1, =0x00000028
	str r1, [r0, #0x4bc]
	str r1, [r0, #0x4c0]
	str r1, [r0, #0x4c4]
	str r1, [r0, #0x4c8]
	str r1, [r0, #0x4cc]
	str r1, [r0, #0x4d0]
	str r1, [r0, #0x4d4]
	str r1, [r0, #0x4d8]


	ldr r1, =0x00020000
	str r1, [r0, #0x760]


	ldr r1, =0x00000028
	str r1, [r0, #0x764]
	str r1, [r0, #0x770]
	str r1, [r0, #0x778]
	str r1, [r0, #0x77c]
	str r1, [r0, #0x780]
	str r1, [r0, #0x784]
	str r1, [r0, #0x78c]
	str r1, [r0, #0x748]
	str r1, [r0, #0x470]
	str r1, [r0, #0x474]
	str r1, [r0, #0x478]
	str r1, [r0, #0x47c]
	str r1, [r0, #0x480]
	str r1, [r0, #0x484]
	str r1, [r0, #0x488]
	str r1, [r0, #0x48c]


	ldr r0, =MMDC_P0_BASE_ADDR
	ldr r2, =0xa1390003
	str r2, [r0, #0x800]


	ldr r2, =0x001f001f
	str r2, [r0, #0x80c]
	str r2, [r0, #0x810]
	ldr r1, =MMDC_P1_BASE_ADDR
	str r2, [r1, #0x80c]
	str r2, [r1, #0x810]

	ldr r2, =0x42190217
	str r2, [r0, #0x83c]


	ldr r2, =0x017B017B
	str r2, [r0, #0x840]
	str r2, [r0, #0x83c]


	ldr r2, =0x015F016C
	str r2, [r1, #0x840]


	ldr r2, =0x4C4C4D4C
	str r2, [r1, #0x848]


	ldr r2, =0x4A4D4C48
	str r2, [r0, #0x848]





	ldr r2, =0x3F3F3F40
	str r2, [r0, #0x850]


	ldr r2, =0x3538382E
	str r2, [r1, #0x850]


	ldr r2, =0x33333333
	str r2, [r0, #0x81c]
	str r2, [r0, #0x820]
	str r2, [r0, #0x824]
	str r2, [r0, #0x828]
	str r2, [r1, #0x81c]
	str r2, [r1, #0x820]
	str r2, [r1, #0x824]
	str r2, [r1, #0x828]


	ldr r2, =0x00000800
	str r2, [r0, #0x8b8]
	str r2, [r1, #0x8b8]


	ldr r2, =0x00020025
	str r2, [r0, #0x004]


	ldr r2, =0x00333030
	str r2, [r0, #0x008]


	ldr r2, =0x676B5313
	str r2, [r0, #0x00c]


	ldr r2, =0xB66E8B63
	str r2, [r0, #0x010]


	ldr r2, =0x01FF00DB
	str r2, [r0, #0x014]


	ldr r2, =0x00001740
	str r2, [r0, #0x018]


	ldr r2, =0x00008000
	str r2, [r0, #0x01c]

	ldr r2, =0x000026d2
	str r2, [r0, #0x02c]


	ldr r2, =0x00431023
	str r2, [r0, #0x030]


	ldr r2, =0x00000047
	str r2, [r0, #0x040]


	ldr r2, =0x83190000
	str r2, [r0, #0x000]


	ldr r2, =0x04008032
	str r2, [r0, #0x01c]


	ldr r2, =0x00008033
	str r2, [r0, #0x01c]

	ldr r2, =0x00048031
	str r2, [r0, #0x01c]


	ldr r2, =0x05208030
	str r2, [r0, #0x01c]

	ldr r2, =0x04008040
	str r2, [r0, #0x01c]


	ldr r2, =0x00005800
	str r2, [r0, #0x020]

	ldr r2, =0x00011117
	str r2, [r0, #0x818]
	str r2, [r1, #0x818]


	ldr r2, =0x00025565
	str r2, [r0, #0x004]

	ldr r2, =0x00011006
	str r2, [r0, #0x404]

	ldr r2, =0x00000000
	str r2, [r0, #0x01c]

.endm

.macro imx6solosabresd_ddr_setting
	ldr r0, =IOMUXC_BASE_ADDR
	ldr r1, =0x000c0000
	str r1, [r0, #0x774]
	ldr r1, =0x00000000
	str r1, [r0, #0x754]

	ldr r1, =0x00000028
	str r1, [r0, #0x4ac]
	str r1, [r0, #0x4b0]
	str r1, [r0, #0x464]
	str r1, [r0, #0x490]
	str r1, [r0, #0x74c]
	str r1, [r0, #0x494]

	ldr r1, =0x00000000
	str r1, [r0, #0x4a0]

	ldr r1, =0x00000028
	str r1, [r0, #0x4b4]
	str r1, [r0, #0x4b8]
	str r1, [r0, #0x76c]

	ldr r1, =0x00020000
	str r1, [r0, #0x750]

	ldr r1, =0x00000028
	str r1, [r0, #0x4bc]
	str r1, [r0, #0x4c0]
	str r1, [r0, #0x4c4]
	str r1, [r0, #0x4c8]

	ldr r1, =0x00020000
	str r1, [r0, #0x760]

	ldr r1, =0x00000028
	str r1, [r0, #0x764]
	str r1, [r0, #0x770]
	str r1, [r0, #0x778]
	str r1, [r0, #0x77c]
	str r1, [r0, #0x470]
	str r1, [r0, #0x474]
	str r1, [r0, #0x478]
	str r1, [r0, #0x47c]

	ldr r0, =MMDC_P0_BASE_ADDR
	ldr r2, =0xA1390003
	str r2, [r0, #0x800]

	ldr r2, =0x00580055
	str r2, [r0, #0x80c]
	ldr r2, =0x00500059
	str r2, [r0, #0x810]

	ldr r2, =0x02540254
	str r2, [r0, #0x83c]
	ldr r2, =0x0238023C
	str r2, [r0, #0x840]

	ldr r2, =0x40464846
	str r2, [r0, #0x848]

	ldr r2, =0x38322C32
	str r2, [r0, #0x850]

	ldr r2, =0x33333333
	str r2, [r0, #0x81c]
	str r2, [r0, #0x820]
	str r2, [r0, #0x824]
	str r2, [r0, #0x828]

	ldr r2, =0x00000800
	str r2, [r0, #0x8b8]
	str r2, [r0, #0x8b8]

	ldr r2, =0x00020036
	str r2, [r0, #0x004]
	ldr r2, =0x09444040
	str r2, [r0, #0x008]

	ldr r2, =0x898E7975
	str r2, [r0, #0x00c]
	ldr r2, =0xFF528F64
	str r2, [r0, #0x010]

	ldr r2, =0x01FF00DB
	str r2, [r0, #0x014]
	ldr r2, =0x00011740
	str r2, [r0, #0x018]

	ldr r2, =0x00008000
	str r2, [r0, #0x01c]
	ldr r2, =0x000026D2
	str r2, [r0, #0x02c]
	ldr r2, =0x008E1023
	str r2, [r0, #0x030]
	ldr r2, =0x00000027
	str r2, [r0, #0x040]

	ldr r2, =0x84190000
	str r2, [r0, #0x000]

	ldr r2, =0x02088032
	str r2, [r0, #0x01c]
	ldr r2, =0x00008033
	str r2, [r0, #0x01c]
	ldr r2, =0x00048031
	str r2, [r0, #0x01c]
	ldr r2, =0x19408030
	str r2, [r0, #0x01c]
	ldr r2, =0x04008040
	str r2, [r0, #0x01c]

	ldr r2, =0x00007800
	str r2, [r0, #0x020]
	ldr r2, =0x00022227
	str r2, [r0, #0x818]
	ldr r2, =0x00025576
	str r2, [r0, #0x004]
	ldr r2, =0x00011006
	str r2, [r0, #0x404]
	ldr r2, =0x00000000
	str r2, [r0, #0x01c]
.endm
.macro imx6_clock_gating
	ldr r0, =CCM_BASE_ADDR
	ldr r1, =0x00C03F3F
	str r1, [r0, #0x068]
	ldr r1, =0x0030FC03
	str r1, [r0, #0x06c]
	ldr r1, =0x0FFFC000
	str r1, [r0, #0x070]
	ldr r1, =0x3FF00000
	str r1, [r0, #0x074]
	ldr r1, =0x00FFF300
	str r1, [r0, #0x078]
	ldr r1, =0x0F0000C3
	str r1, [r0, #0x07c]
	ldr r1, =0x000003FF
	str r1, [r0, #0x080]
	
	 /* Init the DDR according the init script */
    ldr r0, =CCM_BASE_ADDR
    /* select 528MHz for pre_periph_clk_sel */
    ldr r1, [r0,#0x18]
    ldr r3, [r0,#0x18]
 
   and r1, r1, #0xffffcfff
    orr r1, r1, #0x00001000
    str r1, [r0,#0x18]
    
    ldr r1, [r0,#0x14]
    ldr r2, [r0,#0x14]
    orr r1, r1, #0x02000000    
    str r1, [r0,#0x14]
    
    ldr r0, =0x020c8000
    ldr r1, [r0,#0x30]
    orr r1, r1, #0x0001

    ldr r1, =0x00001770  //0x00001770 0x0000059E(30Mhz spectrum spread range)  0x00001c18 (6Mhz)
    str r1, [r0,#0x60] 
    
    ldr r1, =0x05dc8006  //0x05dc8006 0x0bb88006(24kHz) , 0x07068006(40Khz)
    str r1, [r0,#0x40] 

/*
     ldr r1, =0x00000960
     str r1, [r0,#0x60] 
     ldr r1, =0x01f48006  //5mhz
     str r1, [r0,#0x40] 
*/
     ldr r1, [r0,#0x30]
    and r1, r1, #0xFFFEFFFF
    str r1, [r0,#0x30]
  // Delay 300ns at least
    ldr r4, =0x0
pu_delay:
    add r4, r4, #0x1
    cmp r4, #0x200000
    bne pu_delay    
    
    ldr r0, =CCM_BASE_ADDR
    str r2, [r0,#0x14]
    str r3, [r0,#0x18]
.endm

.macro imx6_qos_setting
	ldr r0, =IOMUXC_BASE_ADDR
	ldr r1, =0xF00000CF
	str r1, [r0, #0x10]

	ldr r1, =0x007F007F
	str r1, [r0, #0x18]
	str r1, [r0, #0x1c]
.endm

.macro imx6_ddr_setting
#if defined (CONFIG_MX6SOLO)
	imx6solosabresd_ddr_setting
#elif defined (CONFIG_MX6DL)
	imx6dlsabresd_ddr_setting
#elif defined (CONFIG_MX6Q)
	imx6dqsabresd_ddr_setting
#else
	#error "SOC not configured"
#endif

.endm

/* include the common plugin code here */
#include <asm/arch/mx6_plugin.S>
