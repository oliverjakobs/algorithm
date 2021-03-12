#ifndef AES_H
#define AES_H

#include "common.h"

#define AES_128_NK  4
#define AES_128_NR 10

#define AES_192_NK  6
#define AES_192_NR 12

#define AES_256_NK  8
#define AES_256_NR 14

byte* aes_encrypt_block_128(const byte in[16], byte out[16], const byte key[16]);
byte* aes_encrypt_block_192(const byte in[16], byte out[16], const byte key[24]);
byte* aes_encrypt_block_256(const byte in[16], byte out[16], const byte key[32]);

byte* aes_decrypt_block_128(const byte in[16], byte out[16], const byte key[16]);
byte* aes_decrypt_block_192(const byte in[16], byte out[16], const byte key[24]);
byte* aes_decrypt_block_256(const byte in[16], byte out[16], const byte key[32]);

#endif /* !AES_H */
