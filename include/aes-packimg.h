#ifndef _AES_PACKIMG_H
#define _AES_PACKIMG_H

#ifdef CONFIG_AES_PACKIMG
#include <aes.h>
#include <fuse.h>

static u8 packimg_key_exp[AES_EXPAND_KEY_LENGTH];
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

static void init_aes(void)
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
#define aes_enc(data, len)	aes_encrypt_blocks(packimg_key_exp, (u8*)data, (u8*)data, DIV_ROUND_UP(len, AES_KEY_LENGTH))
#define aes_dec(data, len)	aes_decrypt_blocks(packimg_key_exp, (u8*)data, (u8*)data, DIV_ROUND_UP(len, AES_KEY_LENGTH))
#else
#define init_aes()
#define aes_dec(data, len)
#endif

#endif
