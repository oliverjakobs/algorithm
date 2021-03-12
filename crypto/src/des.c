#include "des.h"

#include <stdio.h>

static const uint8_t IP[] =
{
    58, 50, 42, 34, 26, 18, 10,  2,
    60, 52, 44, 36, 28, 20, 12,  4,
    62, 54, 46, 38, 30, 22, 14,  6,
    64, 56, 48, 40, 32, 24, 16,  8,
    57, 49, 41, 33, 25, 17,  9,  1,
    59, 51, 43, 35, 27, 19, 11,  3,
    61, 53, 45, 37, 29, 21, 13,  5,
    63, 55, 47, 39, 31, 23, 15,  7
};

static const uint8_t INV_IP[] = 
{
    40,  8, 48, 16, 56, 24, 64, 32,
    39,  7, 47, 15, 55, 23, 63, 31,
    38,  6, 46, 14, 54, 22, 62, 30,
    37,  5, 45, 13, 53, 21, 61, 29,
    36,  4, 44, 12, 52, 20, 60, 28,
    35,  3, 43, 11, 51, 19, 59, 27,
    34,  2, 42, 10, 50, 18, 58, 26,
    33,  1, 41,  9, 49, 17, 57, 25
};

static const uint8_t E[] =
{
    32,  1,  2,  3,  4,  5,
     4,  5,  6,  7,  8,  9,
     8,  9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32,  1
};

static const uint8_t P[] =
{
    16,  7, 20, 21,
    29, 12, 28, 17,
     1, 15, 23, 26,
     5, 18, 31, 10,
     2,  8, 24, 14,
    32, 27,  3,  9,
    19, 13, 30,  6,
    22, 11,  4, 25
};

static const uint8_t S_BOXES[8][64] =
{
{
    14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
     0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
     4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
    15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13
},
{
    15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
     3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
     0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
    13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9
},
{
    10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
    13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
    13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
     1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12
},
{
     7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
    13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
    10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
     3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14
},
{
     2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
    14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
     4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
    11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3
},
{
    12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
    10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
     9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
     4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13
},
{
     4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
    13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
     1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
     6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12
},
{
    13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
     1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
     7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
     2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
}
};

static const uint8_t PC1[] =
{
    57, 49, 41, 33, 25, 17,  9,
     1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27,
    19, 11,  3, 60, 52, 44, 36,

    63, 55, 47, 39, 31, 23, 15,
     7, 62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29,
    21, 13,  5, 28, 20, 12,  4 
};

