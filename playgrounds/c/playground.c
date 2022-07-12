#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COPY_BUFM(lhsbuf, lhsbuf_spos, rhsbuf, rhsbuf_spos, len, n) \
    for (int i = 0; i < len; ++i)                   \
        lhsbuf[lhsbuf_spos + i] = rhsbuf[rhsbuf_spos + i];

// HASH256 COMMON
// #define ENCODE_HASH256_COMMON_SIZE 33U
#define ENCODE_HASH256_COMMON(buf_out, i, field, len, n)\
    {\
        uint8_t uf = field;\
        (buf_out)[0] = 0x50U + (uf & 0x0FU);\
        uint8_t *ptr = (uint8_t *)&i;\
        COPY_BUFM(buf_out, 1, ptr, 0, n, n);\
        (buf_out) += len;\
    }
#define _05_XX_ENCODE_HASH256_COMMON(buf_out, tokenid, field, len, n)\
    ENCODE_HASH256_COMMON(buf_out, tokenid, field, len, n);

// NFT TOKEN ID
#define ENCODE_TOKEN_ID_SIZE 33U
#define ENCODE_TOKEN_ID(buf_out, nftokenid)\
    ENCODE_HASH256_COMMON(buf_out, nftokenid, 0xAU, 32, 32);
#define _05_10_ENCODE_TOKEN_ID(buf_out, nftokenid)\
    ENCODE_TOKEN_ID(buf_out, nftokenid);

// #define PREPARE_ANY_TEST_SIZE ?
#define PREPARE_ANY_TEST(buf_out_master, hash)  \
    {                                          \
        u_int8_t *buf_out = buf_out_master;    \
        _05_10_ENCODE_TOKEN_ID(buf_out, hash); \
    }

// TEST ANY TX
int test(void)
{

    unsigned char tx_out[33];

    char* nft_id_value = "000900008BEF7E230500E6841C47682CA6577D3D2CF6A670D89A24C70000002B";

    PREPARE_ANY_TEST(tx_out, nft_id_value);

    // printf("%x", tx_out[0]);
    // printf("%x", tx_out[1]);

    for (int i = 0; i < sizeof(tx_out); i++)
    {
        char hex_string[20];
        sprintf(hex_string, "%02X", tx_out[i]);
        printf("%s", hex_string);
    }
    
    printf("\nDONE\n");
    return 0;
}

int main(void)
{
  test();
  return 0;
}
