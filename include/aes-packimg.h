#ifndef _AES_PACKIMG_H
#define _AES_PACKIMG_H

#ifdef CONFIG_AES_PACKIMG
#include <aes.h>
#include <fuse.h>

extern u8 packimg_key_exp[];
void init_aes(void);
#define aes_enc(data, len)	aes_encrypt_blocks(packimg_key_exp, (u8*)data, (u8*)data, DIV_ROUND_UP(len, AES_KEY_LENGTH))
#define aes_dec(data, len)	aes_decrypt_blocks(packimg_key_exp, (u8*)data, (u8*)data, DIV_ROUND_UP(len, AES_KEY_LENGTH))
#else
#define init_aes()
#define aes_dec(data, len)
#endif

#endif
