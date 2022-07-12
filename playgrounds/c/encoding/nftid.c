#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COPY_BUFM(lhsbuf, lhsbuf_spos, rhsbuf, rhsbuf_spos, len, n) \
    for (int i = 0; i < len; ++i)                   \
        lhsbuf[lhsbuf_spos + i] = rhsbuf[rhsbuf_spos + i];

// // HASH256 COMMON
// // #define ENCODE_HASH256_COMMON_SIZE 33U
// #define ENCODE_HASH256_COMMON(buf_out, i, field, len, n)\
//     {\
//         uint8_t uf = field;\
//         (buf_out)[0] = 0x50U + (uf & 0x0FU);\
//         uint8_t *ptr = (uint8_t *)&i;\
//         COPY_BUFM(buf_out, 1, ptr, 0, n, n);\
//         (buf_out) += ENCODE_HASH256_COMMON_SIZE;\
//     }
// #define _05_XX_ENCODE_HASH256_COMMON(buf_out, tokenid, field, len, n)\
//     ENCODE_HASH256_COMMON(buf_out, tokenid, field, len);

// HASH256 COMMON
#define ENCODE_HASH256_COMMON_SIZE 33U
#define ENCODE_HASH256_COMMON(buf_out, i, field)\
    {\
        uint8_t uf = field;\
        (buf_out)[0] = 0x50U + (uf & 0x0FU);\
        (buf_out)[1] = i[0]; \
        (buf_out)[2] = i[1]; \
        (buf_out)[3] = i[2]; \
        (buf_out)[4] = i[3]; \
        (buf_out)[5] = i[4]; \
        (buf_out)[6] = i[5]; \
        (buf_out)[7] = i[6]; \
        (buf_out)[8] = i[7]; \
        (buf_out)[9] = i[8]; \
        (buf_out)[10] = i[9]; \
        (buf_out)[11] = i[10]; \
        (buf_out)[12] = i[11]; \
        (buf_out)[13] = i[12]; \
        (buf_out)[14] = i[13]; \
        (buf_out)[15] = i[14]; \
        (buf_out)[16] = i[15]; \
        (buf_out)[17] = i[16]; \
        (buf_out)[18] = i[17]; \
        (buf_out)[19] = i[18]; \
        (buf_out)[20] = i[19]; \
        (buf_out)[21] = i[20]; \
        (buf_out)[22] = i[21]; \
        (buf_out)[23] = i[22]; \
        (buf_out)[24] = i[23]; \
        (buf_out)[25] = i[24]; \
        (buf_out)[26] = i[25]; \
        (buf_out)[27] = i[26]; \
        (buf_out)[28] = i[27]; \
        (buf_out)[29] = i[28]; \
        (buf_out)[30] = i[29]; \
        (buf_out)[31] = i[30]; \
        (buf_out)[32] = i[31]; \
        (buf_out) += ENCODE_HASH256_COMMON_SIZE;\
    }
#define _05_XX_ENCODE_HASH256_COMMON(buf_out, tokenid, field)\
    ENCODE_HASH256_COMMON(buf_out, tokenid, field);

// NFT TOKEN ID
#define ENCODE_TOKEN_ID_SIZE 33U
#define ENCODE_TOKEN_ID(buf_out, nftokenid)\
    ENCODE_HASH256_COMMON(buf_out, nftokenid, 0xAU);
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
