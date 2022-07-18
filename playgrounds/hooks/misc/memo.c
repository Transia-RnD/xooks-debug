/**
 * This hook validates a memo format and logs the memo data
 */

#include "hookapi.h"
#include <stdint.h>

#define MAX_MEMO_SIZE 4096

#define COPY_BUFM(lhsbuf, lhsbuf_spos, rhsbuf, rhsbuf_spos, len, n) \
    for (int i = 0; GUARDM(len, n), i < len; ++i)                   \
        lhsbuf[lhsbuf_spos + i] = rhsbuf[rhsbuf_spos + i];

int64_t cbak(uint32_t reserved) {
    TRACESTR("Swap.c: Called.");
    return 0;
}

int64_t hook(uint32_t reserved ) {

    etxn_reserve(2); // we are going to emit 1 transactions

    // ----------------------------------------------------->
    // GET HOOK ACCOUNT & SENDER ACCOUNT
    TRACESTR("Swap.c: Called.");

    unsigned char hook_accid[20];
    hook_account((uint32_t)hook_accid, 20);

    uint8_t account_field[20];
    int32_t account_field_len = otxn_field(SBUF(account_field), sfAccount);
    if (account_field_len < 20) 
        rollback(SBUF("Swap: sfAccount field missing!!!"), 1);

    int equal = 0; BUFFER_EQUAL(equal, hook_accid, account_field, 20);
    if (equal)
    {
        accept(SBUF("Swap: Outgoing transaction"), 2);
    }

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
        accept(SBUF("Notary: Memo is an invalid format. Passing txn."), 50);
    
    int64_t  data_lookup = sto_subfield(memo_ptr, memo_len, sfMemoData);
    uint8_t* data_ptr = SUB_OFFSET(data_lookup) + memo_ptr;
    uint32_t data_len = SUB_LENGTH(data_lookup);

    if (data_len > MAX_MEMO_SIZE)
        rollback(SBUF("Notary: Memo too large (4kib max)."), 4);

    TRACEVAR(data_len);
    TRACEHEX(data_ptr);

    // uint8_t uri_value[uri_len];
    // uint8_t *buf_out = uri_value;
    // COPY_BUFM(buf_out, uri_ptr);

    // TRACEHEX(buf_out)

    accept (0,0,0);

    // _g(1,1);   // every hook needs to import guard function and use it at least once
    // unreachable
    return 0;
}

// {
//   "NFTokenID": "00090000FD34F798FCE8475FDA03420559F11D54B4C5BFB3727D1EA000000005",
//   "Amount": "7",
//   "Flags": 1,
//   "Fee": "3000",
//   "Memos": [
//     {
//       "Memo": {
//         "MemoData": "697066733A2F2F516D614374444B5A4656767666756676626479346573745A626851483744586831364354707631686F776D424779",
//         "MemoFormat": "756E7369676E65642F7061796C6F61642B31",
//         "MemoType": "687474703A2F2F6578616D706C652E636F6D2F6D656D6F2F67656E65726963"
//       }
//     }
//   ],
//   "TransactionType": "NFTokenCreateOffer",
//   "Account": "rEQ6fH8Vp24v88t3BFy5dBopLGqXUepnuq"
// }