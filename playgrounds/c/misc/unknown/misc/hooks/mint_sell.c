/**
 * This hook just accepts any transaction coming through it
 */
 
#include "hookapi.h"

#define ttNFTOKEN_MINT 25
#define ttNFTOKEN_OFFER_CREATE 27

#define tfSELLTOKEN 0x00000001UL
#define tfBURNABLE 0x00000001UL
#define tfTRANSFERABLE 0x00000008UL

// NFT SELL OFFER
#define PREPARE_NFT_CREATE_OFFER_SELL_SIZE 293 // 155 + 116 + 22
#define PREPARE_NFT_CREATE_OFFER_SELL(\
    buf_out_master,\
    flags,\
    tflags,\
    fee,\
    hook_accid,\
    taxon,\
    sequence,\
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
    GET_NEXT_NFT_ID(buf_out, tflags, fee, hook_accid, taxon, sequence);                     /* amount  | size  33 */\
    _06_01_ENCODE_DROPS_AMOUNT(buf_out, drops_amount_raw);                                  /* amount  | size   9 */\
    uint8_t* fee_ptr = buf_out;\
    _08_02_ENCODE_ACCOUNT_OWNER(buf_out, hook_accid);                                       /* account | size  22 */\
    _06_08_ENCODE_DROPS_FEE(buf_out, 120);                                                  /* amount  | size   9 */\
     _07_03_ENCODE_SIGNING_PUBKEY_NULL(buf_out);                                            /* pk      | size  35 */\
    _08_01_ENCODE_ACCOUNT_SRC(buf_out, acc);                                                /* account | size  22 */\
    int64_t edlen = etxn_details((uint32_t)buf_out, PREPARE_NFT_MINT_SIMPLE_SIZE);          /* emitdet | size 1?? */\
}

// NFT TOKEN TAXON
#define ENCODE_NFTOKEN_TAXON_SIZE 6U
#define ENCODE_NFTOKEN_TAXON(buf_out, taxon )\
    ENCODE_UINT32_UNCOMMON(buf_out, taxon, 0x2A );
#define _02_42_ENCODE_NFTOKEN_TAXON(buf_out, taxon )\
    ENCODE_NFTOKEN_TAXON(buf_out, taxon );

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

#define GET_NEXT_NFT_ID_SIZE 33U
#define GET_NEXT_NFT_ID(buf_out, flags, fee, hook_accid, taxon, sequence)\
    {\
        uint8_t uf = 0xAU;\
        (buf_out)[0] = 0x50U + (uf & 0x0FU);\
        (buf_out)[1] = 0x00;\
        (buf_out)[2] = flags;\
        (buf_out)[3] = (fee)[0];\
        (buf_out)[4] = (fee)[1];\
        (buf_out)[5] = (hook_accid)[0];\
        (buf_out)[6] = (hook_accid)[1];\
        (buf_out)[7] = (hook_accid)[2];\
        (buf_out)[8] = (hook_accid)[3];\
        (buf_out)[9] = (hook_accid)[4];\
        (buf_out)[10] = (hook_accid)[5];\
        (buf_out)[11] = (hook_accid)[6];\
        (buf_out)[12] = (hook_accid)[7];\
        (buf_out)[13] = (hook_accid)[8];\
        (buf_out)[14] = (hook_accid)[9];\
        (buf_out)[15] = (hook_accid)[10];\
        (buf_out)[16] = (hook_accid)[11];\
        (buf_out)[17] = (hook_accid)[12];\
        (buf_out)[18] = (hook_accid)[13];\
        (buf_out)[19] = (hook_accid)[14];\
        (buf_out)[20] = (hook_accid)[15];\
        (buf_out)[21] = (hook_accid)[16];\
        (buf_out)[22] = (hook_accid)[17];\
        (buf_out)[23] = (hook_accid)[18];\
        (buf_out)[24] = (hook_accid)[19];\
        (buf_out)[25] = ((taxon) >> 24) & 0xFFU;\
        (buf_out)[26] = ((taxon) >> 16) & 0xFFU;\
        (buf_out)[27] = ((taxon) >> 8) & 0xFFU;\
        (buf_out)[28] = ((taxon) >> 0) & 0xFFU;\
        (buf_out)[29] = ((sequence) >> 24) & 0xFFU;\
        (buf_out)[30] = ((sequence) >> 16) & 0xFFU;\
        (buf_out)[31] = ((sequence) >> 8) & 0xFFU;\
        (buf_out)[32] = ((sequence) >> 0) & 0xFFU;\
        (buf_out) += GET_NEXT_NFT_ID_SIZE;\
    }

// URI
// #define ENCODE_URI_SIZE ?
#define ENCODE_URI(buf_out, vl, vl_len) \
    if (vl_len <= 192) {\
        ENCODE_VL_COMMON(buf_out, vl, vl_len);\
    }\
    else if (vl_len <= 12480) {\
        vl_len -= 193;\
        int byte1 = (vl_len >> 8) + 193;\
        int byte2 = vl_len & 0xFFU;\
        ENCODE_VL_UNCOMMON(buf_out, vl, byte1, byte2, vl_len);\
    }
#define _07_05_ENCODE_URI(buf_out, vl, vl_len)\
    ENCODE_URI(buf_out, vl, vl_len);

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
    TRACESTR("Accept.c: Callback Called.");
    return 0;
}

