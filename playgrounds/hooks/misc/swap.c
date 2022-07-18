/**
 * This hook just accepts any transaction coming through it
 */
 
#define HAS_CALLBACK 1
#include "hookapi.h"
#include <stdint.h>

#define ttNFTOKEN_MINT 25
#define ttNFTOKEN_BURN 26
#define ttNFTOKEN_ACCEPT_OFFER 29
#define ttNFTOKEN_CREATE_OFFER 29

#define tfSELLTOKEN 0x00000001UL
#define tfBURNABLE 0x00000001UL
#define tfTRANSFERABLE 0x00000008UL

#define MAX_MEMO_SIZE 4096

#define COPY_BUFM(lhsbuf, lhsbuf_spos, rhsbuf, rhsbuf_spos, len, n) \
    for (int i = 0; GUARDM(len, n), i < len; ++i)                   \
        lhsbuf[lhsbuf_spos + i] = rhsbuf[rhsbuf_spos + i];

// GENERATE NFTOKEN OFFER INDEX
#define GENERATE_NFTOKEN_OFFER_INDEX_SIZE 26U
#define GENERATE_NFTOKEN_OFFER_INDEX(buf_out_master, hook_accid, sequence)\
    {\
        uint8_t* buf_out = buf_out_master;\
        (buf_out)[0] = 0x00U; \
        (buf_out)[1] = 0x71U; \
        (buf_out)[2] = (hook_accid)[0];\
        (buf_out)[3] = (hook_accid)[1];\
        (buf_out)[4] = (hook_accid)[2];\
        (buf_out)[5] = (hook_accid)[3];\
        (buf_out)[6] = (hook_accid)[4];\
        (buf_out)[7] = (hook_accid)[5];\
        (buf_out)[8] = (hook_accid)[6];\
        (buf_out)[9] = (hook_accid)[7];\
        (buf_out)[10] = (hook_accid)[8];\
        (buf_out)[11] = (hook_accid)[9];\
        (buf_out)[12] = (hook_accid)[10];\
        (buf_out)[13] = (hook_accid)[11];\
        (buf_out)[14] = (hook_accid)[12];\
        (buf_out)[15] = (hook_accid)[13];\
        (buf_out)[16] = (hook_accid)[14];\
        (buf_out)[17] = (hook_accid)[15];\
        (buf_out)[18] = (hook_accid)[16];\
        (buf_out)[19] = (hook_accid)[17];\
        (buf_out)[20] = (hook_accid)[18];\
        (buf_out)[21] = (hook_accid)[19];\
        (buf_out)[22] = (sequence)[0];\
        (buf_out)[23] = (sequence)[1];\
        (buf_out)[24] = (sequence)[2];\
        (buf_out)[25] = (sequence)[3];\
        (buf_out) += GENERATE_NFTOKEN_OFFER_INDEX_SIZE;\
    }

// HASH256 UNCOMMON
#define ENCODE_HASH256_UNCOMMON_SIZE 34U
#define ENCODE_HASH256_UNCOMMON(buf_out, i, field)\
    {\
        uint8_t uf = field; \
        buf_out[0] = 0x50U; \
        buf_out[1] = uf; \
        (buf_out)[2] = i[0]; \
        (buf_out)[3] = i[1]; \
        (buf_out)[4] = i[2]; \
        (buf_out)[5] = i[3]; \
        (buf_out)[6] = i[4]; \
        (buf_out)[7] = i[5]; \
        (buf_out)[8] = i[6]; \
        (buf_out)[9] = i[7]; \
        (buf_out)[10] = i[8]; \
        (buf_out)[11] = i[9]; \
        (buf_out)[12] = i[10]; \
        (buf_out)[13] = i[11]; \
        (buf_out)[14] = i[12]; \
        (buf_out)[15] = i[13]; \
        (buf_out)[16] = i[14]; \
        (buf_out)[17] = i[15]; \
        (buf_out)[18] = i[16]; \
        (buf_out)[19] = i[17]; \
        (buf_out)[20] = i[18]; \
        (buf_out)[21] = i[19]; \
        (buf_out)[22] = i[20]; \
        (buf_out)[23] = i[21]; \
        (buf_out)[24] = i[22]; \
        (buf_out)[25] = i[23]; \
        (buf_out)[26] = i[24]; \
        (buf_out)[27] = i[25]; \
        (buf_out)[28] = i[26]; \
        (buf_out)[29] = i[27]; \
        (buf_out)[30] = i[28]; \
        (buf_out)[31] = i[29]; \
        (buf_out)[32] = i[30]; \
        (buf_out)[33] = i[31]; \
        (buf_out) += ENCODE_HASH256_UNCOMMON_SIZE;\
    }
