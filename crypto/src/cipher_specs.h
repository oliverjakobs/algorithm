#ifndef CIPHER_SPECS_H
#define CIPHER_SPECS_H

#include "blockcipher.h"

void cipher_spec_des(block_cipher_spec* spec, const byte* key, const byte* IV, block_cipher_mode mode);
void cipher_spec_tdes(block_cipher_spec* spec, const byte* key, const byte* IV, block_cipher_mode mode);
void cipher_spec_aes_128(block_cipher_spec* spec, const byte* key, const byte* IV, block_cipher_mode mode);
void cipher_spec_aes_192(block_cipher_spec* spec, const byte* key, const byte* IV, block_cipher_mode mode);
void cipher_spec_aes_256(block_cipher_spec* spec, const byte* key, const byte* IV, block_cipher_mode mode);

#endif /* !CIPHER_SPECS_H */
