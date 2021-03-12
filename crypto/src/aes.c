#include "aes.h"

/* Number  of  columns  (32-bit  words)  comprising  the  State.  For  this  standard, Nb = 4 */
static const uint8_t AES_NB = 4;
static const uint8_t AES_ROWS = 4;

static const byte sbox[256] = 
{
/*  0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F */
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 
};

static const byte inv_sbox[256] = 
{
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

static const byte Rcon[11] = { 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };

static bit32 aes_make_word(const byte bytes[])
{
    return ((bit32)bytes[0] << 24) | ((bit32)bytes[1] << 16) | ((bit32)bytes[2] << 8) | bytes[3];
}

static bit32 aes_rot_word(bit32 w)
{
    return left_rotate32(w, 8);
}

static bit32 aes_sub_word(bit32 w)
{
    byte bytes[] = 
    { 
        sbox[(byte)(w >> 24)], 
        sbox[(byte)(w >> 16)], 
        sbox[(byte)(w >> 8)], 
        sbox[(byte)(w)] 
    };

    return aes_make_word(bytes);
}

static void aes_key_schedule(const byte key[], bit32 schedule[], int8_t Nk, int8_t Nr)
{
    for (int8_t i = 0; i < Nk; ++i)
        schedule[i] = aes_make_word(key + (4 * i));

    bit32 temp;
    for (int8_t i = Nk; i < AES_NB * (Nr + 1); ++i)
    {
        temp = schedule[i - 1];
        if (i % Nk == 0)
            temp = aes_sub_word(aes_rot_word(temp)) ^ ((bit32)Rcon[i / Nk] << 24);
        else if ((Nk > 6) && (i % Nk == 4))
            temp = aes_sub_word(temp);

        schedule[i] = schedule[i - Nk] ^ temp;
    }
}

static void aes_add_round_key(byte state[], const bit32 round_key[])
{
    for (int8_t c = 0; c < AES_NB; ++c)
    {
        state[c * AES_ROWS + 0] ^= (byte)(round_key[c] >> 24);
        state[c * AES_ROWS + 1] ^= (byte)(round_key[c] >> 16);
        state[c * AES_ROWS + 2] ^= (byte)(round_key[c] >> 8);
        state[c * AES_ROWS + 3] ^= (byte)round_key[c];
    }
}

static void aes_sub_bytes(byte state[])
{
    for (uint8_t i = 0; i < AES_ROWS * AES_NB; ++i)
        state[i] = sbox[state[i]];
}

static void aes_inv_sub_bytes(byte state[])
{
    for (uint8_t i = 0; i < AES_ROWS * AES_NB; ++i)
        state[i] = inv_sbox[state[i]];
}

static void aes_shift_rows(byte state[])
{
    /* Rotate r = 1 by 1 column to the left */
    byte temp = state[0 * AES_ROWS + 1];
    state[0 * AES_ROWS + 1] = state[1 * AES_ROWS + 1];
    state[1 * AES_ROWS + 1] = state[2 * AES_ROWS + 1];
    state[2 * AES_ROWS + 1] = state[3 * AES_ROWS + 1];
    state[3 * AES_ROWS + 1] = temp;

    /*
     * Rotate r = 2 by 2 columns to the left or
     * swap the first byte in the row with the 
     * third and the second one with the forth
     */
    temp = state[0 * AES_ROWS + 2];
    state[0 * AES_ROWS + 2] = state[2 * AES_ROWS + 2];
    state[2 * AES_ROWS + 2] = temp;

    temp = state[1 * AES_ROWS + 2];
    state[1 * AES_ROWS + 2] = state[3 * AES_ROWS + 2];
    state[3 * AES_ROWS + 2] = temp;

    /* 
     * Rotate r = 3 by 3 columns to the left or
     * by one column to the right
     */
    temp = state[0 * AES_ROWS + 3];
    state[0 * AES_ROWS + 3] = state[3 * AES_ROWS + 3];
    state[3 * AES_ROWS + 3] = state[2 * AES_ROWS + 3];
    state[2 * AES_ROWS + 3] = state[1 * AES_ROWS + 3];
    state[1 * AES_ROWS + 3] = temp;
}

static void aes_inv_shift_rows(byte state[])
{
    /* Rotate r = 1 by 1 column to the right */
    byte temp = temp = state[0 * AES_ROWS + 1];
    state[0 * AES_ROWS + 1] = state[3 * AES_ROWS + 1];
    state[3 * AES_ROWS + 1] = state[2 * AES_ROWS + 1];
    state[2 * AES_ROWS + 1] = state[1 * AES_ROWS + 1];
    state[1 * AES_ROWS + 1] = temp;

    /* Rotate r = 2 by 2 columns to the right or
     * swap the first byte in the row with the 
     * third and the second one with the forth */
    temp = state[0 * AES_ROWS + 2];
    state[0 * AES_ROWS + 2] = state[2 * AES_ROWS + 2];
    state[2 * AES_ROWS + 2] = temp;

    temp = state[1 * AES_ROWS + 2];
    state[1 * AES_ROWS + 2] = state[3 * AES_ROWS + 2];
    state[3 * AES_ROWS + 2] = temp;

    /* Rotate r = 3 by 3 columns to the right or
     * by one column to the left */
    temp = state[0 * AES_ROWS + 3];
    state[0 * AES_ROWS + 3] = state[1 * AES_ROWS + 3];
    state[1 * AES_ROWS + 3] = state[2 * AES_ROWS + 3];
    state[2 * AES_ROWS + 3] = state[3 * AES_ROWS + 3];
    state[3 * AES_ROWS + 3] = temp;
}

static byte aes_xtime(byte b)
{
    return ((b << 1) ^ (((b >> 7) & 1) * 0x1b));
}

static byte aes_finite_mult(byte a, byte b)
{
    byte a0 = (a >> 0 & 1) * b;
    byte a1 = (a >> 1 & 1) * aes_xtime(b);
    byte a2 = (a >> 2 & 1) * aes_xtime(aes_xtime(b));
    byte a3 = (a >> 3 & 1) * aes_xtime(aes_xtime(aes_xtime(b)));
    byte a4 = (a >> 4 & 1) * aes_xtime(aes_xtime(aes_xtime(aes_xtime(b))));

    return a0 ^ a1 ^ a2 ^ a3 ^ a4;
}

static void aes_mix_columns(byte state[])
{
    byte s0, s1, s2, s3;
    for (uint8_t c = 0; c < AES_NB; ++c)
    {
        s0 = state[c * AES_ROWS + 0];
        s1 = state[c * AES_ROWS + 1];
        s2 = state[c * AES_ROWS + 2];
        s3 = state[c * AES_ROWS + 3];

        state[c * AES_ROWS + 0] = aes_finite_mult(0x02, s0) ^ aes_finite_mult(0x03, s1) ^ s2 ^ s3;
        state[c * AES_ROWS + 1] = s0 ^ aes_finite_mult(0x02, s1) ^ aes_finite_mult(0x03, s2) ^ s3;
        state[c * AES_ROWS + 2] = s0 ^ s1 ^ aes_finite_mult(0x02, s2) ^ aes_finite_mult(0x03, s3);
        state[c * AES_ROWS + 3] = aes_finite_mult(0x03, s0) ^ s1 ^ s2 ^ aes_finite_mult(0x02, s3);
    }
}

static void aes_inv_mix_columns(byte state[])
{
    byte s0, s1, s2, s3;
    for (uint8_t c = 0; c < AES_NB; ++c)
    {
        s0 = state[c * AES_ROWS + 0];
        s1 = state[c * AES_ROWS + 1];
        s2 = state[c * AES_ROWS + 2];
        s3 = state[c * AES_ROWS + 3];

        state[c * AES_ROWS + 0] = aes_finite_mult(0x0e, s0) ^ aes_finite_mult(0x0b, s1)
                                ^ aes_finite_mult(0x0d, s2) ^ aes_finite_mult(0x09, s3);
        state[c * AES_ROWS + 1] = aes_finite_mult(0x09, s0) ^ aes_finite_mult(0x0e, s1)
                                ^ aes_finite_mult(0x0b, s2) ^ aes_finite_mult(0x0d, s3);
        state[c * AES_ROWS + 2] = aes_finite_mult(0x0d, s0) ^ aes_finite_mult(0x09, s1)
                                ^ aes_finite_mult(0x0e, s2) ^ aes_finite_mult(0x0b, s3);
        state[c * AES_ROWS + 3] = aes_finite_mult(0x0b, s0) ^ aes_finite_mult(0x0d, s1)
                                ^ aes_finite_mult(0x09, s2) ^ aes_finite_mult(0x0e, s3);
    }
}

byte* aes_encrypt_block(const byte in[], byte out[], const byte key[], int8_t Nk, int8_t Nr)
{
    byte state[16]; /* state[AES_ROWS * AES_NB] */
    memcpy(state, in, 16);
    
    /* largest possible key_schedule (for Nr = 14) */
    bit32 key_schedule[4 * (14 + 1)]; /* key_schedule[AES_NB * (Nr + 1)] */
    aes_key_schedule(key, key_schedule, Nk, Nr);

    aes_add_round_key(state, key_schedule);

    for (int8_t round = 1; round < Nr; ++round)
    {
        aes_sub_bytes(state);
        aes_shift_rows(state);
        aes_mix_columns(state);
        aes_add_round_key(state, key_schedule + (round * AES_NB));
    }

    aes_sub_bytes(state);
    aes_shift_rows(state);
    aes_add_round_key(state, key_schedule + (Nr * AES_NB));

    memcpy(out, state, 16);
    return out;
}

byte* aes_decrypt_block(const byte in[], byte out[], const byte key[], int8_t Nk, int8_t Nr)
{
    byte state[16]; /* state[AES_ROWS * AES_NB] */
    memcpy(state, in, 16);

    /* largest possible key_schedule (for Nr = 14) */
    bit32 key_schedule[4 * (14 + 1)]; /* key_schedule[AES_NB * (Nr + 1)] */
    aes_key_schedule(key, key_schedule, Nk, Nr);

    aes_add_round_key(state, key_schedule + (Nr * AES_NB));

    for (int8_t round = Nr -1; round > 0; --round)
    {
        aes_inv_shift_rows(state);
        aes_inv_sub_bytes(state);
        aes_add_round_key(state, key_schedule + (round * AES_NB));
        aes_inv_mix_columns(state);
    }

    aes_inv_shift_rows(state);
    aes_inv_sub_bytes(state);
    aes_add_round_key(state, key_schedule);

    memcpy(out, state, 16);
    return out;
}

byte* aes_encrypt_block_128(const byte in[16], byte out[16], const byte key[16])
{
    return aes_encrypt_block(in, out, key, AES_128_NK, AES_128_NR);
}

byte* aes_encrypt_block_192(const byte in[16], byte out[16], const byte key[24])
{
    return aes_encrypt_block(in, out, key, AES_192_NK, AES_192_NR);
}

byte* aes_encrypt_block_256(const byte in[16], byte out[16], const byte key[32])
{
    return aes_encrypt_block(in, out, key, AES_256_NK, AES_256_NR);
}


byte* aes_decrypt_block_128(const byte in[16], byte out[16], const byte key[16])
{
    return aes_decrypt_block(in, out, key, AES_128_NK, AES_128_NR);
}

byte* aes_decrypt_block_192(const byte in[16], byte out[16], const byte key[24])
{
    return aes_decrypt_block(in, out, key, AES_192_NK, AES_192_NR);
}

byte* aes_decrypt_block_256(const byte in[16], byte out[16], const byte key[32])
{
    return aes_decrypt_block(in, out, key, AES_256_NK, AES_256_NR);
}