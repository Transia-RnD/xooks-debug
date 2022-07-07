/**
 * This hook just accepts any transaction coming through it
 */
 
#include "hookapi.h"

#define tfBURNABLE 0x00000001UL
#define tfONLYXRP 0x00000002UL
#define tfTRUSTLINE 0x00000004UL
#define tfTRANSFERABLE 0x00000008UL

int64_t cbak(uint32_t reserved)
{
    TRACESTR("Accept: callback called.");
    return 0;
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
        (buf_out)[29] = (hook_accid)[15];\
        (buf_out)[30] = (hook_accid)[16];\
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

int64_t hook(uint32_t reserved ) {

    TRACESTR("Accept.c: Called.");

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

    int64_t seq_slot = slot_subfield(slot_no, sfMintedNFTokens, 0);
    if (seq_slot < 0)
        rollback(SBUF("Accept: Could not find sfMintedNFTokens on hook account"), 20);

    uint32_t num_tokens = 0;
    uint8_t seq_buf[4];
    seq_slot = slot(SBUF(seq_buf), seq_slot);
    if (seq_slot != 4)
        rollback(SBUF("MintedTokens: Could not fetch sequence from sfMintedNFTokens."), 80);

    // then conver the four byte buffer to an unsigned 32 bit integer
    num_tokens = UINT32_FROM_BUF(seq_buf);
    TRACEVAR(num_tokens); // print the integer for debugging purposes
    TRACEVAR(seq_slot);

    uint8_t fee[2] = { 0x00, 0x00 };
    uint32_t nft_taxon = 0;
    uint8_t taxon[4] = { 0x00U, 0x00U, 0x00U, 0x01U };
    nft_taxon = UINT32_FROM_BUF(taxon);

    uint8_t flags = tfBURNABLE + tfTRANSFERABLE;

    TRACEVAR(flags);
    TRACEVAR(fee);
    TRACEVAR(hook_accid);
    TRACEVAR(taxon);
    TRACEVAR(num_tokens);

    unsigned char id_out[GET_NEXT_NFT_ID_SIZE];
    uint8_t *buf_out = id_out;
    GET_NEXT_NFT_ID(buf_out, flags, fee, hook_accid, nft_taxon, num_tokens + 1);
    TRACEHEX(id_out);
    
    accept (0,0,0); 

    _g(1,1);   // every hook needs to import guard function and use it at least once
    // unreachable
    return 0;
}