#define _05_XX_ENCODE_HASH256_UNCOMMON(buf_out, hash, field)\
    ENCODE_HASH256_UNCOMMON(buf_out, hash, field);
    

// NFTOKEN SELL OFFER
#define ENCODE_NFTOKEN_OFFER_SIZE 34U
#define ENCODE_NFTOKEN_OFFER(buf_out, hash)\
    ENCODE_HASH256_UNCOMMON(buf_out, hash, 0x1DU);
#define _05_29_ENCODE_NFTOKEN_SELL_OFFER(buf_out, hash)\
    ENCODE_NFTOKEN_OFFER(buf_out, hash);

#ifdef HAS_CALLBACK
#define PREPARE_NFT_ACCEPT_SELL_SIMPLE_SIZE 258U // 116 + 120 + 22
#else
#define PREPARE_NFT_ACCEPT_SELL_SIMPLE_SIZE 236U // 116 + 120
#endif
#define PREPARE_NFT_ACCEPT_SELL_SIMPLE(buf_out_master, index_hash) \
{                                                                                            \
    uint8_t *buf_out = buf_out_master;                                                       \
    uint8_t acc[20];                                                                         \
    uint32_t cls = (uint32_t)ledger_seq();                                                   \
    hook_account(SBUF(acc));                                                                 \
    _01_02_ENCODE_TT(buf_out, ttNFTOKEN_ACCEPT_OFFER);                                      /* uint16  | size   3 */\
    _02_04_ENCODE_SEQUENCE(buf_out, 0);                                                     /* uint32  | size   5 */\
    _02_26_ENCODE_FLS(buf_out, cls + 1);                                                    /* uint32  | size   6 */\
    _02_27_ENCODE_LLS(buf_out, cls + 5);                                                    /* uint32  | size   6 */\
    _05_29_ENCODE_NFTOKEN_SELL_OFFER(buf_out, index_hash);                                  /* hash256 | size  34 */\
    _06_08_ENCODE_DROPS_FEE(buf_out, 1000);                                                 /* amount  | size   9 */\
    _07_03_ENCODE_SIGNING_PUBKEY_NULL(buf_out);                                             /* pk      | size  35 */\
    _08_01_ENCODE_ACCOUNT_SRC(buf_out, acc);                                                /* account | size  22 */\
    int64_t edlen = etxn_details((uint32_t)buf_out, PREPARE_NFT_ACCEPT_SELL_SIMPLE_SIZE);   /* emitdet | size 1?? */\
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

// NFT TOKEN ID
#define ENCODE_TOKEN_ID_SIZE 33U
#define ENCODE_TOKEN_ID(buf_out, nftokenid)\
    ENCODE_HASH256_COMMON(buf_out, nftokenid, 0xAU);
#define _05_10_ENCODE_TOKEN_ID(buf_out, nftokenid)\
    ENCODE_TOKEN_ID(buf_out, nftokenid);

#ifdef HAS_CALLBACK
#define PREPARE_NFT_BURN_SIMPLE_SIZE 257U // 116 + 119 + 22
#else
#define PREPARE_NFT_BURN_SIMPLE_SIZE 235U // 116 + 119
#endif
#define PREPARE_NFT_BURN_SIMPLE(buf_out_master, nftokenid) \
{                                                                                            \
    uint8_t *buf_out = buf_out_master;                                                       \
    uint8_t acc[20];                                                                         \
    uint32_t cls = (uint32_t)ledger_seq();                                                   \
    hook_account(SBUF(acc));                                                                 \
    _01_02_ENCODE_TT(buf_out, ttNFTOKEN_BURN);                                              /* uint16  | size   3 */\
    _02_04_ENCODE_SEQUENCE(buf_out, 0);                                                     /* uint32  | size   5 */\
    _02_26_ENCODE_FLS(buf_out, cls + 1);                                                    /* uint32  | size   6 */\
    _02_27_ENCODE_LLS(buf_out, cls + 5);                                                    /* uint32  | size   6 */\
    _05_10_ENCODE_TOKEN_ID(buf_out, nftokenid);                                             /* hash256 | size  33 */\
    _06_08_ENCODE_DROPS_FEE(buf_out, 1000);                                                 /* amount  | size   9 */\
    _07_03_ENCODE_SIGNING_PUBKEY_NULL(buf_out);                                             /* pk      | size  35 */\
    _08_01_ENCODE_ACCOUNT_SRC(buf_out, acc);                                                /* account | size  22 */\
    int64_t edlen = etxn_details((uint32_t)buf_out, PREPARE_NFT_BURN_SIMPLE_SIZE);          /* emitdet | size 1?? */\
}

// VL UNUNCOMMON
#define ENCODE_VL_UNUNCOMMON(buf_out, vl, vl_len, field, field2, field3)\
    {\
        buf_out[0] = 0x75U;\
        buf_out[1] = field;\
        buf_out[2] = field2;\
        buf_out[3] = field3;\
        uint8_t *ptr = (uint8_t *)&vl;\
        COPY_BUFM(buf_out, 4, ptr, 0, vl_len, vl_len);\
        buf_out += vl_len;\
    }
#define _07_XX_ENCODE_VL_UNUNCOMMON(buf_out, vl, vl_len, field, field2, field3)\
    ENCODE_VL_UNUNCOMMON(buf_out, vl, vl_len, field, field2, field3)\

// VL UNCOMMON
#define ENCODE_VL_UNCOMMON(buf_out, vl, vl_len, field, field2)\
    {\
        buf_out[0] = 0x75U;\
        buf_out[1] = field;\
        buf_out[2] = field2;\
        uint8_t *ptr = (uint8_t *)&vl;\
        COPY_BUFM(buf_out, 3, ptr, 0, vl_len, vl_len);\
        buf_out += vl_len;\
    }
#define _07_XX_ENCODE_VL_UNCOMMON(buf_out, vl, vl_len, field, field2)\
    ENCODE_VL_UNCOMMON(buf_out, vl, vl_len, field, field2)\

// VL COMMON
#define ENCODE_VL_COMMON(buf_out, vl, vl_len)\
    {\
        buf_out[0] = 0x75U;\
        buf_out[1] = vl_len;\
        uint8_t *ptr = (uint8_t *)&vl;\
        COPY_BUFM(buf_out, 2, ptr, 0, vl_len, vl_len);\
        buf_out += vl_len;\
    }
#define _07_XX_ENCODE_VL_COMMON(buf_out, vl, vl_len)\
    ENCODE_VL_COMMON(buf_out, vl, vl_len)\

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
    }\
    else if (vl_len <= 918744) {\
        vl_len -= 12481;\
        int byte1 = 241 + (vl_len >> 16);\
        int byte2 = (vl_len >> 8) & 0xFFU;\
        int byte3 = vl_len & 0xFFU;\
        ENCODE_VL_UNUNCOMMON(buf_out, vl, vl_len, byte1, byte2, byte3);\
    }

