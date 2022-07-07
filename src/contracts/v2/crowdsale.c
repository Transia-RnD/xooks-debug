/**
 * This hook just accepts any transaction coming through it
 */
 
#include "hookapi.h"
#include <stdint.h>

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

// NFT TOKEN ID
#define ENCODE_TOKEN_ID_SIZE 33U
#define ENCODE_TOKEN_ID(buf_out, nftokenid)\
    ENCODE_HASH256_COMMON(buf_out, nftokenid, 0xAU);
#define _05_10_ENCODE_TOKEN_ID(buf_out, nftokenid)\
    ENCODE_TOKEN_ID(buf_out, nftokenid);

// NFT SELL OFFER
#define PREPARE_NFT_CREATE_OFFER_SELL_SIZE 271U // 133 + 116 + 22
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
    _06_01_ENCODE_DROPS_AMOUNT(buf_out, drops_amount_raw);                                  /* amount  | size   9 */\
    uint8_t* fee_ptr = buf_out;\
    _06_08_ENCODE_DROPS_FEE(buf_out, 0);                                                    /* amount  | size   9 */\
    _07_03_ENCODE_SIGNING_PUBKEY_NULL(buf_out);                                             /* pk      | size  35 */\
    _08_01_ENCODE_ACCOUNT_SRC(buf_out, acc);                                                /* account | size  22 */\
    int64_t edlen = etxn_details((uint32_t)buf_out, PREPARE_NFT_CREATE_OFFER_SELL_SIZE);    /* emitdet | size 1?? */\
    int64_t fee = etxn_fee_base(buf_out_master, PREPARE_NFT_CREATE_OFFER_SELL_SIZE);\ 
    _06_08_ENCODE_DROPS_FEE(fee_ptr, fee);\
}

// VL COMMON
#define ENCODE_VL_UNCOMMON(buf_out, vl, vl_len, field, field2)\
    {\
        buf_out[0] = 0x75U;\
        buf_out[1] = field;\
        buf_out[2] = field2;\
        *(uint64_t*)(buf_out +  2) = *(uint64_t*)(vl +  0);\
        *(uint64_t*)(buf_out + 10) = *(uint64_t*)(vl +  8);\
        *(uint64_t*)(buf_out + 18) = *(uint64_t*)(vl + 16);\
        *(uint64_t*)(buf_out + 26) = *(uint64_t*)(vl + 24);\
        *(uint64_t*)(buf_out + 34) = *(uint64_t*)(vl + 32);\
        *(uint64_t*)(buf_out + 42) = *(uint64_t*)(vl + 40);\
        *(uint64_t*)(buf_out + 50) = *(uint64_t*)(vl + 48);\
        buf_out += vl_len + 3;\
    }
#define _07_XX_ENCODE_VL_UNCOMMON(buf_out, vl, vl_len, field, field2)\
    ENCODE_VL_UNCOMMON(buf_out, vl, vl_len, field, field2)\

#define ENCODE_VL_COMMON(buf_out, vl, vl_len)\
    {\
        buf_out[0] = 0x75U;\
        buf_out[1] = vl_len;\
        *(uint64_t*)(buf_out +  2) = *(uint64_t*)(vl +  0);\
        *(uint64_t*)(buf_out + 10) = *(uint64_t*)(vl +  8);\
        *(uint64_t*)(buf_out + 18) = *(uint64_t*)(vl + 16);\
        *(uint64_t*)(buf_out + 26) = *(uint64_t*)(vl + 24);\
        *(uint64_t*)(buf_out + 34) = *(uint64_t*)(vl + 32);\
        *(uint64_t*)(buf_out + 42) = *(uint64_t*)(vl + 40);\
        *(uint64_t*)(buf_out + 50) = *(uint64_t*)(vl + 48);\
        buf_out += vl_len + 2;\
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
    }
#define _07_05_ENCODE_URI(buf_out, vl, vl_len)\
    ENCODE_URI(buf_out, vl, vl_len);

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

// NFT TOKEN TAXON
#define ENCODE_NFTOKEN_TAXON_SIZE 6U
#define ENCODE_NFTOKEN_TAXON(buf_out, taxon )\
    ENCODE_UINT32_UNCOMMON(buf_out, taxon, 0x2A );
