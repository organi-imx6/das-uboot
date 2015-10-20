#include <common.h>
#include <command.h>
#include <aes-packimg.h>

u8 packimg_key_exp[AES_EXPAND_KEY_LENGTH];
static u64 BKDRHash(u64 hash, u32 data)
{
	u64 seed = 131313; // 31 131 1313 13131 131313 etc..
	int i;

	for(i=0;i<4;i++){
		hash = hash * seed + (data&0xff);
		data>>=8;
	}

	return hash;
}

void init_aes(void)
{
	u8 key[AES_KEY_LENGTH];
	u32 id1, id2;
	u64 hash=0;

	fuse_read(0, 1, &id1);
	fuse_read(0, 2, &id2);
	hash = BKDRHash(hash,id1);
	hash = BKDRHash(hash,id2);
	memcpy(key, &hash, 8);
	hash = BKDRHash(hash,id1);
	hash = BKDRHash(hash,id2);
	memcpy(key+8, &hash, 8);

#if 0
	{
		int i;
		printf("init_aes key:");
		for(i=0;i<sizeof(key);i++)
			printf("%x,", key[i]);
		printf("\n");
	}
#endif

	aes_expand_key(key, packimg_key_exp);
}


/**
 * do_encrypt() - Handle the "aes packimg" command-line command
 * @cmdtp:	Command data struct pointer
 * @flag:	Command flag
 * @argc:	Command-line argument count
 * @argv:	Array of command-line arguments
 *
 * Returns zero on success, CMD_RET_USAGE in case of misuse and negative
 * on error.
 */
static int do_encrypt(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	uint32_t src_addr, len;
	uint8_t *src_ptr;

	if (argc != 3)
		return CMD_RET_USAGE;

	src_addr = simple_strtoul(argv[1], NULL, 16);
	len = simple_strtoul(argv[2], NULL, 16);

	src_ptr = (uint8_t *)src_addr;

	/* First we expand the key. */
	init_aes();

	aes_enc(src_ptr, len);
	return 0;
}
U_BOOT_CMD(
	encrypt, 3, 1, do_encrypt,
	"encryption data with current CPU",
	"encrypt src len - Encrypt block of data $len bytes long at address $src\n"
);

#if 1
static int do_decrypt(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	uint32_t src_addr, len;
	uint8_t *src_ptr;

	if (argc != 3)
		return CMD_RET_USAGE;

	src_addr = simple_strtoul(argv[1], NULL, 16);
	len = simple_strtoul(argv[2], NULL, 16);

	src_ptr = (uint8_t *)src_addr;

	/* First we expand the key. */
	init_aes();

	aes_dec(src_ptr, len);
	return 0;
}

/***************************************************/
U_BOOT_CMD(
	decrypt, 3, 1, do_decrypt,
	"decryption data with current CPU",
	"decrypt src len - Decrypt block of data $len bytes long at address $src\n"
);
#endif