static const uint8_t PC2[] =
{
    14, 17, 11, 24,  1,  5,
     3, 28, 15,  6, 21, 10,
    23, 19, 12,  4, 26,  8,
    16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

static const uint8_t LEFT_SHIFTS_PER_ROUND[] = 
{
/*  1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16 */
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

static bit64 des_make_block(const byte bytes[])
{
    bit64 block = 0;
    for (int8_t i = 0; i < 8; ++i)
        block = block << 8 | bytes[i];
    return block;
}

static void des_split_block(byte bytes[], bit64 block)
{
    for (int8_t i = 0; i < 8; ++i)
        bytes[i] = (byte)(block >> (7 - i) * 8);
}

static bit32 des_permute32(bit32 block, const int8_t p[])
{
    bit32 result = 0;
    for (int i = 0; i < 32; ++i)
    {
        bit32 mask = ((bit32)1) << (32 - p[i]);
        if (block & mask) result |= ((bit32)1) << (31 - i);
    }
    return result;
}

static bit64 des_permute64(bit64 block, const int8_t p[])
{
    bit64 result = 0;
    for (int i = 0; i < 64; ++i)
    {
        bit64 mask = ((bit64)1) << (64 - p[i]);
        if (block & mask) result |= ((bit64)1) << (63 - i);
    }
    return result;
}

static bit48 des_expand(bit32 block)
{
    bit48 result = 0;
    for (int i = 0; i < 48; ++i)
    {
        bit32 mask = ((bit32)1) << (32 - E[i]);
        if (block & mask) result |= ((bit48)1) << (47 - i);
    }
    return result;
}

/* split a 64 bit block into two 32 bit blocks L and R */
static void des_split64(bit64 block, bit32* L, bit32* R)
{
    *L = (block >> 32), *R = block;
}

/* join two 32 bit blocks into one 64 bit block */
static bit64 des_join64(bit32 L, bit32 R)
{
    return (((bit64)L) << 32) | R;
}

static void des_permuted_choice1(bit64 key, bit28* C, bit28* D)
{
    *C = 0, *D = 0;
    for (int i = 0; i < 28; ++i)
    {
        bit64 mask = ((bit64)1) << (64 - PC1[i]);
        if (key & mask) *C |= ((bit28)1) << (27 - i);

        mask = ((bit64)1) << (64 - PC1[i + 28]);
        if (key & mask) *D |= ((bit28)1) << (27 - i);
    }
}

static bit48 des_permuted_choice2(bit28 C, bit28 D)
{
    bit48 K = 0;
    for (int i = 0; i < 48; ++i)
    {
        int8_t index = PC2[i];
        bit28 mask = ((bit28)1) << (28 - (index % 28));
        if ((index > 28 ? D : C) & mask) K |= ((bit48)1) << (47 - i);
    }

    return K;
}

static bit48 des_key_schedule(const byte key[], bit48 round_keys[], int8_t reverse)
{
    bit28 C, D;
    des_permuted_choice1(des_make_block(key), &C, &D);

    for (int n = 0; n < 16; ++n)
    {
        C = left_rotate28(C, LEFT_SHIFTS_PER_ROUND[n]);
        D = left_rotate28(D, LEFT_SHIFTS_PER_ROUND[n]);

        int8_t index = reverse ? 15 - n : n;
        round_keys[index] = des_permuted_choice2(C, D);
    }

    return des_permuted_choice2(C, D);
}

static bit4 des_select(bit48 B, uint8_t n)
{
    const bit4 S_BOX_COLS = 16;
    const bit6 BIT_MASK_FIRST_6 = 1 << 5;

    bit6 b = (B >> ((7 - n) * 6));  /* get the n-th 6 bit block out of B */
    b &= ~(3 << 6);                 /* clear first two bits of the actual 8 bits */

    bit4 row = (b & 1) | (b & BIT_MASK_FIRST_6 ? 2 : 0);    /* first and last bit denotes the row */
    bit4 col = (b & ~BIT_MASK_FIRST_6) >> 1;                /* middle four bits denotes the column */

    return S_BOXES[n][row * S_BOX_COLS + col];
}

static bit32 des_cipher_function(bit32 R, bit48 K)
{
    bit48 B = des_expand(R) ^ K;

    bit32 S = 0;
    for (int8_t i = 0; i < 8; ++i)
        S = (S << 4) | des_select(B, i);

    return des_permute32(S, P);
}

byte* des_process_block(const byte in[8], byte out[8], const byte key[8], int8_t reverse)
{
    bit48 round_keys[16];
    des_key_schedule(key, round_keys, reverse);

    bit64 block = des_permute64(des_make_block(in), IP);

    /* for decryption the names L and R should be swapped */
    bit32 L, R, temp;
    des_split64(block, &L, &R);
    for (int n = 0; n < 16; ++n)
    {
        temp = L;
        L = R;
        R = temp ^ des_cipher_function(R, round_keys[n]);
    }

    block = des_permute64(des_join64(R, L), INV_IP);
    des_split_block(out, block);
    return out;
}

byte* des_encrypt_block(const byte in[8], byte out[8], const byte key[8])
{
    return des_process_block(in, out, key, 0);
}

byte* des_decrypt_block(const byte in[8], byte out[8], const byte key[8])
{
    return des_process_block(in, out, key, 1);
}

byte* tdes_encrypt_block(const byte in[8], byte out[8], const byte key[24])
{
    des_encrypt_block(in, out, key);
    des_decrypt_block(out, out, key + 8);
    des_encrypt_block(out, out, key + 16);
}

byte* tdes_decrypt_block(const byte in[8], byte out[8], const byte key[24])
{
    des_decrypt_block(in, out, key + 16);
    des_encrypt_block(out, out, key + 8);
    des_decrypt_block(out, out, key);
}