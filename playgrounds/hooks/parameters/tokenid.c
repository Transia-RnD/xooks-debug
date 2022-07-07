/**
 * This hook just accepts any transaction coming through it
 */
 
#include "hookapi.h"

int64_t cbak(uint32_t reserved) { 
    TRACESTR("Sell.c: Callback Called.");
    return 0;
}

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

// NFTOKEN ID
#define ENCODE_TOKEN_ID_SIZE 33U
#define ENCODE_TOKEN_ID(buf_out, nftokenid) \
    ENCODE_HASH256_COMMON(buf_out, nftokenid, 0xAU);
#define _05_10_ENCODE_TOKEN_ID(buf_out, nftokenid)\
    ENCODE_TOKEN_ID(buf_out, nftokenid);

// #define PREPARE_NFT_ID_TEST_SIZE 33U
#define PREPARE_NFT_ID_TEST(buf_out_master, uri, uri_len)  \
    {                                          \
        uint8_t *buf_out = buf_out_master;    \
        _07_05_ENCODE_URI(buf_out, uri, uri_len); \
    }

int64_t hook(uint32_t reserved ) {

    TRACESTR("Sell.c: Called.");

    // IDE: Parameters
    // name: tokenid
    // value: 00090000E3E2649FB84DFB055036605B49DE305618BA72972DCBAB9D00000002

    uint8_t pname[] = { 0x6EU, 0x66U, 0x74U, 0x6FU, 0x6BU, 0x65U, 0x6EU, 0x69U, 0x64U,  };
    uint8_t nft_id_value[32];
    int64_t value_len = hook_param(nft_id_value, 32, pname, 9);

    TRACEHEX(nft_id_value); // <- value

    unsigned char tx_out[ENCODE_TOKEN_ID_SIZE];
    PREPARE_NFT_ID_TEST(tx_out, nft_id_value, value_len);

    accept (0,0,0); 

    _g(1,1);   // every hook needs to import guard function and use it at least once
    // unreachable
    return 0;
}