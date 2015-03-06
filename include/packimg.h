#ifndef _PACKIMG_H
#define _PACKIMG_H

#include <common.h>

#define PACK_MAGIC 0x4b434150
#define PACK_NAME_MAX 32

struct pack_header {
	uint32_t magic;
	uint32_t nentry;
	uint32_t crc;
};

struct pack_entry {
	uint32_t offset;
	uint32_t size;
	uint32_t ldaddr;
	uint32_t crc;
	char name[PACK_NAME_MAX];
};

#ifdef CONFIG_SPL_BUILD
#include <spi_flash.h>
/*
	failed: return -1
	success: return 0, name==NULL or can't find
			return <name>@address
*/
int sf_load_packimg(struct spi_flash *flash, uint32_t offs, char *name);
#else
#ifdef CONFIG_NAND_PACKIMG
#include <nand.h>
int nand_packimg_read(nand_info_t *nand, uint32_t nand_off, uint32_t nand_size);
int nand_packimg_write(nand_info_t *nand, uint32_t nand_off, uint32_t nand_size, uint32_t mem_off, 
				  uint32_t mem_size, uint32_t max_copy);
#endif
#endif //#ifdef CONFIG_SPL_BUILD

#endif
