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

typedef struct pack_info{
	const char *name;
	uint32_t ldaddr;
	uint32_t size;
}pack_info_t;

#if defined(CONFIG_SPL_SPI_SUPPORT)
#include <spi_flash.h>
int sf_load_packimg(struct spi_flash *flash, uint32_t offs, char *name);
#endif

#if defined(CONFIG_SPL_MMC_SUPPORT)
#include <mmc.h>
int mmc_load_packimg(struct mmc *mmc, uint32_t offs_sector, pack_info_t *info);
#endif

#if defined(CONFIG_SPL_NAND_SUPPORT)
#include <nand.h>
int nand_packimg_read(nand_info_t *nand, uint32_t nand_off, uint32_t nand_size);
int nand_packimg_write(nand_info_t *nand, uint32_t nand_off, uint32_t nand_size, uint32_t mem_off, 
				  uint32_t mem_size, uint32_t max_copy);
#endif

#endif