#define _07_05_ENCODE_URI(buf_out, vl, vl_len)\
    ENCODE_URI(buf_out, vl, vl_len);

// NFT TOKEN TAXON
#define ENCODE_NFTOKEN_TAXON_SIZE 6U
#define ENCODE_NFTOKEN_TAXON(buf_out, taxon )\
    ENCODE_UINT32_UNCOMMON(buf_out, taxon, 0x2A );
#define _02_42_ENCODE_NFTOKEN_TAXON(buf_out, taxon )\
    ENCODE_NFTOKEN_TAXON(buf_out, taxon );

#ifdef HAS_CALLBACK
#define PREPARE_NFT_MINT_SIMPLE_SIZE 257U // 116 + 97 + ??(VL) + 22
#else
#define PREPARE_NFT_MINT_SIMPLE_SIZE 235U // 116 + 97 + ??(VL)
#endif
#define PREPARE_NFT_MINT_SIMPLE(buf_out_master, flags, taxon, uri, uri_len) \
{                                                                                            \
    uint8_t *buf_out = buf_out_master;                                                       \
    uint8_t acc[20];                                                                         \
    uint32_t cls = (uint32_t)ledger_seq();                                                   \
    hook_account(SBUF(acc));                                                                 \
    _01_02_ENCODE_TT(buf_out, ttNFTOKEN_MINT);                                              /* uint16  | size   3 */\
    _02_02_ENCODE_FLAGS(buf_out, flags);                                                    /* uint32  | size   5 */\
    _02_04_ENCODE_SEQUENCE(buf_out, 0);                                                     /* uint32  | size   5 */\
    _02_26_ENCODE_FLS(buf_out, cls + 1);                                                    /* uint32  | size   6 */\
    _02_27_ENCODE_LLS(buf_out, cls + 5);                                                    /* uint32  | size   6 */\
    _02_42_ENCODE_NFTOKEN_TAXON(buf_out, taxon);                                            /* amount  | size   6 */\
    _07_05_ENCODE_URI(buf_out, uri, uri_len);                                               /* vl     |  size   ? */\
    _06_08_ENCODE_DROPS_FEE(buf_out, 120);                                                  /* amount  | size   9 */\
    uint8_t* fee_ptr = buf_out;\
    _07_03_ENCODE_SIGNING_PUBKEY_NULL(buf_out);                                             /* pk      | size  35 */\
    _08_01_ENCODE_ACCOUNT_SRC(buf_out, acc);                                                /* account | size  22 */\
    int64_t edlen = etxn_details((uint32_t)buf_out, PREPARE_NFT_MINT_SIMPLE_SIZE);          /* emitdet | size 1?? */\
}

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

