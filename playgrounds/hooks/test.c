/**
 * This hook just accepts any transaction coming through it
 */
 
#include "hookapi.h"

#define ttNFTOKEN_MINT 25
#define ttNFTOKEN_OFFER_CREATE 27

#define tfSELLTOKEN 0x00000001UL
#define tfBURNABLE 0x00000001UL
#define tfTRANSFERABLE 0x00000008UL

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

// NFT TOKEN TAXON
#define ENCODE_TOKEN_ID_SIZE 33U
#define ENCODE_TOKEN_ID(buf_out, nftokenid)\
    ENCODE_HASH256_COMMON(buf_out, nftokenid, 0xAU);
#define _05_10_ENCODE_TOKEN_ID(buf_out, nftokenid)\
    ENCODE_TOKEN_ID(buf_out, nftokenid);

// NFT SELL OFFER
#define PREPARE_NFT_CREATE_OFFER_SELL_SIZE 293U // 155 + 116 + 22
#define PREPARE_NFT_CREATE_OFFER_SELL(\
    buf_out_master,\
    flags,\
    hook_accid,\
    nftokenid,\
    drops_amount_raw\
) \
{                                                                                            \
    uint8_t *buf_out = buf_out_master;                                                       \
    uint8_t acc[20];                                                                         \
    uint32_t cls = (uint32_t)ledger_seq();                                                   \
    hook_account(SBUF(acc));                                                                 \
    _01_02_ENCODE_TT(buf_out, ttNFTOKEN_OFFER_CREATE);                                      /* uint16  | size   3 */\
    _02_02_ENCODE_FLAGS(buf_out, flags);                                                    /* uint32  | size   5 */\
    _02_04_ENCODE_SEQUENCE(buf_out, 0);                                                     /* uint32  | size   5 */\
    _02_26_ENCODE_FLS(buf_out, cls + 1);                                                    /* uint32  | size   6 */\
    _02_27_ENCODE_LLS(buf_out, cls + 5);                                                    /* uint32  | size   6 */\
    _05_10_ENCODE_TOKEN_ID(buf_out, nftokenid);                                             /* amount  | size  33 */\
    _08_02_ENCODE_ACCOUNT_OWNER(buf_out, hook_accid);                                       /* account | size  22 */\
    _06_01_ENCODE_DROPS_AMOUNT(buf_out, drops_amount_raw);                                  /* amount  | size   9 */\
    uint8_t* fee_ptr = buf_out;\
    _06_08_ENCODE_DROPS_FEE(buf_out, 0);                                                    /* amount  | size   9 */\
    _07_03_ENCODE_SIGNING_PUBKEY_NULL(buf_out);                                             /* pk      | size  35 */\
    _08_01_ENCODE_ACCOUNT_SRC(buf_out, acc);                                                /* account | size  22 */\
    int64_t edlen = etxn_details((uint32_t)buf_out, PREPARE_NFT_CREATE_OFFER_SELL_SIZE);    /* emitdet | size 1?? */\
    int64_t fee = etxn_fee_base(buf_out_master, PREPARE_NFT_CREATE_OFFER_SELL_SIZE);                                \ 
    _06_08_ENCODE_DROPS_FEE(fee_ptr, fee);                                                                          \
}

int64_t cbak(uint32_t reserved) { 
    TRACESTR("Sell.c: Callback Called.");
    return 0;
}

int64_t hook(uint32_t reserved ) {

    TRACESTR("Sell.c: Called.");

    // uint8_t hook_accid[20] = {
    // 0x05U, 0xb5U, 0xf4U, 0x3aU, 0xf7U, 
    // 0x17U, 0xb8U, 0x19U, 0x48U, 0x49U, 0x1fU, 0xb7U, 0x07U, 0x9eU, 0x4fU, 0x17U, 0x3fU, 0x4eU, 0xceU, 0xb3U};
    
    unsigned char hook_accid[20];
    hook_account((uint32_t)hook_accid, 20);

    uint8_t pname[] = { 0x6EU, 0x66U, 0x74U, 0x6FU, 0x6BU, 0x65U, 0x6EU, 0x69U, 0x64U,  };
    uint8_t nftid_value[32];
    int64_t value_len = hook_param(nftid_value, 32, pname, 9);

    TRACEHEX(nftid_value); // <- value

    uint8_t flags = tfSELLTOKEN;
    int64_t sell_drops_raw = 10; // this will be the default

    // create a buffer to write the emitted transaction into
    // TX LEN + ENCODING + URI LEN
    unsigned char sell_tx[PREPARE_NFT_CREATE_OFFER_SELL_SIZE];
    // prepare mint xls20 tx
    PREPARE_NFT_CREATE_OFFER_SELL(
        sell_tx, 
        flags,
        hook_accid,
        nftid_value,
        sell_drops_raw
    );

    TRACEHEX(sell_tx);

    accept (0,0,0); 

    _g(1,1);   // every hook needs to import guard function and use it at least once
    // unreachable
    return 0;
}