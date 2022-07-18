/**
 * This hook just accepts any transaction coming through it
 */
 
#include "hookapi.h"

#define COUNTER_RESET_SIZE 1U
#define COUNTER_RESET()\
    {\
        printf("RESET\n");\
        count = 0;\
    }

#define COUNTER_INCREMENT_SIZE 1U
#define COUNTER_INCREMENT(key)\
    {\
        printf("INCREMENT\n");\
        if (state_set(value + 1, 4, key, 32) == 4)\
        {\
            // ... state successfully saved
        }\
        count += 1;\
    }

#define COUNTER_DECREMENT_SIZE 1U
#define COUNTER_DECREMENT()\
    {\
        printf("DECREMENT\n");\
        count -= 1;\
    }

int64_t cbak(uint32_t reserved) { 
    TRACESTR("Counter.c: Called.");
    return 0;
}

int64_t hook(uint32_t reserved ) {

    TRACESTR("Counter.c: Called.");

    uint8_t vault[16];
    int64_t vault_pusd = 0;
    int64_t vault_xrp = 0;
    uint8_t vault_exists = 0;
    if (state(SBUF(vault), SBUF(vault_key)) == 16)
    {
        vault_pusd = float_sto_set(vault, 8);
        vault_xrp  = float_sto_set(vault + 8, 8);
        vault_exists = 1;
    }


    uint8_t key[32] = {
        0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
        0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
        0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
        0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x01U
    };
    uint8_t value[4] = { 0xC0U, 0x01U, 0xCAU, 0xFEU };
    if (state_set(value, 4, key, 32) == 4)
    {
        // ... state successfully saved
        TRACESTR("Counter.c: Called.");
    }

    accept (0,0,0); 

    _g(1,1);   // every hook needs to import guard function and use it at least once
    // unreachable
    return 0;
}