/**
 * lib.c - An testing env for hooks in C
 *
 * Author: Denis Angell
 * Date: 14 June 2022
 *
 **/

#define HAS_CALLBACK
#include <stdint.h>
#include "hookapi.h"
#include "transactions/nftoken_mint.h"
#include "transactions/nftoken_offer_create.h"

int64_t cbak(uint32_t reserved)
{
    TRACESTR("Swap.c: callback called.");
    return 0;
}

int64_t hook(uint32_t reserved ) {

    TRACESTR("Swap.c: Called.");
    accept (0,0,0);

    _g(1,1);   // every hook needs to import guard function and use it at least once
    // unreachable
    return 0;
}