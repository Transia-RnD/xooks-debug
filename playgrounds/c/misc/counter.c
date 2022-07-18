#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COUNTER_RESET_SIZE 1U
#define COUNTER_RESET()\
    {\
        printf("RESET\n");\
        count = 0;\
    }

#define COUNTER_INCREMENT_SIZE 1U
#define COUNTER_INCREMENT()\
    {\
        printf("INCREMENT\n");\
        count += 1;\
    }

#define COUNTER_DECREMENT_SIZE 1U
#define COUNTER_DECREMENT()\
    {\
        printf("DECREMENT\n");\
        count -= 1;\
    }

// TEST ANY TX
int test(void)
{
    u_int32_t count = 0;
    COUNTER_INCREMENT();
    COUNTER_INCREMENT();
    COUNTER_INCREMENT();
    COUNTER_DECREMENT();
    COUNTER_DECREMENT();
    printf("%u\n", count);

    COUNTER_RESET();
    printf("%u\n", count);

    printf("\nDONE\n");
    return 0;
}

int main(void)
{
  test();
  return 0;
}
