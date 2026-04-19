#ifndef CLEFIA_INTERNAL_H
#define CLEFIA_INTERNAL_H

#include <stdint.h>
#include <stddef.h>
#include "clefia.h"

typedef uint32_t u32;
typedef uint8_t u8;

extern const u32 CON128[60];
extern const u32 CON192[84];
extern const u32 CON256[92];

extern const u8 S0[256];
extern const u8 S1[256];


void encrypt_block(const u8 in[16], u8 out[16], const u32 *RK, const u32 WK[4], int rounds);
void decrypt_block(const u8 in[16], u8 out[16], const u32 *RK, const u32 WK[4], int rounds);


void gfn4(u32 T[4], const u32 *RK, int r);
void gfn4_inv(u32 T[4], const u32 *RK, int r);
void gfn8(u32 T[8], const u32 *RK, int r);


void keyschedule_128(const u8 key[16], u32 RK[36], u32 WK[4]);
void keyschedule_192(const u8 key[24], u32 RK[44], u32 WK[4]);
void keyschedule_256(const u8 key[32], u32 RK[52], u32 WK[4]);


size_t ecb_encrypt(const u8 *in, u8 *out, size_t len, const u32 *RK, const u32 WK[4], int rounds);
size_t ecb_decrypt(const u8 *in, u8 *out, size_t len, const u32 *RK, const u32 WK[4], int rounds);
size_t cbc_encrypt(const u8 *in, u8 *out, size_t len, const u32 *RK, const u32 WK[4], int rounds, u8 iv[16]);
size_t cbc_decrypt(const u8 *in, u8 *out, size_t len, const u32 *RK, const u32 WK[4], int rounds, u8 iv[16]);
void ctr_crypt(const u8 *in, u8 *out, size_t len, const u32 *RK, const u32 WK[4], int rounds, u8 ctr[16]);


u8 gf_mul(u8 a, u8 b);
void sigma(u32 L[4]);

#endif
