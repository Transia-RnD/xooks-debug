#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ttNFTOKEN_MINT 25
#define ttNFTOKEN_OFFER_CREATE 27

#define tfSELLTOKEN 0x00000001UL
#define tfBURNABLE 0x00000001UL
#define tfTRANSFERABLE 0x00000008UL

// URI
// URI_SIZE is included in the macro.
#define ENCODE_URI(buf_out, vl, vl_len) \
    if (vl_len <= 193) {\
        ENCODE_VL_COMMON(buf_out, vl, vl_len);\
    }\
    else if (vl_len <= 12480) {\
        vl_len -= 193;\
        int byte1 = (vl_len >> 8) + 193;\
        int byte2 = vl_len & 0xFFU;\
        ENCODE_VL_UNCOMMON(buf_out, vl, vl_len, byte1, byte2);\
    else if (vl_len <= 918744) {\
        vl_len -= 12481;\
        int byte1 = 241 + (vl_len >> 16);\
        int byte2 = (vl_len >> 8) & 0xFFU;\
        int byte3 = vl_len & 0xFFU;\
        ENCODE_VL_UNUNCOMMON(buf_out, vl, vl_len, byte1, byte2, byte3);\
    }
#define _07_05_ENCODE_URI(buf_out, vl, vl_len)\
    ENCODE_URI(buf_out, vl, vl_len);

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
        (buf_out)[23] = i[21]; \
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

// NFT TOKEN TAXON
#define ENCODE_NFTOKEN_TAXON_SIZE 6
#define ENCODE_NFTOKEN_TAXON(buf_out, taxon )\
    ENCODE_UINT32_UNCOMMON(buf_out, taxon, 0x2A );
#define _02_42_ENCODE_NFTOKEN_TAXON(buf_out, taxon )\
    ENCODE_NFTOKEN_TAXON(buf_out, taxon );

// NEXT NFT ID
// TODO: Fix buf_out[1] => The flags should be 2 bytes. I think I pass in only 1
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

#define PREPARE_TEST_SIZE 33U
#define PREPARE_TEST(buf_out_master, nftid)  \
    {                                          \
        u_int8_t *buf_out = buf_out_master;    \
        _05_10_ENCODE_TOKEN_ID(buf_out, nftid); \
    }

#define UINT32_FROM_BUF(buf)\
    (((uint64_t)((buf)[0]) << 24) +\
     ((uint64_t)((buf)[1]) << 16) +\
     ((uint64_t)((buf)[2]) <<  8) +\
     ((uint64_t)((buf)[3]) <<  0))

// TEST ANY TX
int test(void)
{

    // TRACESTR("Sell.c: Called.");

    // HOOK ACCOUNT
    uint8_t hook_accid[20] = {
    0x05U, 0xb5U, 0xf4U, 0x3aU, 0xf7U, 
    0x17U, 0xb8U, 0x19U, 0x48U, 0x49U, 0x1fU, 0xb7U, 0x07U, 0x9eU, 0x4fU, 0x17U, 0x3fU, 0x4eU, 0xceU, 0xb3U};

    // FLAGS
    uint8_t tflags = tfBURNABLE + tfTRANSFERABLE;

    // FEE
    uint8_t fee[2] = { 0x00, 0x00 };

    // TAXON
    uint32_t nft_taxon = 0;
    uint8_t taxon[4] = { 0x00U, 0x00U, 0x00U, 0x01U };
    nft_taxon = UINT32_FROM_BUF(taxon);
    printf("TAXON: %d\n", nft_taxon);

    // SEQUENCE
    uint32_t token_seq = 0;
    uint8_t sequence[4] = { 0x00U, 0x00U, 0x00U, 0x01U };
    token_seq = UINT32_FROM_BUF(sequence);
    printf("SEQUENCE: %d\n", token_seq);

    // SCRAMBLE TAXON
    nft_taxon = (nft_taxon ^ (384160001 * token_seq + 2459)) % 4294967296;
    printf("SCRAMBLED TAXON: %d\n", nft_taxon);

    // GET NEXT NFT ID
    unsigned char nftid_out[GET_NEXT_NFT_ID_SIZE];
    uint8_t *buf_id_out = nftid_out;
    GET_NEXT_NFT_ID(buf_id_out, tflags, fee, hook_accid, nft_taxon, token_seq);
    printf("\nNFTID\n");

    for (int i = 0; i < sizeof(nftid_out); i++)
    {
        char hex_string[20];
        sprintf(hex_string, "%02X", nftid_out[i]);
        printf("%s", hex_string);
    }

    // unsigned char prepare_out[PREPARE_TEST_SIZE];
    // uint8_t *buf_out = prepare_out;
    // PREPARE_TEST(buf_out, nftid_out);

    // for (int i = 0; i < sizeof(prepare_out); i++)
    // {
    //     char hex_string[20];
    //     sprintf(hex_string, "%02X", prepare_out[i]);
    //     printf("%s", hex_string);
    // }
    
    printf("\nDONE\n");
    return 0;
}

int main(void)
{
  test();
  return 0;
}
