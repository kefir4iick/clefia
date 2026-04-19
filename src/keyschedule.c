#include "clefia.h"
#include "internal.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>

void keyschedule_128(const u8 key[16], u32 RK[36], u32 WK[4])
{
    u32 K[4], L[4], T[4];
    for (int i = 0; i < 4; i++) {
        K[i] = ((u32)key[4*i] << 24) | ((u32)key[4*i+1] << 16) | ((u32)key[4*i+2] << 8) | key[4*i+3];
    }

    memcpy(L, K, sizeof(K));    
    gfn4(L, CON128, 12);

    WK[0] = K[0]; WK[1] = K[1]; WK[2] = K[2]; WK[3] = K[3];

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 4; j++) {
            T[j] = L[j] ^ CON128[24 + 4*i + j];
        }

        sigma(L);

        if (i & 1) {
            for (int j = 0; j < 4; j++) {
                T[j] ^= K[j];
            }
        }

        for (int j = 0; j < 4; j++) {
            RK[4*i + j] = T[j];
        }
    }
}


void keyschedule_192(const u8 key[24], u32 RK[44], u32 WK[4])
{
    u32 KL[4], KR[4], T[8];
    u32 LL[4], LR[4];

    for (int i = 0; i < 4; i++) {
        KL[i] = ((u32)key[4*i] << 24) | ((u32)key[4*i+1] << 16) | ((u32)key[4*i+2] << 8) | key[4*i+3];
    }
    KR[0] = ((u32)key[16] << 24) | ((u32)key[17] << 16) | ((u32)key[18] << 8)  | key[19];
    KR[1] = ((u32)key[20] << 24) | ((u32)key[21] << 16) | ((u32)key[22] << 8)  | key[23];
    KR[2] = ~KL[0];
    KR[3] = ~KL[1];

    memcpy(T, KL, 16);
    memcpy(T + 4, KR, 16);
    gfn8(T, CON192, 10);

    memcpy(LL, T, 16);
    memcpy(LR, T + 4, 16);

    for (int i = 0; i < 4; i++) {
        WK[i] = KL[i] ^ KR[i];
    }

    for (int i = 0; i < 11; i++) {
        int use_LL = ((i % 4) == 0 || (i % 4) == 1);
        u32 *S = use_LL ? LL : LR;
        u32 TT[4];

        for (int j = 0; j < 4; j++) {
            TT[j] = S[j] ^ CON192[40 + 4*i + j];
        }

        sigma(S);

        if (i & 1) {
            if (use_LL) {
                for (int j = 0; j < 4; j++) TT[j] ^= KR[j];
            } else {
                for (int j = 0; j < 4; j++) TT[j] ^= KL[j];
            }
        }

        for (int j = 0; j < 4; j++) {
            RK[4*i + j] = TT[j];
        }
    }
}


void keyschedule_256(const u8 key[32], u32 RK[52], u32 WK[4])
{
    u32 KL[4], KR[4], T[8];
    u32 LL[4], LR[4];

    for (int i = 0; i < 4; i++) {
        KL[i] = ((u32)key[4*i] << 24) | ((u32)key[4*i+1] << 16) | ((u32)key[4*i+2] << 8) | key[4*i+3];
        KR[i] = ((u32)key[16 + 4*i] << 24) | ((u32)key[16 + 4*i + 1] << 16) | ((u32)key[16 + 4*i + 2] << 8) | key[16 + 4*i + 3];
    }

    memcpy(T, KL, 16);
    memcpy(T + 4, KR, 16);
    gfn8(T, CON256, 10);
    memcpy(LL, T, 16);
    memcpy(LR, T + 4, 16);

    for (int i = 0; i < 4; i++) {
        WK[i] = KL[i] ^ KR[i];
    }

    for (int i = 0; i < 13; i++) {
        int use_LL = ((i % 4) == 0 || (i % 4) == 1);
        u32 *S = use_LL ? LL : LR;
        u32 TT[4];

        for (int j = 0; j < 4; j++) {
            TT[j] = S[j] ^ CON256[40 + 4*i + j];
        }

        sigma(S);

        if (i & 1) {
            if (use_LL) {
                for (int j = 0; j < 4; j++) TT[j] ^= KR[j];
            } else {
                for (int j = 0; j < 4; j++) TT[j] ^= KL[j];
            }
        }

        for (int j = 0; j < 4; j++) {
            RK[4*i + j] = TT[j];
        }
    }
}
