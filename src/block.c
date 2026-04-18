#include "clefia.h"
#include <string.h>

void encrypt_block(const u8 in[16], u8 out[16], const u32 *RK, const u32 WK[4], int rounds)
{
    u32 T[4];
    for (int i = 0; i < 4; i++) {
        T[i] = ((u32)in[4*i] << 24) | ((u32)in[4*i+1] << 16) | ((u32)in[4*i+2] << 8) | in[4*i+3];
    }
    
    T[1] ^= WK[0];
    T[3] ^= WK[1];

    gfn4(T, RK, rounds);     

    T[1] ^= WK[2];
    T[3] ^= WK[3];

    for (int i = 0; i < 4; i++) {
        out[4*i]   = (T[i] >> 24) & 0xFF;
        out[4*i+1] = (T[i] >> 16) & 0xFF;
        out[4*i+2] = (T[i] >> 8)  & 0xFF;
        out[4*i+3] = T[i] & 0xFF;
    }
}

void decrypt_block(const u8 in[16], u8 out[16], const u32 *RK, const u32 WK[4], int rounds)
{
    u32 T[4];
    for (int i = 0; i < 4; i++) {
        T[i] = ((u32)in[4*i] << 24) | ((u32)in[4*i+1] << 16) | ((u32)in[4*i+2] << 8) | in[4*i+3];
    }
    
    T[1] ^= WK[2];
    T[3] ^= WK[3];

    gfn4_inv(T, RK, rounds);     

    T[1] ^= WK[0];
    T[3] ^= WK[1];

    for (int i = 0; i < 4; i++) {
        out[4*i]   = (T[i] >> 24) & 0xFF;
        out[4*i+1] = (T[i] >> 16) & 0xFF;
        out[4*i+2] = (T[i] >> 8)  & 0xFF;
        out[4*i+3] = T[i] & 0xFF;
    }
}
