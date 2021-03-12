#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>

typedef uint8_t bit4;
typedef uint8_t bit6;
typedef uint8_t bit8;
typedef uint8_t byte;

typedef uint32_t bit28;
typedef uint32_t bit32;
typedef uint64_t bit48;
typedef uint64_t bit64;

void print_bin4(bit4 c);
void print_bin6(bit6 c);
void print_bin8(bit8 c);
void print_bin32(bit32 c);
void print_bin48(bit48 c);
void print_bin64(bit64 c);

void print_byte(byte b);
void print_bytes32(bit32 b);
void print_bytes64(bit64 b);
void print_byte_array(const byte bytes[], size_t size);
void print_byte_block(const byte bytes[], size_t size, size_t line);

bit28 left_rotate28(bit28 b, uint8_t amount);
bit32 left_rotate32(bit32 b, uint8_t amount);

byte* xor_bytes(byte* out, const byte* left, const byte* right, size_t size);

#endif /* !COMMON_H */