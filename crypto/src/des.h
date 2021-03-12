#ifndef DES_H
#define DES_H

#include "common.h"

byte* des_encrypt_block(const byte in[8], byte out[8], const byte key[8]);
byte* des_decrypt_block(const byte in[8], byte out[8], const byte key[8]);

byte* tdes_encrypt_block(const byte in[8], byte out[8], const byte key[24]);
byte* tdes_decrypt_block(const byte in[8], byte out[8], const byte key[24]);

#endif /* !DES_H */
