#include "clefia.h"
#include <stdio.h>
#include <string.h>

typedef unsigned char u8;
typedef unsigned int  u32;

static void print_hex(const char *label, const u8 *data, size_t len)
{
    printf("%-12s: ", label);
    for (size_t i = 0; i < len; i++) {
        printf("%02x", data[i]);
        if ((i + 1) % 4 == 0) printf(" ");
    }
    printf("\n");
}

static void test_128(void)
{
    const u8 pt[16] = {
        0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
        0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f
    };

    print_hex("plain", pt, 16);
    printf("\n");
    
    printf("128-bit key: \n");

    const u8 key[16] = {
        0xff,0xee,0xdd,0xcc,0xbb,0xaa,0x99,0x88,
        0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00
    };

    const u8 expected_128[16] = {
        0xde,0x2b,0xf2,0xfd,
        0x9b,0x74,0xaa,0xcd,
        0xf1,0x29,0x85,0x55,
        0x45,0x94,0x94,0xfd
    };

    u8 ct[16], dec[16];

    u32 RK[36], WK[4];
    keyschedule_128(key, RK, WK);

    encrypt_block(pt, ct, RK, WK, 18);
    decrypt_block(ct, dec, RK, WK, 18);

    print_hex("key", key, 16);
    print_hex("cipher", ct, 16);

    int ok_enc = memcmp(ct, expected_128, 16) == 0;
    int ok_dec = memcmp(pt, dec, 16) == 0;

    print_hex("expected", expected_128, 16);

    if (ok_enc && ok_dec)
        printf("success\n\n");
    else {
        printf("fail\n");
        if (!ok_enc) printf("   enc wrong\n");
        if (!ok_dec) printf("   dec wrong\n");
        printf("\n");
    }
}

static void test_192(void)
{
    printf("192-bit key: \n");

    const u8 key[24] = {
        0xff,0xee,0xdd,0xcc,0xbb,0xaa,0x99,0x88,
        0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
        0xf0,0xe0,0xd0,0xc0,0xb0,0xa0,0x90,0x80
    };

    const u8 pt[16] = {
        0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
        0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f
    };

    const u8 expected_192[16] = {
        0xe2,0x48,0x2f,0x64,
        0x9f,0x02,0x8d,0xc4,
        0x80,0xdd,0xa1,0x84,
        0xfd,0xe1,0x81,0xad
    };

    u8 ct[16], dec[16];

    u32 RK[44], WK[4];
    keyschedule_192(key, RK, WK);

    encrypt_block(pt, ct, RK, WK, 22);
    decrypt_block(ct, dec, RK, WK, 22);

    print_hex("key", key, 24);
    print_hex("cipher", ct, 16);

    int ok_enc = memcmp(ct, expected_192, 16) == 0;
    int ok_dec = memcmp(pt, dec, 16) == 0;

    print_hex("expected", expected_192, 16);

    if (ok_enc && ok_dec)
        printf("success\n\n");
    else {
        printf("fail\n");
        if (!ok_enc) printf("   enc wrong\n");
        if (!ok_dec) printf("   dec wrong\n");
        printf("\n");
    }
}

static void test_256(void)
{
    printf("256-bit key:\n");

    const u8 key[32] = {
        0xff,0xee,0xdd,0xcc,0xbb,0xaa,0x99,0x88,
        0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
        0xf0,0xe0,0xd0,0xc0,0xb0,0xa0,0x90,0x80,
        0x70,0x60,0x50,0x40,0x30,0x20,0x10,0x00
    };

    const u8 pt[16] = {
        0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
        0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f
    };

    const u8 expected_256[16] = {
        0xa1,0x39,0x78,0x14,
        0x28,0x9d,0xe8,0x0c,
        0x10,0xda,0x46,0xd1,
        0xfa,0x48,0xb3,0x8a
    };

    u8 ct[16], dec[16];

    u32 RK[52], WK[4];
    keyschedule_256(key, RK, WK);

    encrypt_block(pt, ct, RK, WK, 26);
    decrypt_block(ct, dec, RK, WK, 26);

    print_hex("key", key, 32);
    print_hex("cipher", ct, 16);

    int ok_enc = memcmp(ct, expected_256, 16) == 0;
    int ok_dec = memcmp(pt, dec, 16) == 0;

    print_hex("expected", expected_256, 16);

    if (ok_enc && ok_dec)
        printf("success\n\n");
    else {
        printf("fail\n");
        if (!ok_enc) printf("   enc wrong\n");
        if (!ok_dec) printf("   dec wrong\n");
        printf("\n");
    }
}

int main(void)
{
    printf("test\n\n");

    test_128();
    test_192();
    test_256();

    return 0;
}
