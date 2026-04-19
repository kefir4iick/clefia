#include "clefia.h"
#include "internal.h"
#include <string.h>

int clefia_init(cl_ctx *ctx, const u8 *key, cl_keylen keylen)
{
    if (!ctx || !key)
        return -1;

    memset(ctx, 0, sizeof(*ctx));

    switch (keylen) {
        case KEY_128:
            keyschedule_128(key, ctx->RK, ctx->WK);
            ctx->rounds = 12;
            break;
        case KEY_192:
            keyschedule_192(key, ctx->RK, ctx->WK);
            ctx->rounds = 14;
            break;
        case KEY_256:
            keyschedule_256(key, ctx->RK, ctx->WK);
            ctx->rounds = 16;
            break;
        default:
            return -1;
    }
    return 0;
}


int encrypt_file(cl_ctx *ctx, cl_mode mode, const u8 iv_in[16], const u8 *in, u8 *out, size_t len) {
    if (!ctx || !in || !out)
        return -1;

    switch (mode) {
        case ECB:
            ecb_encrypt(in, out, len, ctx->RK, ctx->WK, ctx->rounds);
            return 0;

        case CBC: {
            if (!iv_in) return -1;
            u8 iv[16];
            memcpy(iv, iv_in, 16);
            cbc_encrypt(in, out, len, ctx->RK, ctx->WK, ctx->rounds, iv);
            return 0;
        }

        case CTR: {
            if (!iv_in) return -1;
            u8 ctr[16];
            memcpy(ctr, iv_in, 16);
            ctr_crypt(in, out, len, ctx->RK, ctx->WK, ctx->rounds, ctr);
            return 0;
        }

        default:
            return -1;
    }
}


int decrypt_file(cl_ctx *ctx, cl_mode mode, const u8 iv_in[16], const u8 *in, u8 *out, size_t len) {
    if (!ctx || !in || !out)
        return -1;

    if (mode != CTR && (len % 16 != 0))
        return -1;

    switch (mode) {
        case ECB:
            ecb_decrypt(in, out, len, ctx->RK, ctx->WK, ctx->rounds);
            return 0;

        case CBC: {
            if (!iv_in) return -1;
            u8 iv[16];
            memcpy(iv, iv_in, 16);
            cbc_decrypt(in, out, len, ctx->RK, ctx->WK, ctx->rounds, iv);
            return 0;
        }

        case CTR: {
            if (!iv_in) return -1;
            u8 ctr[16];
            memcpy(ctr, iv_in, 16);
            ctr_crypt(in, out, len, ctx->RK, ctx->WK, ctx->rounds, ctr);
            return 0;
        }

        default:
            return -1;
    }
}