int64_t hook(uint32_t reserved)
{

    TRACESTR("Swap: started");

    // before we start calling hook-api functions we should tell the hook how many tx we intend to create
    etxn_reserve(2); // we are going to emit 2 transactions
    
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
    TRACEVAR(account_field_len);
    if (account_field_len < 20)                                   // negative values indicate errors from every api
        rollback(SBUF("Swap: sfAccount field missing!!!"), 1);  // this code could never be hit in prod
                                                                  // but it's here for completeness

    // compare the "From Account" (sfAccount) on the transaction with the account the hook is running on
    int equal = 0; BUFFER_EQUAL(equal, hook_accid, account_field, 20);
    if (equal)
    {
        // if the accounts are equal (memcmp == 0) the otxn was sent from the hook account to someone else
        // accept() it and end the hook execution here
        accept(SBUF("Swap: Outgoing transaction"), 2);
    }

    // fetch the sent Amount
    // Amounts can be 384 bits or 64 bits. If the Amount is an XRP value it will be 64 bits.
    unsigned char amount_buffer[48];
    int64_t amount_len = otxn_field(SBUF(amount_buffer), sfAmount);
    int64_t drops_required = 10000; // this will be the default
    int64_t sell_drops_raw = 10; // this will be the default


    if (amount_len != 8)
    {
        // you can trace the behaviour of your hook using the trace(buf, size, as_hex) api
        // which will output to xrpld's trace log
        TRACESTR("Swap: Non-XRP transaction detected, accepting");
        accept(SBUF("Swap: Non-XRP"), 2);
    } else
    {
        TRACESTR("Swap: XRP transaction detected, accepting");
        int64_t otxn_drops = AMOUNT_TO_DROPS(amount_buffer);
        TRACEVAR(otxn_drops);
        TRACEVAR(drops_required);
        if (drops_required > otxn_drops)
        {
            accept(SBUF("Swap: Ammount not equal to Requirement "), 2);
        }
        // ----------------------------------------------------->
        // FETCH PARAMETERS

        // IDE: Parameters
        // name: uri
        // value: 697066733a2f2f516d614374444b5a4656767666756676626479346573745a626851483744586831364354707631686f776d424779

        uint8_t pname[] = { 0x75U, 0x72U, 0x69U };
        uint8_t pvalue[53];
        int64_t value_len = hook_param(pvalue, 53, pname, 3);

        TRACEHEX(pvalue); // <- value
        TRACEVAR(value_len);

        // ----------------------------------------------------->
        // FETCH NEXT NFTOKEN ID

        // get the account the hook is running on and the account that created the txn
        uint8_t hook_accid[20];
        hook_account(SBUF(hook_accid));

        uint8_t keylet[34];
        if (util_keylet(SBUF(keylet), KEYLET_ACCOUNT, hook_accid, 20, 0, 0, 0, 0) != 34)
            rollback(SBUF("Accept: Internal error, could not generate keylet"), 10);

        // then requesting XRPLD slot that keylet into a new slot for us
        int64_t slot_no = slot_set(SBUF(keylet), 0);
        if (slot_no < 0)
        rollback(SBUF("Accept: Could not set keylet in slot"), 10);

        int64_t result = slot_subfield(slot_no, sfMintedNFTokens, 0);
        if (result < 0)
            rollback(SBUF("Accept: Could not find sfMintedNFTokens on hook account"), 20);

        uint8_t fee[2] = { 0x00, 0x00 };
        uint32_t taxon[4] = { 0x00U, 0x00U, 0x00U, 0x00U };
        uint8_t tflags = tfBURNABLE + tfTRANSFERABLE;

        TRACEVAR(result);

        unsigned char id_out[GET_NEXT_NFT_ID_SIZE];
        uint8_t *buf_out = id_out;
        GET_NEXT_NFT_ID(buf_out, tflags, fee, hook_accid, *taxon, result + 10);
        
        TRACEHEX(id_out);

        // ----------------------------------------------------->
        // BUILD MINT TX

        // create a buffer to write the emitted transaction into
        // TX LEN + ENCODING + URI LEN
        unsigned char mint_tx[PREPARE_NFT_MINT_SIMPLE_SIZE + 2 + value_len];
        // prepare mint xls20 tx
        PREPARE_NFT_MINT_SIMPLE(mint_tx, tflags, *taxon, pvalue, value_len);

        TRACEHEX(mint_tx)

        // emit the transaction
        uint8_t mint_hash[32];
        int64_t mint_result = emit(SBUF(mint_hash), SBUF(mint_tx));
        TRACEVAR(mint_result);

        // ----------------------------------------------------->
        // BUILD SELL TX

        uint8_t flags = tfSELLTOKEN;

        // create a buffer to write the emitted transaction into
        // TX LEN + ENCODING + URI LEN
        unsigned char sell_tx[PREPARE_NFT_CREATE_OFFER_SELL_SIZE];
        // prepare mint xls20 tx
        PREPARE_NFT_CREATE_OFFER_SELL(
            sell_tx, 
            flags,
            tflags,
            fee,
            hook_accid,
            *taxon,
            result + 1,
            sell_drops_raw
        );

        TRACEHEX(sell_tx);

        // emit the transaction
        uint8_t sell_hash[32];
        int64_t sell_result = emit(SBUF(sell_hash), SBUF(sell_tx));
        TRACEVAR(sell_result);

    }

    // accept and allow the original transaction through
    accept(SBUF("Swap: Emitted transaction"), 0);
    return 0;
}