#define _02_42_ENCODE_NFTOKEN_TAXON(buf_out, taxon )\
    ENCODE_NFTOKEN_TAXON(buf_out, taxon );

#define PREPARE_NFT_MINT_SIMPLE_SIZE 235U // 97 + 116
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

int64_t cbak(uint32_t reserved) {
    TRACESTR("Crowdsale.c: Called.");
    return 0;
}

int64_t hook(uint32_t reserved ) {

    TRACESTR("Crowdsale.c: Called.");

    // before we start calling hook-api functions we should tell the hook how many tx we intend to create
    // etxn_reserve(1); // we are going to emit 2 transactions
    
    // this api fetches the AccountID of the account the hook currently executing is installed on
    // since hooks can be triggered by both incoming and ougoing transactions this is important to know
    unsigned char hook_accid[20];
    hook_account((uint32_t)hook_accid, 20);

    // NB:
    //  almost all of the hook apis require a buffer pointer and buffer length to be supplied ... to make this a
    //  little easier to code a macro: `SBUF(your_buffer)` expands to `your_buffer, sizeof(your_buffer)`

    // next fetch the sfAccount field from the originating transaction
    uint8_t account_field[20];
    int32_t account_field_len = otxn_field(SBUF(account_field), sfAccount);
    if (account_field_len < 20)                                   // negative values indicate errors from every api
        rollback(SBUF("Crowdsale: sfAccount field missing!!!"), 1);  // this code could never be hit in prod
                                                                  // but it's here for completeness

    // compare the "From Account" (sfAccount) on the transaction with the account the hook is running on
    int equal = 0; BUFFER_EQUAL(equal, hook_accid, account_field, 20);
    if (equal)
    {
        // if the accounts are equal (memcmp == 0) the otxn was sent from the hook account to someone else
        // accept() it and end the hook execution here
        accept(SBUF("Crowdsale: Outgoing transaction"), 2);
    }

    // fetch the sent Amount
    // Amounts can be 384 bits or 64 bits. If the Amount is an XRP value it will be 64 bits.
    unsigned char amount_buffer[48];
    int64_t amount_len = otxn_field(SBUF(amount_buffer), sfAmount);
    int64_t drops_required = 10000000; // this will be the default
    int64_t sell_drops_raw = 10; // this will be the default

    if (amount_len != 8)
    {
        // you can trace the behaviour of your hook using the trace(buf, size, as_hex) api
        // which will output to xrpld's trace log
        TRACESTR("Crowdsale: Non-XRP transaction detected, accepting");
        accept(SBUF("Crowdsale: Non-XRP"), 2);
    }
    TRACESTR("Crowdsale: XRP transaction detected, calculating");
    // IDE: Parameter - Price Per Unit
    // name: price
    // value: 10000000 - IN DROPs

    // uint32_t drops_required = 0;
    // uint8_t price_name[] = { 0x70U, 0x72U, 0x69U, 0x63U, 0x65U };
    // uint8_t price_value[32];
    // int64_t d_req_len = hook_param(price_value, 32, price_name, 5);
    // drops_required = UINT32_FROM_BUF(price_value);

    // TRACEVAR(drops_required); // <- value

    // if (drops_required < 1)
    // {
    //     rollback(SBUF("Crowdsale: Price per unit must be more than 1"), 2);
    // }

    int64_t otxn_drops = AMOUNT_TO_DROPS(amount_buffer);
    if (otxn_drops < drops_required)
    {
        rollback(SBUF("Crowdsale: Ammount not equal to minimum"), 2);
    }

    int64_t num_tickets = otxn_drops / drops_required;
    // TRACEVAR(num_tickets);

    // if (num_tickets < 1)
    // {
    //     rollback(SBUF("Crowdsale: Must purchase at least one"), 2);
    // }

    // ----------------------------------------------------->
    // FETCH PARAMETERS

    // IDE: Parameters 1 - URI
    // name: uri
    // value: 697066733A2F2F516D614374444B5A4656767666756676626479346573745A626851483744586831364354707631686F776D424779

    uint8_t pname[] = { 0x75U, 0x72U, 0x69U };
    uint8_t uri_value[53];
    int64_t value_len = hook_param(uri_value, 53, pname, 3);

    // TRACEHEX(uri_value); // <- value

    // ----------------------------------------------------->
    // FETCH NEXT NF TOKEN PARAMS
    // Flags
    // Fee
    // Taxon
    // Sequence

    // Flags
    uint8_t tflags = tfBURNABLE + tfTRANSFERABLE;

    // Fee
    uint8_t fee[2] = { 0x00, 0x00 };
    
    // Taxon
    uint32_t nft_taxon = 0;
    uint8_t taxon[4] = { 0x00U, 0x00U, 0x00U, 0x01U };
    nft_taxon = UINT32_FROM_BUF(taxon);

    // Sequence
    uint32_t token_seq = 0;
    uint8_t keylet[34];
    if (util_keylet(SBUF(keylet), KEYLET_ACCOUNT, hook_accid, 20, 0, 0, 0, 0) != 34)
        rollback(SBUF("Crowdsale: Internal error, could not generate keylet"), 10);

    // then requesting XRPLD slot that keylet into a new slot for us
    int64_t slot_no = slot_set(SBUF(keylet), 0);
    if (slot_no < 0)
        rollback(SBUF("Crowdsale: Could not set keylet in slot"), 10);

    int64_t seq_slot = slot_subfield(slot_no, sfMintedNFTokens, 0);
    if (seq_slot < 0) {
        TRACESTR("Crowdsale: Could not find sfMintedNFTokens on hook account");
    } else {
        uint8_t seq_buf[4];
        seq_slot = slot(SBUF(seq_buf), seq_slot);
        if (seq_slot != 4)
            rollback(SBUF("Crowdsale: Could not fetch sequence from sfMintedNFTokens."), 80);

        // then conver the four byte buffer to an unsigned 32 bit integer
        token_seq = UINT32_FROM_BUF(seq_buf);
        // TRACEVAR(token_seq); // print the integer for debugging purposes
        // TRACEVAR(seq_slot);
    }

    etxn_reserve(num_tickets * 2); // we are going to emit 2 transactions
    for (int i = 0; GUARD(10), i < num_tickets; ++i)
    {
        if (i <= num_tickets) {
            // TRACEVAR(i);

            // ----------------------------------------------------->
            // BUILD MINT TX

            // TX LEN + ENCODING (2) + URI LEN <- BAD BAD SMELLY
            unsigned char mint_tx[PREPARE_NFT_MINT_SIMPLE_SIZE + 2 + value_len];
            // prepare tx
            PREPARE_NFT_MINT_SIMPLE(mint_tx, tflags, nft_taxon, uri_value, value_len);

            // TRACEHEX(mint_tx)

            // ----------------------------------------------------->
            // BUILD NEXT NF TOKEN ID
            unsigned char nftid_out[GET_NEXT_NFT_ID_SIZE];
            uint8_t *buf_out = nftid_out;
            token_seq += i; // Not i + 1 because the tx is already emitted
            nft_taxon = (nft_taxon ^ (384160001 * token_seq + 2459)) % 4294967296;
            GET_NEXT_NFT_ID(buf_out, tflags, fee, hook_accid, nft_taxon, token_seq);
            TRACEHEX(nftid_out);

            // ----------------------------------------------------->
            // BUILD SELL TX

            uint8_t flags = tfSELLTOKEN;

            unsigned char sell_tx[PREPARE_NFT_CREATE_OFFER_SELL_SIZE];
            // prepare tx
            PREPARE_NFT_CREATE_OFFER_SELL(
                sell_tx, 
                flags,
                hook_accid,
                nftid_out,
                sell_drops_raw
            );

            // TRACEHEX(sell_tx);

            // emit the mint transaction
            uint8_t mint_hash[32];
            int64_t mint_result = emit(SBUF(mint_hash), SBUF(mint_tx));
            TRACEVAR(mint_result);

            // emit the sell transaction
            uint8_t sell_hash[32];
            int64_t sell_result = emit(SBUF(sell_hash), SBUF(sell_tx));
            TRACEVAR(sell_result);
        }
    }

    accept (0,0,0); 

    // _g(1,1);   // every hook needs to import guard function and use it at least once
    // unreachable
    return 0;
}