#include <common.h>
#include <command.h>
#include <aes-packimg.h>

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

