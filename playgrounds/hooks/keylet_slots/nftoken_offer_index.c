/**
 * This hook just accepts any transaction coming through it
 */
 
#include "hookapi.h"

#define KEYLET_NFTOKEN_PAGE 23
#define KEYLET_NFTOKEN_OFFER 24

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

int64_t cbak(uint32_t reserved) { 
    TRACESTR("NFTokenOfferIndex.c: Callback Called.");
    return 0;
}

int64_t hook(uint32_t reserved ) {

    TRACESTR("NFTokenOfferIndex.c: Called.");

    // get the account the hook is running on and the account that created the txn
    uint8_t hook_accid[20];
    hook_account(SBUF(hook_accid));

    TRACEHEX(hook_accid);

    uint8_t account_id[20];
    int32_t account_id_len = otxn_field(SBUF(account_id), sfAccount);
    if (account_id_len < 20)
        rollback(SBUF("NFTokenOfferIndex: sfAccount field missing!!!"), 1);

    uint8_t account_seq[4];
    int32_t account_seq_len = otxn_field(SBUF(account_seq), sfSequence);
    if (account_seq_len < 4)
        rollback(SBUF("NFTokenOfferIndex: sfSequence field missing!!!"), 1);

    TRACEVAR(account_seq);

    unsigned char offer_index[GENERATE_NFTOKEN_OFFER_INDEX_SIZE];
    // prepare tx
    GENERATE_NFTOKEN_OFFER_INDEX(offer_index, account_id, account_seq);

    TRACEHEX(offer_index);

    // uint8_t keylet[34];
    // if (util_keylet(SBUF(keylet), KEYLET_NFTOKEN_OFFER, hook_accid, 20, account_seq, 5, 0, 0) != 34)
    //     rollback(SBUF("MintedTokens: Internal error, could not generate keylet"), 10);

    uint8_t hash_out[32];
    if (util_sha512h(hash_out, 32, offer_index, GENERATE_NFTOKEN_OFFER_INDEX_SIZE) < 0)
        rollback("NFTokenOfferIndex: Could not generate Hash", 23, 1);

    TRACEHEX(hash_out);

    accept (0,0,0); 

    _g(1,1);   // every hook needs to import guard function and use it at least once
    // unreachable
    return 0;
}