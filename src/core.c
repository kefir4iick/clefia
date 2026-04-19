#include "clefia.h"
#include "internal.h"
#include <string.h>
#include <stdio.h>

uint8_t gf_mul(uint8_t x, uint8_t y) {
    uint16_t result = 0;
    uint16_t temp = x;

    for (int bit = 0; bit < 8; bit++) {
        if (y & 1) {
            result ^= temp;
        }

        temp <<= 1;
        if (temp & 0x100) {
            temp ^= 0x11D;
        }

        y >>= 1;
    }

    return (uint8_t)(result & 0xFF);
}

static u32 F0(u32 rk, u32 x)
{
    u32 v = rk ^ x;

    u8 b0 = S0[(v >> 24) & 0xFF];
    u8 b1 = S1[(v >> 16) & 0xFF];
    u8 b2 = S0[(v >> 8) & 0xFF];
    u8 b3 = S1[(v) & 0xFF];

    u8 y0 = b0 ^ gf_mul(b1, 0x02) ^ gf_mul(b2, 0x04) ^ gf_mul(b3, 0x06);
    u8 y1 = gf_mul(b0, 0x02) ^ b1 ^ gf_mul(b2, 0x06) ^ gf_mul(b3, 0x04);
    u8 y2 = gf_mul(b0, 0x04) ^ gf_mul(b1, 0x06) ^ b2 ^ gf_mul(b3, 0x02);
    u8 y3 = gf_mul(b0, 0x06) ^ gf_mul(b1, 0x04) ^ gf_mul(b2, 0x02) ^ b3;

    return ((u32)y0 << 24) | ((u32)y1 << 16) | ((u32)y2 << 8) | y3;
}


static u32 F1(u32 rk, u32 x)
{
    u32 v = rk ^ x;

    u8 b0 = S1[(v >> 24) & 0xFF];
    u8 b1 = S0[(v >> 16) & 0xFF];
    u8 b2 = S1[(v >> 8) & 0xFF];
    u8 b3 = S0[(v) & 0xFF];

    u8 y0 = b0 ^ gf_mul(b1, 0x08) ^ gf_mul(b2, 0x02) ^ gf_mul(b3, 0x0A);
    u8 y1 = gf_mul(b0, 0x08) ^ b1 ^ gf_mul(b2, 0x0A) ^ gf_mul(b3, 0x02);
    u8 y2 = gf_mul(b0, 0x02) ^ gf_mul(b1, 0x0A) ^ b2 ^ gf_mul(b3, 0x08);
    u8 y3 = gf_mul(b0, 0x0A) ^ gf_mul(b1, 0x02) ^ gf_mul(b2, 0x08) ^ b3;

    return ((u32)y0 << 24) | ((u32)y1 << 16) | ((u32)y2 << 8) | y3;
}


void sigma(uint32_t L[4])
{
    uint32_t t0 = L[0], t1 = L[1], t2 = L[2], t3 = L[3];
    
    L[0] = (t0 << 7) | (t1 >> 25);
    L[1] = (t1 << 7) | (t3 & 0x7F);
    L[2] = (t0 & 0xFE000000) | (t2 >> 7);
    L[3] = (t2 << 25) | (t3 >> 7);
}

void gfn4(uint32_t T[4], const uint32_t *RK, int r)
{
    uint32_t t0 = T[0], t1 = T[1], t2 = T[2], t3 = T[3];
    for (int i = 0; i < r; i++) {
        t1 ^= F0(RK[2 * i], t0);
        t3 ^= F1(RK[2 * i + 1], t2);
        uint32_t tmp = t0;
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = tmp;
    }

    T[0] = t3;
    T[1] = t0;
    T[2] = t1;
    T[3] = t2;
}


void gfn8(uint32_t T[8], const uint32_t *RK, int r)
{
    uint32_t t[8];
    memcpy(t, T, 32);
    for (int i = 0; i < r; i++) {
        t[1] ^= F0(RK[4 * i + 0], t[0]);
        t[3] ^= F1(RK[4 * i + 1], t[2]);
        t[5] ^= F0(RK[4 * i + 2], t[4]);
        t[7] ^= F1(RK[4 * i + 3], t[6]);
        uint32_t tmp = t[0];
        t[0] = t[1]; t[1] = t[2]; t[2] = t[3]; t[3] = t[4];
        t[4] = t[5]; t[5] = t[6]; t[6] = t[7]; t[7] = tmp;
    }

    T[0] = t[7];
    T[1] = t[0];
    T[2] = t[1];
    T[3] = t[2];
    T[4] = t[3];
    T[5] = t[4];
    T[6] = t[5];
    T[7] = t[6];
}


void gfn4_inv(uint32_t T[4], const uint32_t *RK, int r)
{
    uint32_t t0 = T[0], t1 = T[1], t2 = T[2], t3 = T[3];
    for (int i = 0; i < r; i++) {
        int rk_idx = 2 * (r - i - 1);
        t1 ^= F0(RK[rk_idx], t0);
        t3 ^= F1(RK[rk_idx + 1], t2);
        uint32_t tmp0 = t0, tmp1 = t1, tmp2 = t2;
        t0 = t3;
        t1 = tmp0;
        t2 = tmp1;
        t3 = tmp2;
    }

    T[0] = t1;
    T[1] = t2;
    T[2] = t3;
    T[3] = t0;
}
