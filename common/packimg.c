#include <common.h>
#include <packimg.h>

static uint32_t calc_crc(void *buff, int size)
{
	int i;
	uint32_t ret = 0;
	for (i = 0; i < size; i += 4)
		ret += *(uint32_t *)(buff + i);
	return ret;
}

#define MAX_PACK_ENTRY	8

#if defined(CONFIG_SPL_SPI_SUPPORT)
#include <spi_flash.h>
/*
	failed: return -1
	success: return 0, name==NULL or can't find
			return <name>@address
*/
int sf_load_packimg(struct spi_flash *flash, uint32_t offs, char *name)
{
	struct pack_header *ph;
	struct pack_entry *pe;
	uint8_t buf[sizeof(*ph)+sizeof(*pe)*MAX_PACK_ENTRY];
	uint32_t crc;
	int ret, i;

	ret = spi_flash_read(flash, offs, sizeof(buf), buf);
	if(ret<0){
		printf("%s: load head failed\n", __FUNCTION__);
		return ret;
	}
	ph = (struct pack_header *)buf;
	pe = (struct pack_entry *)(ph+1);

	// check valid header
	if (ph->magic != PACK_MAGIC){
		printf("load packimg from 0x%x fail\n", offs);
		return -1;
	}
	offs+=ret;
	crc = calc_crc(pe, ph->nentry * sizeof(*pe));
	if (ph->crc != crc){
		printf("packimg head crc error 0x%x should be 0x%x\n", ph->crc, crc);
		return -1;
	}
	if(ph->nentry>MAX_PACK_ENTRY){
		printf("Warning packimg entry %d bigger than %d\n", ph->nentry, MAX_PACK_ENTRY);
		ph->nentry = MAX_PACK_ENTRY;
	}

	// load all entries
	for (i = 0; i < ph->nentry; i++){
		if(name && strcmp(name, pe[i].name)==0) //find target
			ret = pe[i].ldaddr;
		
		debug("load %s@0x%x to ram 0x%x\n", pe[i].name, offs+pe[i].offset, pe[i].ldaddr);
		spi_flash_read(flash, offs+pe[i].offset, pe[i].size, (void *)pe[i].ldaddr);
		crc = calc_crc((void *)pe[i].ldaddr, pe[i].size);
		if (pe[i].crc != crc){
			printf("packimg data crc error 0x%x should be 0x%x\n", pe[i].crc, crc);
			return -1;
		}
	}
	debug("load packimg success\n");
	return ret;
}
#endif //#if defined(CONFIG_SPL_SPI_SUPPORT)

#if defined(CONFIG_SPL_MMC_SUPPORT)

#include <mmc.h>
#define ROUND_UP(n,log2)	(((n) + (1<<(log2)) - 1) >> (log2))

int mmc_load_packimg(struct mmc *mmc, uint32_t offs_sector, pack_info_t *info)
{
	struct pack_header *ph;
	struct pack_entry *pe;
	uint8_t *buf = (uint8_t *)CONFIG_SYS_TEXT_BASE-4096;
	uint32_t crc, size;
	int ret, i,j;

	size = ROUND_UP(sizeof(buf),mmc->block_dev.log2blksz);
	ret = mmc->block_dev.block_read(0, offs_sector, size, buf);
	if(ret<0){
		printf("%s: load head failed\n", __FUNCTION__);
		return ret;
	}
	ph = (struct pack_header *)buf;
	pe = (struct pack_entry *)(ph+1);

	// check valid header
	if (ph->magic != PACK_MAGIC){
		printf("mmc load packimg from sector 0x%x fail\n", offs_sector);
		return -1;
	}

	crc = calc_crc(pe, ph->nentry * sizeof(*pe));
	if (ph->crc != crc){
		printf("packimg head crc error 0x%x should be 0x%x\n", ph->crc, crc);
		return -1;
	}
	if(ph->nentry>MAX_PACK_ENTRY){
		printf("Warning packimg entry %d bigger than %d\n", ph->nentry, MAX_PACK_ENTRY);
		ph->nentry = MAX_PACK_ENTRY;
	}

	// load all entries
	for (i = 0; i < ph->nentry; i++){
		for(j=0; info[j].name; j++){
			if(strcmp(info[j].name, pe[i].name)==0){ //match name
				info[j].ldaddr = pe[i].ldaddr;
				info[j].size = pe[i].size;
				printf("find %s @ 0x%x(size=0x%x)\n", info[j].name, info[j].ldaddr, info[j].size);
			}
		}

		debug("load %s@0x%x to ram 0x%x\n", pe[i].name, 
			offs_sector+(pe[i].offset>>mmc->block_dev.log2blksz), pe[i].ldaddr);
		mmc->block_dev.block_read(0, offs_sector+(pe[i].offset>>mmc->block_dev.log2blksz), 
			ROUND_UP(pe[i].size, mmc->block_dev.log2blksz), (void *)pe[i].ldaddr);
		crc = calc_crc((void *)pe[i].ldaddr, pe[i].size);
		if (pe[i].crc != crc){
			printf("packimg data crc error 0x%x should be 0x%x\n", pe[i].crc, crc);
			return -1;
		}
	}
	debug("load packimg success\n");
	return 0;
}
#endif //#if defined(CONFIG_SPL_MMC_SUPPORT)

