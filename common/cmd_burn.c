/*
 * Copyright 2000-2009
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
void mxc_serial_burn_putCmd(const char *s, int len);

/*
 * setting information
 */
typedef struct {
	uint8_t remain1: 1,
		closeReq: 1,
		remain2: 1,
		tripClear: 1,
		remain3: 4;
	uint32_t dateTime;
}__attribute__((packed)) SettingsInfo;

/*
 * frame sent by A20
 */
typedef struct {
	uint8_t head;
	uint8_t length;
	uint8_t frameTpyes;
	SettingsInfo settingsInfo;
	uint16_t crc;
	uint8_t tail;
}__attribute__((packed)) UpFrame;

const uint16_t crc16_half_byte[16]= {
	0x0000, 0x1021, 0x2042, 0x3063,
	0x4084, 0x50a5, 0x60c6, 0x70e7,
	0x8108, 0x9129, 0xa14a, 0xb16b,
	0xc18c, 0xd1ad, 0xe1ce, 0xf1ef };

uint16_t crc16Check(uint8_t *ptr,uint8_t len)
{
	uint8_t da = 0;
	uint16_t crc = 0;
	while(len-- != 0)
	{
		da = ((uint8_t)(crc/256))/16;
		crc <<= 4;
		crc ^= crc16_half_byte[da^(*ptr/16)];
		da = ((uint8_t)(crc/256)/16);
		crc <<= 4;
		crc ^= crc16_half_byte[da^(*ptr&0x0f)];
		ptr++;
	}
	return(crc);
}

static int do_burn(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	
	UpFrame upFrame;
	memset(&upFrame, 0, sizeof(UpFrame));

	upFrame.head = 0xAA;
	upFrame.length = sizeof(SettingsInfo) + 2;
	upFrame.frameTpyes = 0;
	upFrame.settingsInfo.closeReq = 1;
	upFrame.settingsInfo.dateTime = 0;
	upFrame.settingsInfo.tripClear = 0;
	upFrame.crc = crc16Check((uint8_t *)(&(upFrame.settingsInfo)), sizeof(SettingsInfo));
	upFrame.tail = 0x55;
	
	mxc_serial_burn_putCmd((const char *)&upFrame, sizeof(UpFrame));
	return 0;
}

U_BOOT_CMD(
	burn,	CONFIG_SYS_MAXARGS,	1,	do_burn,
	"echo args to console",
	"[args..]\n"
	"    - echo args to console; \\c suppresses newline"
);
