#ifndef BLOCK_CIPHER_H
#define BLOCK_CIPHER_H

#include "common.h"

typedef enum
{
    BLOCK_CIPHER_MODE_ECB,
    BLOCK_CIPHER_MODE_CBC,
    BLOCK_CIPHER_MODE_CTR,
    BLOCK_CIPHER_MODE_GCM,
    BLOCK_CIPHER_MODE_CCM
} block_cipher_mode;

typedef byte*(*encrypt_block_func)(const byte[], byte[], const byte[]);
typedef byte*(*decrypt_block_func)(const byte[], byte[], const byte[]);

typedef struct
{
    block_cipher_mode mode;

    const byte* key;
    size_t key_length;
    const byte* IV;
    size_t block_size;

    encrypt_block_func encrypt;
    decrypt_block_func decrypt;
} block_cipher_spec;

byte* block_cipher_encrypt(const block_cipher_spec* spec, const byte msg[], size_t msg_len, byte out[]);
byte* block_cipher_decrypt(const block_cipher_spec* spec, const byte msg[], size_t msg_len, byte out[]);

#endif /* !BLOCK_CIPHER_H */
