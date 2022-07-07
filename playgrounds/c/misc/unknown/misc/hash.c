#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define tfBURNABLE 0x00000001UL
#define tfTRANSFERABLE 0x00000008UL

// // HASH256 COMMON
// #define ENCODE_HASH256_COMMON_SIZE 33U
// #define ENCODE_HASH256_COMMON(buf_out, tokenid, field)\
//     {\
//         uint32_t ui = (*tokenid);\
//         uint8_t uf = field;\
//         (buf_out)[0] = 0x50U + (uf & 0x0FU);\
        // (buf_out)[2] = (ui >> 24) & 0xFFU;\
        // (buf_out)[3] = (ui >> 16) & 0xFFU;\
        // (buf_out)[4] = (ui >> 8) & 0xFFU;\
        // (buf_out)[5] = (ui >> 0) & 0xFFU;\
//         (buf_out) += ENCODE_HASH256_COMMON_SIZE;\
//     }
// #define _05_XX_ENCODE_HASH256_COMMON(buf_out, tokenid, field)\
//     ENCODE_HASH256_COMMON(buf_out, tokenid, field);

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
// #define ENCODE_TOKEN_ID_SIZE 3
#define ENCODE_TOKEN_ID(buf_out, nftokenid) \
    ENCODE_HASH256_COMMON(buf_out, nftokenid, 0xAU);
#define _05_10_ENCODE_TOKEN_ID(buf_out, nftokenid)\
    ENCODE_TOKEN_ID(buf_out, nftokenid);

// #define PREPARE_TEST_SIZE ?
#define PREPARE_TEST(buf_out_master, tokenid)  \
    {                                          \
        u_int8_t *buf_out = buf_out_master;    \
        _05_10_ENCODE_TOKEN_ID(buf_out, tokenid); \
    }

#define GET_NEXT_NFT_ID_SIZE 32U
#define GET_NEXT_NFT_ID(buf_out, flags, fee, hook_accid, taxon, sequence)\
    {\
        (buf_out)[0] = 0x00;\
        (buf_out)[1] = flags;\
        (buf_out)[2] = (fee)[0];\
        (buf_out)[3] = (fee)[1];\
        (buf_out)[4] = (hook_accid)[0];\
        (buf_out)[5] = (hook_accid)[1];\
        (buf_out)[6] = (hook_accid)[2];\
        (buf_out)[7] = (hook_accid)[3];\
        (buf_out)[8] = (hook_accid)[4];\
        (buf_out)[9] = (hook_accid)[5];\
        (buf_out)[10] = (hook_accid)[6];\
        (buf_out)[11] = (hook_accid)[7];\
        (buf_out)[12] = (hook_accid)[8];\
        (buf_out)[13] = (hook_accid)[9];\
        (buf_out)[14] = (hook_accid)[10];\
        (buf_out)[15] = (hook_accid)[11];\
        (buf_out)[16] = (hook_accid)[12];\
        (buf_out)[17] = (hook_accid)[13];\
        (buf_out)[18] = (hook_accid)[14];\
        (buf_out)[19] = (hook_accid)[15];\
        (buf_out)[20] = (hook_accid)[16];\
        (buf_out)[21] = (hook_accid)[17];\
        (buf_out)[22] = (hook_accid)[18];\
        (buf_out)[23] = (hook_accid)[19];\
        (buf_out)[24] = ((taxon) >> 24) & 0xFFU;\
        (buf_out)[25] = ((taxon) >> 16) & 0xFFU;\
        (buf_out)[26] = ((taxon) >> 8) & 0xFFU;\
        (buf_out)[27] = ((taxon) >> 0) & 0xFFU;\
        (buf_out)[28] = ((sequence) >> 24) & 0xFFU;\
        (buf_out)[29] = ((sequence) >> 16) & 0xFFU;\
        (buf_out)[30] = ((sequence) >> 8) & 0xFFU;\
        (buf_out)[31] = ((sequence) >> 0) & 0xFFU;\
        (buf_out) += GET_NEXT_NFT_ID_SIZE;\
    }

// TEST ANY TX
int test(void)
{

    unsigned char tx_out[33];

    uint8_t hook_accid[20] = {
    0x05U, 0xb5U, 0xf4U, 0x3aU, 0xf7U, 
    0x17U, 0xb8U, 0x19U, 0x48U, 0x49U, 0x1fU, 0xb7U, 0x07U, 0x9eU, 0x4fU, 0x17U, 0x3fU, 0x4eU, 0xceU, 0xb3U};

    uint8_t fee[2] = { 0x00, 0x00 };
    uint8_t taxon[4] = { 0x00, 0x00, 0x00, 0x00 };
    uint8_t flags = tfBURNABLE + tfTRANSFERABLE;

    unsigned char id_out[GET_NEXT_NFT_ID_SIZE];
    uint8_t *buf_out = id_out;
    GET_NEXT_NFT_ID(buf_out, flags, fee, hook_accid, *taxon, 1);
    // TRACEHEX(id_out);

    for (int i = 0; i < sizeof(id_out); i++)
    {
        char hex_string[20];
        sprintf(hex_string, "%02X", id_out[i]);
        printf("%s", hex_string);
    }

    printf("\nTOKEN\n");

    PREPARE_TEST(tx_out, id_out);

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
