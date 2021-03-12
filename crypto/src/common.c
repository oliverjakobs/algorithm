#include "common.h"

#include <stdio.h>

void print_bin4(bit4 c)
{
    for (int8_t i = 3; i >= 0; --i)
    {
        putchar( (c & (((bit4)1) << i)) ? '1' : '0' );
    }
    putchar('\n');
}

void print_bin6(bit6 c)
{
    for (int8_t i = 5; i >= 0; --i)
    {
        putchar( (c & (((bit6)1) << i)) ? '1' : '0' );
    }
    putchar('\n');
}

void print_bin8(bit8 c)
{
    for (int8_t i = 7; i >= 0; --i)
    {
        putchar( (c & (((bit8)1) << i)) ? '1' : '0' );
        if (i % 4 == 0) putchar(' ');
    }
    putchar('\n');
}

void print_bin32(bit32 c)
{
    for (int8_t i = 31; i >= 0; --i)
    {
        putchar( (c & (((bit32)1) << i)) ? '1' : '0' );
        if (i % 4 == 0) putchar(' ');
    }
    putchar('\n');
}

void print_bin48(bit48 c)
{
    for (int8_t i = 47; i >= 0; --i)
    {
        putchar( (c & (((bit48)1) << i)) ? '1' : '0' );
        if (i % 4 == 0) putchar(' ');
    }
    putchar('\n');
}

void print_bin64(bit64 c)
{
    for (int8_t i = 63; i >= 0; --i)
    {
        putchar( (c & (((bit64)1) << i)) ? '1' : '0' );
        if (i % 4 == 0) putchar(' ');
    }
    putchar('\n');
}

void print_byte(byte b)
{
    printf("%02hhX\n", b);
}

void print_bytes32(bit32 b)
{
    for (int8_t i = 3; i >= 0; --i)
    {
        printf("%02hhX", (byte)(b >> i * 8));
        if (i != 0) putchar(':');
    }
    putchar('\n');
}

void print_bytes64(bit64 b)
{
    for (int8_t i = 7; i >= 0; --i)
    {
        printf("%02hhX", (byte)(b >> i * 8));
        if (i != 0) putchar(':');
    }
    putchar('\n');
}

void print_byte_array(const byte bytes[], size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        printf("%02hhX", bytes[i]);
        if (i != size - 1) printf(":");
    }
    printf("\n");
}

void print_byte_block(const byte bytes[], size_t size, size_t line)
{
    for (size_t i = 0; i < size; ++i)
    {
        if (i % line != 0) printf(":");
        else if (i != 0) printf("\n");
        printf("%02hhX", bytes[i]);
    }
    printf("\n");
}

bit28 left_rotate28(bit28 b, uint8_t amount)
{
    return (b << amount) | (b >> (28 - amount));
}

bit32 left_rotate32(bit32 b, uint8_t amount)
{
    return (b << amount) | (b >> (32 - amount));
}

byte* xor_bytes(byte* out, const byte* left, const byte* right, size_t size)
{
    for (int8_t i = 0; i < size; ++i)
        out[i] = left[i] ^ right[i];

    return out;
}