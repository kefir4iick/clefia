#ifndef CLEFIA_H
#define CLEFIA_H

#include <stdint.h>
#include <stddef.h>

typedef uint8_t u8;
typedef uint32_t u32;

typedef enum {
    ECB = 0,
    CBC = 1,
    CTR = 2
} cl_mode;

typedef enum {
    KEY_128 = 16,
    KEY_192 = 24,
    KEY_256 = 32
} cl_keylen;

typedef struct {
    u32 RK[52];
    u32 WK[4];
    int rounds;
} cl_ctx;


int clefia_init(cl_ctx *ctx, const u8 *key, cl_keylen keylen);
int encrypt_file(cl_ctx *ctx, cl_mode mode, const u8 iv[16], const u8 *in, u8 *out, size_t len);
int decrypt_file(cl_ctx *ctx, cl_mode mode, const u8 iv[16], const u8 *in, u8 *out, size_t len);

#endif
