#include "blockcipher.h"

#include <stdlib.h>
#include <string.h>

byte* block_cipher_encrypt_ecb(const block_cipher_spec* spec, const byte msg[], size_t msg_len, byte out[])
{
    for (int8_t b = 0; b < msg_len; b += spec->block_size)
    {
        spec->encrypt(msg + b, out + b, spec->key);
    }
    return out;
}

byte* block_cipher_decrypt_ecb(const block_cipher_spec* spec, const byte msg[], size_t msg_len, byte out[])
{
    for (int8_t b = 0; b < msg_len; b += spec->block_size)
    {
        spec->decrypt(msg + b, out + b, spec->key);
    }
    return out;
}

byte* block_cipher_encrypt_cbc(const block_cipher_spec* spec, const byte msg[], size_t msg_len, byte out[])
{
    byte* block = malloc(spec->block_size);
    const byte* prev = spec->IV;
    for (int8_t b = 0; b < msg_len; b += spec->block_size)
    {
        xor_bytes(block, prev, msg + b, spec->block_size);
        prev = spec->encrypt(block, out + b, spec->key);
    }
    free(block);
    return out;
}

byte* block_cipher_decrypt_cbc(const block_cipher_spec* spec, const byte msg[], size_t msg_len, byte out[])
{
    byte* block = malloc(spec->block_size);
    const byte* prev = spec->IV;
    for (int8_t b = 0; b < msg_len; b += spec->block_size)
    {
        spec->decrypt(msg + b, block, spec->key);
        xor_bytes(out + b, prev, block, spec->block_size);
        prev = msg + b;
    }
    free(block);
    return out;
}

static void block_cipher_ctr_increment(byte* ctr, size_t block_size)
{
    for (int8_t i = block_size - 1; i >= 0; --i)
    {
        /* for ctr[i] to be greater than ctr[i]+1 there had to be an overflow, 
         * so we need to increment the next byte too */
        if (ctr[i] < ++ctr[i]) return;
    }
}

byte* block_cipher_encrypt_ctr(const block_cipher_spec* spec, const byte msg[], size_t msg_len, byte out[])
{
    byte* ctr = malloc(spec->block_size);
    memcpy(ctr, spec->IV, spec->block_size);
    for (int8_t b = 0; b < msg_len; b += spec->block_size)
    {
        spec->encrypt(ctr, out + b, spec->key);
        xor_bytes(out + b, out + b, msg + b, spec->block_size);
        block_cipher_ctr_increment(ctr, spec->block_size);
    }
    free(ctr);
    return out;
}

byte* block_cipher_encrypt(const block_cipher_spec* spec, const byte msg[], size_t msg_len, byte out[])
{
    switch (spec->mode)
    {
    case BLOCK_CIPHER_MODE_ECB: return block_cipher_encrypt_ecb(spec, msg, msg_len, out);
    case BLOCK_CIPHER_MODE_CBC: return block_cipher_encrypt_cbc(spec, msg, msg_len, out);
    case BLOCK_CIPHER_MODE_CTR: return block_cipher_encrypt_ctr(spec, msg, msg_len, out);
    case BLOCK_CIPHER_MODE_GCM: break;
    case BLOCK_CIPHER_MODE_CCM: break;
    }

    return NULL;
}

byte* block_cipher_decrypt(const block_cipher_spec* spec, const byte msg[], size_t msg_len, byte out[])
{
    switch (spec->mode)
    {
    case BLOCK_CIPHER_MODE_ECB: return block_cipher_decrypt_ecb(spec, msg, msg_len, out);
    case BLOCK_CIPHER_MODE_CBC: return block_cipher_decrypt_cbc(spec, msg, msg_len, out);
    case BLOCK_CIPHER_MODE_CTR: return block_cipher_encrypt_ctr(spec, msg, msg_len, out);
    case BLOCK_CIPHER_MODE_GCM: break;
    case BLOCK_CIPHER_MODE_CCM: break;
    }

    return NULL;
}