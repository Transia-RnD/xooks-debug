/**
 * tax-cur-in.c - Adds Tax Hook to Currency on Incoming Transactions
 *
 * Author: Richard Holland
 * Date: 1 Mar 2021
 *
 **/

#include <stdint.h>
#include "../services/hookapi.h"

#define LIQACCT "rww31Sbht5GgbAqyg5rDEnmPiLtBhfLRsA"
#define LIQTAX 0.01f
// #define RESIACCT "rLVidmD5cyF1ciLNWLmZ6BdDNnPVdwjh1r"
// #define RESITAX 0.01f

int64_t cbak(int64_t reserved)
{
    return 0;
}

int64_t hook(int64_t reserved)
{

    TRACESTR("LP: started");

    // before we start calling hook-api functions we should tell the hook how many tx we intend to create
    etxn_reserve(1); // we are going to emit 3 transactions
    uint8_t currency[20] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 'C', 'S', 'C', 0,0,0,0,0};

    // this api fetches the AccountID of the account the hook currently executing is installed on
    // since hooks can be triggered by both incoming and ougoing transactions this is important to know
    unsigned char hook_accid[20];
    hook_account((uint32_t)hook_accid, 20);

    // get the source tag if any... negative means it wasn't provided
    int64_t source_tag = otxn_field(0,0, sfSourceTag);
    if (source_tag < 0)
        source_tag = 0xFFFFFFFFU;

    // NB:
    //  almost all of the hook apis require a buffer pointer and buffer length to be supplied ... to make this a
    //  little easier to code a macro: `SBUF(your_buffer)` expands to `your_buffer, sizeof(your_buffer)`

    // next fetch the sfAccount field from the originating transaction
    uint8_t account_field[20];
    int32_t account_field_len = otxn_field(SBUF(account_field), sfAccount);
    TRACEVAR(account_field_len);
    if (account_field_len < 20)                                   // negative values indicate errors from every api
        rollback(SBUF("LP: sfAccount field missing!!!"), 1);  // this code could never be hit in prod
                                                                  // but it's here for completeness

    // compare the "From Account" (sfAccount) on the transaction with the account the hook is running on
    int equal = 0; BUFFER_EQUAL(equal, hook_accid, account_field, 20);
    if (!equal)
    {
        // if the accounts are not equal (memcmp != 0) the otxn was sent to the hook account by someone else
        // accept() it and end the hook execution here
        accept(SBUF("LP: Incoming transaction"), 2);
    }

    int64_t liq_drops_to_send = 1000; // this will be the default
    // int64_t resi_drops_to_send = 1000; // this will be the default
    // int64_t corp_drops_to_send = 1000; // this will be the default

    // execution to here means the user has sent a valid transaction FROM the account the hook is installed on

    // process the amount sent, which could be either xrp or pusd
    // to do this we 'slot' the originating txn, that is: we place it into a slot so we can use the slot api
    // to examine its internals
    int64_t oslot = otxn_slot(0);
    if (oslot < 0)
        rollback(SBUF("LP: Could not slot originating txn."), 1);

    // specifically we're interested in the amount sent
    int64_t amt_slot = slot_subfield(oslot, sfAmount, 0);
    if (amt_slot < 0)
        rollback(SBUF("LP: Could not slot otxn.sfAmount"), 2);

    int64_t amt = slot_float(amt_slot);
    if (amt < 0)
        rollback(SBUF("LP: Could not parse amount."), 1);

    // the slot_type api allows determination of fields and subtypes of fields according to the doco
    // in this case we're examining an amount field to see if it's a native (xrp) amount or an iou amount
    // this means passing flag=1
    int64_t is_xrp = slot_type(amt_slot, 1);
    if (is_xrp)
    {
        // XRP INCOMING
        accept(SBUF("LP: Sent you XRP!"), 0);
    }
    else
    {
        // NON-XRP incoming

        // you can trace the behaviour of your hook using the trace(buf, size, as_hex) api
        // which will output to xrpld's trace log
        liq_drops_to_send = (int64_t)((double)amt * LIQTAX); // otherwise we send 1%
        // resi_drops_to_send = (int64_t)((double)amt * RESITAX); // otherwise we send 1%

        TRACEVAR(liq_drops_to_send);
        // TRACEVAR(resi_drops_to_send);

        // hooks communicate accounts via the 20 byte account ID, this can be generated from an raddr like so
        // a more efficient way to do this is precompute the account-id from the raddr (if the raddr never changes)
        uint8_t tax_laccid[20];
        int64_t let = util_accid(
                SBUF(tax_laccid),                                   /* <-- generate into this buffer  */
                SBUF(LIQACCT));         /* <-- from this r-addr           */
        TRACEVAR(let);

        // // hooks communicate accounts via the 20 byte account ID, this can be generated from an raddr like so
        // // a more efficient way to do this is precompute the account-id from the raddr (if the raddr never changes)
        // uint8_t tax_raccid[20];
        // int64_t ret = util_accid(
        //         SBUF(tax_raccid),                                   /* <-- generate into this buffer  */
        //         SBUF(RESIACCT));         /* <-- from this r-addr           */
        // TRACEVAR(ret);

        //setup the outgoing txn
        int64_t fee = etxn_fee_base(PREPARE_PAYMENT_SIMPLE_TRUSTLINE_SIZE);

        // we need to dump the iou amount into a buffer
        // by supplying -1 as the fieldcode we tell float_sto not to prefix an actual STO header on the field
        uint8_t amt_out[48];
        if (float_sto(SBUF(amt_out), SBUF(currency), SBUF(hook_accid), liq_drops_to_send, -1) < 0)
            rollback(SBUF("ITAX: Could not dump currency amount into sto"), 1);

        // set the currency code and issuer in the amount field
        for (int i = 0; GUARD(20),i < 20; ++i)
        {
            amt_out[i + 28] = hook_accid[i];
            amt_out[i +  8] = currency[i];
        }

        // finally create the outgoing txn
        uint8_t ltxn_out[PREPARE_PAYMENT_SIMPLE_TRUSTLINE_SIZE];
        PREPARE_PAYMENT_SIMPLE_TRUSTLINE(ltxn_out, amt_out, fee, tax_laccid, source_tag, source_tag);

        uint8_t emithash[32];
        if (emit(SBUF(emithash), SBUF(ltxn_out)) < 0)
            rollback(SBUF("ITAX: Emitting ltxn failed"), 1);

        accept(SBUF("ITAX: Sent you CURRENCY!"), 0);
    }
    return 0;
}
