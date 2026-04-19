#include "clefia.h"
#include "internal.h"
#include <string.h>
#include <stdint.h>

static void xor_block(u8 *a, const u8 *b)
{
    for (int i = 0; i < 16; i++)
        a[i] ^= b[i];
}


size_t ecb_encrypt(const u8 *in, u8 *out, size_t len, const u32 *RK, const u32 WK[4], int rounds)
{
    size_t i;
    size_t full = (len / 16) * 16;
    for (i = 0; i < full; i += 16) {
        encrypt_block(in + i, out + i, RK, WK, rounds);
    }

    if (len % 16 != 0) {
        u8 buf[16] = {0};
        memcpy(buf, in + full, len - full);
        encrypt_block(buf, out + full, RK, WK, rounds);
        return full + 16;
    }

    return full;
}

size_t ecb_decrypt(const u8 *in, u8 *out, size_t len, const u32 *RK, const u32 WK[4], int rounds)
{
    if (len == 0 || len % 16 != 0)
        return 0;

    for (size_t i = 0; i < len; i += 16) {
        decrypt_block(in + i, out + i, RK, WK, rounds);
    }

    return len;
}


size_t cbc_encrypt(const u8 *in, u8 *out, size_t len, const u32 *RK, const u32 WK[4], int rounds, u8 iv[16])
{
    u8 buf[16];
    u8 local_iv[16];
    memcpy(local_iv, iv, 16);
    size_t i;
    size_t full = (len / 16) * 16;

    for (i = 0; i < full; i += 16) {
        memcpy(buf, in + i, 16);
        xor_block(buf, local_iv);
        encrypt_block(buf, out + i, RK, WK, rounds);
        memcpy(local_iv, out + i, 16);
    }

    if (len % 16 != 0) {
        memset(buf, 0, 16);
        memcpy(buf, in + full, len - full);
        xor_block(buf, local_iv);
        encrypt_block(buf, out + full, RK, WK, rounds);
        memcpy(local_iv, out + full, 16);
        memcpy(iv, local_iv, 16);
        return full + 16;
    }

    memcpy(iv, local_iv, 16);
    return full;
}

size_t cbc_decrypt(const u8 *in, u8 *out, size_t len, const u32 *RK, const u32 WK[4], int rounds, u8 iv[16])
{
    u8 buf[16];
    u8 prev_cipher_block[16];
    u8 local_iv[16];

    if (len == 0 || len % 16 != 0)
        return 0;

    memcpy(local_iv, iv, 16);

    for (size_t i = 0; i < len; i += 16) {
        memcpy(prev_cipher_block, in + i, 16);
        decrypt_block(in + i, buf, RK, WK, rounds);
        xor_block(buf, local_iv);
        memcpy(out + i, buf, 16);
        memcpy(local_iv, prev_cipher_block, 16);
    }

    memcpy(iv, local_iv, 16);
    return len;
}


void ctr_crypt(const u8 *in, u8 *out, size_t len, const u32 *RK, const u32 WK[4], int rounds, u8 ctr[16])
{
    u8 keystream[16];
    u8 counter[16];
    memcpy(counter, ctr, 16);

    for (size_t i = 0; i < len; i += 16) {
        encrypt_block(counter, keystream, RK, WK, rounds);

        size_t block_size = (len - i < 16) ? (len - i) : 16;
        for (size_t j = 0; j < block_size; j++) {
            out[i + j] = in[i + j] ^ keystream[j];
        }

        for (int k = 15; k >= 0; k--) {
            if (++counter[k]) break;
        }
    }
    memcpy(ctr, counter, 16);
}
