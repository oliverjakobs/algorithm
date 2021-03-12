#include "cipher_specs.h"

#include "des.h"
#include "aes.h"

void cipher_spec_des(block_cipher_spec* spec, const byte* key, const byte* IV, block_cipher_mode mode)
{
    *spec = (block_cipher_spec){ mode, key, 8, IV, 8, des_encrypt_block, des_decrypt_block };
}

void cipher_spec_tdes(block_cipher_spec* spec, const byte* key, const byte* IV, block_cipher_mode mode)
{
    *spec = (block_cipher_spec){ mode, key, 24, IV, 8, tdes_encrypt_block, tdes_decrypt_block };
}

void cipher_spec_aes_128(block_cipher_spec* spec, const byte* key, const byte* IV, block_cipher_mode mode)
{
    *spec = (block_cipher_spec){ mode, key, 16, IV, 16, aes_encrypt_block_128, aes_decrypt_block_128 };
}

void cipher_spec_aes_192(block_cipher_spec* spec, const byte* key, const byte* IV, block_cipher_mode mode)
{
    *spec = (block_cipher_spec){ mode, key, 24, IV, 16, aes_encrypt_block_192, aes_decrypt_block_192 };
}

void cipher_spec_aes_256(block_cipher_spec* spec, const byte* key, const byte* IV, block_cipher_mode mode)
{
    *spec = (block_cipher_spec){ mode, key, 32, IV, 16, aes_encrypt_block_256, aes_decrypt_block_256 };
}