#if defined(CONFIG_SPL_NAND_SUPPORT)
#include <nand.h>
int nand_packimg_read(nand_info_t *nand, uint32_t nand_off, uint32_t nand_size)
{
	int i, err;
	size_t size;
	char buff[512];
	struct pack_header *ph;
	struct pack_entry *pe;
	uint32_t offs = nand_off, crc;

	if ((nand_off & (nand->erasesize - 1)) || (nand_size & (nand->erasesize - 1))) {
		printf("offset %x and size %x must be block aligned\n", nand_off, nand_size);
		return -1;
	}

	while (offs < nand_off + nand_size) {
		size = 512;
		err = nand_read_skip_bad(nand, offs, &size, NULL, nand->size, (void *)buff);
		if (err) {
			printf("nand read offset %x fail\n", offs);
			return err;
		}
		ph = (void *)buff;
		pe = (void *)buff + sizeof(*ph);

		// check valid header
		if (ph->magic != PACK_MAGIC)
			goto next_block;
		crc = calc_crc(pe, ph->nentry * sizeof(*pe));
		if (ph->crc != crc)
			goto next_block;

		// load all entries
		for (i = 0; i < ph->nentry; i++) {
			size = pe[i].size;
			err = nand_read_skip_bad(nand, offs + pe[i].offset, &size, NULL, nand->size, (void *)pe[i].ldaddr);
			if (err) {
				printf("nand read offset %x size %x to %x fail\n",
					   offs + pe[i].offset, pe[i].size, pe[i].ldaddr);
				return err;
			}
			crc = calc_crc((void *)pe[i].ldaddr, pe[i].size);
			if (pe[i].crc != crc)
				goto next_block;
		}

		printf("load packimg at %x success\n", offs);
		return 0;

	next_block:
		printf("invalid packimg at offset %x\n", offs);
		offs += nand->erasesize;
	}

	printf("load packimg from %x to %x fail\n", nand_off, nand_size);
	return -1;
}

int nand_packimg_write(nand_info_t *nand, uint32_t nand_off, uint32_t nand_size, uint32_t mem_off, 
				  uint32_t mem_size, uint32_t max_copy)
{
	int err;
	int nsize = nand_size, msize, copies = 0;
	uint32_t noffs = nand_off, moffs;

	if ((nand_off & (nand->erasesize - 1)) || (nand_size & (nand->erasesize - 1))) {
		printf("offset %x and size %x must be block aligned\n", nand_off, nand_size);
		return -1;
	}

	while (nsize >= mem_size && copies < max_copy) {
		moffs = mem_off;
		msize = mem_size;

		while (msize > 0) {
			uint32_t size;
			while (nand_block_isbad(nand, noffs)) {
				noffs += nand->erasesize;
				nsize -= nand->erasesize;
				if (nsize < msize)
					goto out;
			}

			err = nand_erase(nand, noffs, nand->erasesize);
			if (err) {
				printf("erase block %x fail\n", noffs);
				goto out;
			}

			size = msize > nand->erasesize ? nand->erasesize : msize;
			err = nand_write(nand, noffs, &size, (void *)moffs);
			if (err) {
				printf("write block %x fail\n", noffs);
				goto out;
			}

			moffs += nand->erasesize;
			msize -= nand->erasesize;
			noffs += nand->erasesize;
			nsize -= nand->erasesize;
		}

		copies++;
	}

out:
	if (copies) {
		printf("success write %d/%d copies of packimg to %x\n", copies, max_copy, nand_off);
		return 0;
	}
	else {
		printf("fail write packimg to %x\n", nand_off);
		return -1;
	}
}
#endif //#if defined(CONFIG_SPL_NAND_SUPPORT)