// NFT SELL OFFER
#ifdef HAS_CALLBACK
#define PREPARE_NFT_CREATE_OFFER_SELL_SIZE 320U // 116 + 182 + 22
#else
#define PREPARE_NFT_CREATE_OFFER_SELL_SIZE 298U // 116 + 182
#endif
#define PREPARE_NFT_CREATE_OFFER_SELL(\
    buf_out_master,\
    flags,\
    dest_accid,\
    nftokenid,\
    drops_amount_raw\
) \
{                                                                                            \
    uint8_t *buf_out = buf_out_master;                                                       \
    uint8_t acc[20];                                                                         \
    uint32_t cls = (uint32_t)ledger_seq();                                                   \
    hook_account(SBUF(acc));                                                                 \
    _01_02_ENCODE_TT(buf_out, ttNFTOKEN_CREATE_OFFER);                                      /* uint16  | size   3 */\
    _02_02_ENCODE_FLAGS(buf_out, flags);                                                    /* uint32  | size   5 */\
    _02_04_ENCODE_SEQUENCE(buf_out, 0);                                                     /* uint32  | size   5 */\
    _02_26_ENCODE_FLS(buf_out, cls + 1);                                                    /* uint32  | size   6 */\
    _02_27_ENCODE_LLS(buf_out, cls + 5);                                                    /* uint32  | size   6 */\
    _08_03_ENCODE_ACCOUNT_DST(buf_out, dest_accid);                                         /* account | size  22 */\
    _05_10_ENCODE_TOKEN_ID(buf_out, nftokenid);                                             /* hash256 | size  33 */\
    _06_01_ENCODE_DROPS_AMOUNT(buf_out, drops_amount_raw);                                  /* amount  | size   9 */\
    uint8_t* fee_ptr = buf_out;\
    _06_08_ENCODE_DROPS_FEE(buf_out, 0);                                                    /* amount  | size   9 */\
    _07_03_ENCODE_SIGNING_PUBKEY_NULL(buf_out);                                             /* pk      | size  35 */\
    _08_01_ENCODE_ACCOUNT_SRC(buf_out, acc);                                                /* account | size  22 */\
    int64_t edlen = etxn_details((uint32_t)buf_out, PREPARE_NFT_CREATE_OFFER_SELL_SIZE);    /* emitdet | size 1?? */\
    int64_t fee = etxn_fee_base(buf_out_master, PREPARE_NFT_CREATE_OFFER_SELL_SIZE);\ 
    _06_08_ENCODE_DROPS_FEE(fee_ptr, fee);\
}

