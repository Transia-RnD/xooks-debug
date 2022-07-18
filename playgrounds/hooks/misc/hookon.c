/**
 * This hook shows you how to manage the incoming/outgoing xrp/ic and hook on transactions
 */
 
#include "hookapi.h"
#include <stdint.h>

int64_t cbak(uint32_t reserved) {
    TRACESTR("Hookon.c: Called.");
    return 0;
}

int64_t hook(uint32_t reserved ) {

    // 0. COMMON - GET ACCOUNT
    TRACESTR("Hookon.c: Called.");

    unsigned char hook_accid[20];
    hook_account((uint32_t)hook_accid, 20);

    uint8_t account_field[20];
    int32_t account_field_len = otxn_field(SBUF(account_field), sfAccount);
    if (account_field_len < 20) 
        rollback(SBUF("Hookon: sfAccount field missing!!!"), 1);

    // 1. INCOMING/OUTGOING
    int equal = 0; BUFFER_EQUAL(equal, hook_accid, account_field, 20);
    if (equal)
    {
        accept(SBUF("Hookon: Outgoing transaction"), 2);
    }
    
    TRACESTR("Hookon.c: Incoming transaction.");
    
    // ----------------------------------------------------------------------->
    // 2. XRP/IC
    unsigned char amount_buffer[48];
    int64_t amount_len = otxn_field(SBUF(amount_buffer), sfAmount);
    int64_t sell_drops_raw = 10;
    if (amount_len != 8)
    {
        accept(SBUF("Hookon: Non-XRP transaction detected."), 2);
    }
    TRACESTR("Hookon: XRP transaction detected.");

    // // ----------------------------------------------------------------------->
    // // 3. Transaction Type - DA: better to chain hooks..
    // // 2. XRP/IC
    // unsigned char amount_buffer[48];
    // int64_t amount_len = otxn_field(SBUF(amount_buffer), sfAmount);
    // int64_t sell_drops_raw = 10;
    // if (amount_len != 8)
    // {
    //     accept(SBUF("Hookon: Invalid Transaction Type detected."), 2);
    // }
    // TRACESTR("Hookon: Valid Transaction Type detected.");

    accept (0,0,0); 

    _g(1,1);   // every hook needs to import guard function and use it at least once
    // unreachable
    return 0;
}