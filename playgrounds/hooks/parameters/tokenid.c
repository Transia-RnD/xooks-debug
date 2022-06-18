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
#define ENCODE_HASH256_COMMON(buf_out, tokenid, field)\
    {\
        uint8_t ui = (*tokenid);\
        uint8_t uf = field;\
        (buf_out)[0] = 0x50U + (uf & 0x0FU);\
        (buf_out)[1] = (ui >> 24) & 0xFFU;\
        (buf_out)[2] = (ui >> 16) & 0xFFU;\
        (buf_out)[3] = (ui >> 8) & 0xFFU;\
        (buf_out)[4] = (ui >> 0) & 0xFFU;\
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