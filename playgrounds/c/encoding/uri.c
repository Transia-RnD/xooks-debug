#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// VL COMMON
#define ENCODE_VL_UNCOMMON(buf_out, vl, vl_len, field, field2)\
    {\
        buf_out[0] = 0x75U;\
        buf_out[1] = field;\
        buf_out[2] = field2;\
        *(uint64_t*)(buf_out +  2) = *(uint64_t*)(vl +  0);\
        *(uint64_t*)(buf_out + 10) = *(uint64_t*)(vl +  8);\
        *(uint64_t*)(buf_out + 18) = *(uint64_t*)(vl + 16);\
        *(uint64_t*)(buf_out + 26) = *(uint64_t*)(vl + 24);\
        *(uint32_t*)(buf_out + 34) = *(uint32_t*)(vl + 32);\
        buf_out += vl_len;\
    }
#define _07_XX_ENCODE_VL_UNCOMMON(buf_out, vl, vl_len, field, field2)\
    ENCODE_VL_UNCOMMON(buf_out, vl, vl_len, field, field2)\

#define ENCODE_VL_COMMON(buf_out, vl, vl_len)\
    {\
        buf_out[0] = 0x75U;\
        buf_out[1] = vl_len;\
        *(uint64_t*)(buf_out +  2) = *(uint64_t*)(vl +  0);\
        *(uint64_t*)(buf_out + 10) = *(uint64_t*)(vl +  8);\
        *(uint64_t*)(buf_out + 18) = *(uint64_t*)(vl + 16);\
        *(uint64_t*)(buf_out + 26) = *(uint64_t*)(vl + 24);\
        *(uint64_t*)(buf_out + 34) = *(uint64_t*)(vl + 32);\
        *(uint64_t*)(buf_out + 42) = *(uint64_t*)(vl + 40);\
        *(uint64_t*)(buf_out + 50) = *(uint64_t*)(vl + 48);\
        buf_out += vl_len;\
    }
#define _07_XX_ENCODE_VL_COMMON(buf_out, vl, vl_len)\
    ENCODE_VL_COMMON(buf_out, vl, vl_len)\

// URI
// #define ENCODE_URI_SIZE ?
#define ENCODE_URI(buf_out, hex) \
    int vl_len = sizeof(hex);\
    if (vl_len <= 192) {\
        printf("LESS THAN 192\n");\
        ENCODE_VL_COMMON(buf_out, hex, vl_len);\
    }\
    else if (vl_len <= 12480) {\
        vl_len -= 193;\
        printf("GREATER THAN 193\n");\
        int byte1 = (vl_len >> 8) + 193;\
        printf("BYTE 1\n");\
        printf("%d\n", byte1);\
        int byte2 = vl_len & 0xFFU;\
        printf("BYTE 2\n");\
        printf("%d\n", byte2);\
        ENCODE_VL_UNCOMMON(buf_out, hex, byte1, byte2, vl_len);\
    }
#define _07_05_ENCODE_URI(buf_out, hex)\
    ENCODE_URI(buf_out, hex);

// #define PREPARE_URI_TEST_SIZE ?
#define PREPARE_URI_TEST(buf_out_master, uri)  \
    {                                          \
        u_int8_t *buf_out = buf_out_master;    \
        _07_05_ENCODE_URI(buf_out, uri); \
    }

// TEST ANY TX
int test(void)
{

    // 697066733A2F2F516D614374444B5A4656767666756676626479346573745A626851483744586831364354707631686F776D424779

    unsigned char tx_out[55];

    uint8_t uri_value[53] = { 0X69U, 0X70U, 0X66U, 0X73U, 0X3AU, 0X2FU, 0X2FU, 0X51U, 0X6DU, 0X61U, 0X43U, 0X74U, 0X44U, 0X4BU, 0X5AU, 0X46U, 0X56U, 0X76U, 0X76U, 0X66U, 0X75U, 0X66U, 0X76U, 0X62U, 0X64U, 0X79U, 0X34U, 0X65U, 0X73U, 0X74U, 0X5AU, 0X62U, 0X68U, 0X51U, 0X48U, 0X37U, 0X44U, 0X58U, 0X68U, 0X31U, 0X36U, 0X43U, 0X54U, 0X70U, 0X76U, 0X31U, 0X68U, 0X6FU, 0X77U, 0X6DU, 0X42U, 0X47U, 0X79 };
    
    PREPARE_URI_TEST(tx_out, uri_value);

    // printf("%x", tx_out[0]);
    // printf("%x", tx_out[1]);

    for (int i = 0; i < sizeof(tx_out); i++)
    {
        char hex_string[20];
        sprintf(hex_string, "%02X", tx_out[i]);
        printf("%s", hex_string);
    }
    
    printf("\nDONE\n");
    return 0;
}

int main(void)
{
  test();
  return 0;
}
