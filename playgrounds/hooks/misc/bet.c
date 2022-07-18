/**
 * This hook just accepts any transaction coming through it
 */
 
#include "hookapi.h"
#include <stdint.h>

int64_t cbak(uint32_t reserved) { 
    return 0;
}

slipId = slipCount;
slips[slipId] = Slip({
    destination: destination,
    value: value,
    win: win,
    position: position,
    toSeal: newValue,
    executed: false,
    refunded: false
});
slipCount += 1;
emit BetSubmission(slipId);
if (newValue == 0) {
    emit BetSealed(slipId);
}

newValue = value;
for (uint i = 0; i<slipCount; i++) {
    if (slips[i].destination == msg.sender) {
        continue;
    }
    if (slips[i].position == position) {
        continue;
    }
    if (slips[i].executed) {
        continue;
    }
    if (slips[i].toSeal > newValue) {
        continue;
    }
    Slip storage slip = slips[i];
    newValue = newValue.sub(slip.toSeal);
    slip.toSeal = 0;
    isSealed = true;
    emit BetSealed(i);
}
if (!isSealed) {
    newValue = win;
}

#define COVER_UNSEALED_SIZE 1U
#define COVER_UNSEALED(value, win, position)\
    {\
        uint8_t isSealed = 0;\
        uint8_t new_value = value;\
        uint8_t value[4];\
        uint8_t key[32] = {\
            0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,\
            0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,\
            0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,\
            0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x01U\
        };\
        if (state(value, 4, key, 32) < 0)\
        {\
            rollback(SBUF("Carbon: sfAccount field missing!!!"), 1);\
        }\
        TRACEVAR(value);\
        TRACESTR("Accept.c: COVER.");\
    }

int64_t hook(uint32_t reserved ) {

    TRACESTR("Accept.c: Called.");

    uint8_t odd = 1;


    uint8_t position = 1; 
    uint8_t amount = 10;

    uint8_t win = 0;
    uint8_t newValue = 0;
    if (position == 1) {
        win = amount.mul(odd);
    }
    if (position == 0) {
        win = amount.div(odd);
    }
    newValue = COVER_UNSEALED(amount, win, position);
    // slipId = ADD_SLIP(msg.sender, amount, win, newValue, position);

    accept (0,0,0); 

    _g(1,1);   // every hook needs to import guard function and use it at least once
    // unreachable
    return 0;
}