// HASH256 FROM BUFFER
#define HASH256_FROM_BUFFER_SIZE 32U
#define HASH256_FROM_BUFFER(buf_out, i)\
    {\
        (buf_out)[0] = i[0]; \
        (buf_out)[1] = i[1]; \
        (buf_out)[2] = i[2]; \
        (buf_out)[3] = i[3]; \
        (buf_out)[4] = i[4]; \
        (buf_out)[5] = i[5]; \
        (buf_out)[6] = i[6]; \
        (buf_out)[7] = i[7]; \
        (buf_out)[8] = i[8]; \
        (buf_out)[9] = i[9]; \
        (buf_out)[10] = i[10]; \
        (buf_out)[11] = i[11]; \
        (buf_out)[12] = i[12]; \
        (buf_out)[13] = i[13]; \
        (buf_out)[14] = i[14]; \
        (buf_out)[15] = i[15]; \
        (buf_out)[16] = i[16]; \
        (buf_out)[17] = i[17]; \
        (buf_out)[18] = i[18]; \
        (buf_out)[19] = i[19]; \
        (buf_out)[20] = i[20]; \
        (buf_out)[21] = i[21]; \
        (buf_out)[22] = i[22]; \
        (buf_out)[23] = i[23]; \
        (buf_out)[24] = i[24]; \
        (buf_out)[25] = i[25]; \
        (buf_out)[26] = i[26]; \
        (buf_out)[27] = i[27]; \
        (buf_out)[28] = i[28]; \
        (buf_out)[29] = i[29]; \
        (buf_out)[30] = i[30]; \
        (buf_out)[31] = i[31]; \
        (buf_out) += HASH256_FROM_BUFFER_SIZE;\
    }

int64_t cbak(uint32_t reserved) {
    TRACESTR("Swap.c: Called.");
    return 0;
}

