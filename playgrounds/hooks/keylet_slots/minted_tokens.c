/**
 * This hook just accepts any transaction coming through it
 */
 
#include "hookapi.h"

int64_t cbak(uint32_t reserved) { 
    TRACESTR("MintedTokens.c: Callback Called.");
    return 0;
}

int64_t hook(uint32_t reserved ) {

    TRACESTR("MintedTokens.c: Called.");

    // get the account the hook is running on and the account that created the txn
    uint8_t hook_accid[20];
    hook_account(SBUF(hook_accid));

    uint8_t keylet[34];
    if (util_keylet(SBUF(keylet), KEYLET_ACCOUNT, hook_accid, 20, 0, 0, 0, 0) != 34)
        rollback(SBUF("MintedTokens: Internal error, could not generate keylet"), 10);

    // then requesting XRPLD slot that keylet into a new slot for us
    int64_t slot_no = slot_set(SBUF(keylet), 0);
    if (slot_no < 0)
    rollback(SBUF("MintedTokens: Could not set keylet in slot"), 10);

    int64_t seq_slot = slot_subfield(slot_no, sfMintedNFTokens, 0);
    if (seq_slot < 0)
        rollback(SBUF("MintedTokens: Could not find sfMintedNFTokens on hook account"), 20);

    uint32_t num_tokens = 0;
    uint8_t seq_buf[4];
    seq_slot = slot(SBUF(seq_buf), seq_slot);
    if (seq_slot != 4)
        rollback(SBUF("MintedTokens: Could not fetch sequence from sfMintedNFTokens."), 80);
    
    // then conver the four byte buffer to an unsigned 32 bit integer
    num_tokens = UINT32_FROM_BUF(seq_buf);
    TRACEVAR(num_tokens); // print the integer for debugging purposes
    TRACEVAR(seq_slot);

    accept (0,0,0); 

    _g(1,1);   // every hook needs to import guard function and use it at least once
    // unreachable
    return 0;
}