int64_t hook(uint32_t reserved ) {

    etxn_reserve(2); // we are going to emit 2 transactions

    // 0. COMMON - GET ACCOUNT
    TRACESTR("Swap.c: Called.");

    unsigned char hook_accid[20];
    hook_account((uint32_t)hook_accid, 20);

    uint8_t account_field[20];
    int32_t account_field_len = otxn_field(SBUF(account_field), sfAccount);
    if (account_field_len < 20) 
        rollback(SBUF("Swap: sfAccount field missing!!!"), 1);

    // 0.1 COMMON CATCH
    // INCOMING: NFTSellOffer
    // INCOMING CURRENCY: XRP
    // OUTGOING: NONE
    // OUTGOING CURRENCY: NONE
    int equal = 0; BUFFER_EQUAL(equal, hook_accid, account_field, 20);
    if (equal)
    {
        accept(SBUF("Swap: Outgoing transaction"), 2);
    }

    // ----------------------------------------------------->
    // VALIDATE MEMO
    
    uint8_t memos[MAX_MEMO_SIZE];
    int64_t memos_len = otxn_field(SBUF(memos), sfMemos);

    if (memos_len < 0)
    {
        accept(SBUF("Swap: Invalid Memo transaction"), 2);
    }

    int64_t   memo_lookup = sto_subarray(memos, memos_len, 0);
    uint8_t*  memo_ptr = SUB_OFFSET(memo_lookup) + memos;
    uint32_t  memo_len = SUB_LENGTH(memo_lookup);

    memo_lookup = sto_subfield(memo_ptr, memo_len, sfMemo);
    memo_ptr = SUB_OFFSET(memo_lookup) + memo_ptr;
    memo_len = SUB_LENGTH(memo_lookup);

    if (memo_lookup < 0)
        rollback(SBUF("Notary: Incoming txn had a blank sfMemos, abort."), 1);

    int64_t  format_lookup   = sto_subfield(memo_ptr, memo_len, sfMemoFormat);
    uint8_t* format_ptr = SUB_OFFSET(format_lookup) + memo_ptr;
    uint32_t format_len = SUB_LENGTH(format_lookup);

    int is_unsigned_payload = 0;
    BUFFER_EQUAL_STR_GUARD(is_unsigned_payload, format_ptr, format_len, "unsigned/payload+1", 1);
    if (!is_unsigned_payload)
        accept(SBUF("Swap: Memo is an invalid format. Passing txn."), 50);

    int64_t  data_lookup = sto_subfield(memo_ptr, memo_len, sfMemoData);
    uint8_t* uri_ptr = SUB_OFFSET(data_lookup) + memo_ptr;
    uint32_t uri_len = SUB_LENGTH(data_lookup);

    if (uri_len > MAX_MEMO_SIZE)
        rollback(SBUF("Swap: Memo too large (4kib max)."), 4);

    TRACEVAR(uri_len);
    TRACEHEX(uri_ptr);

    uint8_t uri_value[uri_len];
    uint8_t *buf_out = uri_value;
    HASH256_FROM_BUFFER(buf_out, uri_ptr);

    TRACEHEX(buf_out)

    // ----------------------------------------------------->
    // GET AMOUNT TYPE (XRP or IC)

    unsigned char amount_buffer[48];
    int64_t amount_len = otxn_field(SBUF(amount_buffer), sfAmount);
    int64_t sell_drops_raw = 7;
    if (amount_len != 8)
    {
        TRACESTR("Swap: Non-XRP transaction detected, accepting");
        accept(SBUF("Swap: Non-XRP"), 2);
    }
    TRACESTR("Swap: XRP transaction detected, calculating");
    int64_t otxn_drops = AMOUNT_TO_DROPS(amount_buffer);

    TRACEVAR(sell_drops_raw);
    TRACEVAR(otxn_drops);
    
    if (sell_drops_raw != otxn_drops) 
    {
        accept(SBUF("Swap: Invalid Swap Amount"), 2);
    }

    // ----------------------------------------------------->
    // BUILD ACCEPT TX

    uint8_t account_seq[4];
    int32_t account_seq_len = otxn_field(SBUF(account_seq), sfSequence);
    if (account_seq_len < 4)
        rollback(SBUF("Swap: sfSequence field missing!!!"), 1);
    
    // uint8_t keylet[34];
    // if (util_keylet(SBUF(keylet), KEYLET_NFTOKEN_OFFER, hook_accid, 20, account_seq, 5, 0, 0) != 34)
    //     rollback(SBUF("MintedTokens: Internal error, could not generate keylet"), 10);

    unsigned char offer_index[GENERATE_NFTOKEN_OFFER_INDEX_SIZE];
    GENERATE_NFTOKEN_OFFER_INDEX(offer_index, account_field, account_seq);

    uint8_t hash_out[32];
    if (util_sha512h(hash_out, 32, offer_index, GENERATE_NFTOKEN_OFFER_INDEX_SIZE) < 0)
        rollback("Swap: Could not generate Hash", 23, 1);

    TRACEHEX(hash_out);
    
    unsigned char accept_tx[PREPARE_NFT_ACCEPT_SELL_SIMPLE_SIZE];
    PREPARE_NFT_ACCEPT_SELL_SIMPLE(
        accept_tx,
        hash_out
    );

    TRACEHEX(accept_tx);

    // emit the accept transaction
    uint8_t accept_hash[32];
    int64_t accept_result = emit(SBUF(accept_hash), SBUF(accept_tx));
    TRACEVAR(accept_result);

    // ----------------------------------------------------->
    // BUILD BURN TX

    uint8_t nftokenid[32];
    int32_t nftokenid_len = otxn_field(SBUF(nftokenid), sfNFTokenID);
    if (nftokenid_len < 32)
        rollback(SBUF("Swap: sfNFTokenID field missing!!!"), 1);

    unsigned char burn_tx[PREPARE_NFT_BURN_SIMPLE_SIZE];
    PREPARE_NFT_BURN_SIMPLE(
        burn_tx,
        nftokenid
    );

    TRACEHEX(burn_tx);

    // emit the burn transaction
    uint8_t burn_hash[32];
    int64_t burn_result = emit(SBUF(burn_hash), SBUF(burn_tx));
    TRACEVAR(burn_result);

    // ----------------------------------------------------->
    // BUILD MINT TX

    // Taxon
    uint32_t nft_taxon = 0;
    uint8_t taxon[4] = { 0x00U, 0x00U, 0x00U, 0x01U };
    nft_taxon = UINT32_FROM_BUF(taxon);

    // Flags
    uint8_t mint_flags = tfBURNABLE + tfTRANSFERABLE;

    // Fee
    uint8_t mint_fee[2] = { 0x00, 0x00 };

    // // TX LEN + ENCODING (2) + URI LEN <- BAD BAD SMELLY
    unsigned char mint_tx[PREPARE_NFT_MINT_SIMPLE_SIZE + 2 + uri_len];
    PREPARE_NFT_MINT_SIMPLE(
        mint_tx,
        mint_flags,
        nft_taxon,
        uri_ptr,
        uri_len
    );

    // TRACEHEX(mint_tx);

    // // emit the mint transaction
    // uint8_t mint_hash[32];
    // int64_t mint_result = emit(SBUF(mint_hash), SBUF(mint_tx));
    // TRACEVAR(mint_result);

    // ----------------------------------------------------->
    // BUILD SELL TX
    // Sequence
    // uint32_t token_seq = 0;
    // uint8_t keylet[34];
    // if (util_keylet(SBUF(keylet), KEYLET_ACCOUNT, hook_accid, 20, 0, 0, 0, 0) != 34)
    //     rollback(SBUF("Crowdsale: Internal error, could not generate keylet"), 10);

    // // then requesting XRPLD slot that keylet into a new slot for us
    // int64_t slot_no = slot_set(SBUF(keylet), 0);
    // if (slot_no < 0)
    //     rollback(SBUF("Crowdsale: Could not set keylet in slot"), 10);

    // int64_t seq_slot = slot_subfield(slot_no, sfMintedNFTokens, 0);
    // if (seq_slot < 0) {
    //     TRACESTR("Crowdsale: Could not find sfMintedNFTokens on hook account");
    // } else {
    //     uint8_t seq_buf[4];
    //     seq_slot = slot(SBUF(seq_buf), seq_slot);
    //     if (seq_slot != 4)
    //         rollback(SBUF("Crowdsale: Could not fetch sequence from sfMintedNFTokens."), 80);

    //     token_seq = UINT32_FROM_BUF(seq_buf);
    // }

    // ----------------------------------------------------->
    // BUILD NEXT NF TOKEN ID
    // unsigned char nftid_out[GET_NEXT_NFT_ID_SIZE];
    // uint8_t *buf_out = nftid_out;
    // nft_taxon = (nft_taxon ^ (384160001 * token_seq + 2459)) % 4294967296;
    // GET_NEXT_NFT_ID(buf_out, mint_flags, mint_fee, hook_accid, nft_taxon, token_seq);
    // TRACEHEX(nftid_out);

    // uint8_t sell_flags = tfSELLTOKEN;

    // unsigned char sell_tx[PREPARE_NFT_CREATE_OFFER_SELL_SIZE];
    // prepare tx
    // PREPARE_NFT_CREATE_OFFER_SELL(
    //     sell_tx, 
    //     sell_flags,
    //     account_field,
    //     nftid_out,
    //     sell_drops_raw
    // );

    // TRACEHEX(sell_tx);

    // emit the sell transaction
    // uint8_t sell_hash[32];
    // int64_t sell_result = emit(SBUF(sell_hash), SBUF(sell_tx));
    // TRACEVAR(sell_result);

    accept (0,0,0); 

    // _g(1,1);   // every hook needs to import guard function and use it at least once
    // unreachable
    return 0;
}