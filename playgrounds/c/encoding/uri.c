#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// #define COPY_BUFM(lhsbuf, lhsbuf_spos, rhsbuf, rhsbuf_spos, len, n) \
//     for (int i = 0; i < len; ++i)                   \
//         lhsbuf[lhsbuf_spos + i] = rhsbuf[rhsbuf_spos + i];

#define COPY_BUFM(lhsbuf, lhsbuf_spos, rhsbuf, rhsbuf_spos, len, n) \
    for (int i = 0; GUARDM(len, n), i < len; ++i)                   \
        lhsbuf[lhsbuf_spos + i] = rhsbuf[rhsbuf_spos + i];

// VL UNUNCOMMON
#define ENCODE_VL_UNUNCOMMON(buf_out, vl, vl_len, field, field2, field3)\
    {\
        buf_out[0] = 0x75U;\
        buf_out[1] = field;\
        buf_out[2] = field2;\
        buf_out[3] = field3;\
        uint8_t *ptr = (uint8_t *)&vl;\
        COPY_BUFM(buf_out, 4, ptr, 0, vl_len, vl_len);\
        buf_out += vl_len;\
    }
#define _07_XX_ENCODE_VL_UNUNCOMMON(buf_out, vl, vl_len, field, field2, field3)\
    ENCODE_VL_UNUNCOMMON(buf_out, vl, vl_len, field, field2, field3)\

// VL UNCOMMON
#define ENCODE_VL_UNCOMMON(buf_out, vl, vl_len, field, field2)\
    {\
        buf_out[0] = 0x75U;\
        buf_out[1] = field;\
        buf_out[2] = field2;\
        uint8_t *ptr = (uint8_t *)&vl;\
        COPY_BUFM(buf_out, 3, ptr, 0, vl_len, vl_len);\
        buf_out += vl_len;\
    }
#define _07_XX_ENCODE_VL_UNCOMMON(buf_out, vl, vl_len, field, field2)\
    ENCODE_VL_UNCOMMON(buf_out, vl, vl_len, field, field2)\

// VL COMMON
#define ENCODE_VL_COMMON(buf_out, vl, vl_len)\
    {\
        buf_out[0] = 0x75U;\
        buf_out[1] = vl_len;\
        uint8_t *ptr = (uint8_t *)&vl;\
        COPY_BUFM(buf_out, 2, ptr, 0, vl_len, vl_len);\
        buf_out += vl_len;\
    }
#define _07_XX_ENCODE_VL_COMMON(buf_out, vl, vl_len)\
    ENCODE_VL_COMMON(buf_out, vl, vl_len)\

// URI
// URI_SIZE is included in the macro.
#define ENCODE_URI(buf_out, vl, vl_len) \
    if (vl_len <= 193) {\
        ENCODE_VL_COMMON(buf_out, vl, vl_len);\
    }\
    else if (vl_len <= 12480) {\
        vl_len -= 193;\
        int byte1 = (vl_len >> 8) + 193;\
        int byte2 = vl_len & 0xFFU;\
        ENCODE_VL_UNCOMMON(buf_out, vl, vl_len, byte1, byte2);\
    }\
    else if (vl_len <= 918744) {\
        vl_len -= 12481;\
        int byte1 = 241 + (vl_len >> 16);\
        int byte2 = (vl_len >> 8) & 0xFFU;\
        int byte3 = vl_len & 0xFFU;\
        ENCODE_VL_UNUNCOMMON(buf_out, vl, vl_len, byte1, byte2, byte3);\
    }

#define _07_05_ENCODE_URI(buf_out, vl, vl_len)\
    ENCODE_URI(buf_out, vl, vl_len);

// #define PREPARE_URI_TEST_SIZE ?
#define PREPARE_URI_TEST(buf_out_master, uri, uri_len)  \
    {                                          \
        u_int8_t *buf_out = buf_out_master;    \
        _07_05_ENCODE_URI(buf_out, uri, uri_len); \
    }

// TEST ANY TX
int test(void)
{

    // // VL COMMON
    // unsigned char tx_out[55];

    // uint8_t uri_value[53] = { 0X69U, 0X70U, 0X66U, 0X73U, 0X3AU, 0X2FU, 0X2FU, 0X51U, 0X6DU, 0X61U, 0X43U, 0X74U, 0X44U, 0X4BU, 0X5AU, 0X46U, 0X56U, 0X76U, 0X76U, 0X66U, 0X75U, 0X66U, 0X76U, 0X62U, 0X64U, 0X79U, 0X34U, 0X65U, 0X73U, 0X74U, 0X5AU, 0X62U, 0X68U, 0X51U, 0X48U, 0X37U, 0X44U, 0X58U, 0X68U, 0X31U, 0X36U, 0X43U, 0X54U, 0X70U, 0X76U, 0X31U, 0X68U, 0X6FU, 0X77U, 0X6DU, 0X42U, 0X47U, 0X79 };
    // int64_t uri_len = 53;

    // PREPARE_URI_TEST(tx_out, uri_value, uri_len);

    // for (int i = 0; i < sizeof(tx_out); i++)
    // {
    //     char hex_string[20];
    //     sprintf(hex_string, "%02X", tx_out[i]);
    //     printf("%s", hex_string);
    // }

    // VL UNCOMMON
    unsigned char tx_out[91];

    uint8_t uri_value[89] = { 0X69U, 0X70U, 0X66U, 0X73U, 0X3AU, 0X2FU, 0X2FU, 0X62U, 0X61U, 0X66U, 0X6BU, 0X72U, 0X65U, 0X69U, 0X66U, 0X32U, 0X77U, 0X68U, 0X69U, 0X37U, 0X67U, 0X33U, 0X77U, 0X65U, 0X78U, 0X72U, 0X72U, 0X71U, 0X79U, 0X6CU, 0X72U, 0X37U, 0X35U, 0X70U, 0X64U, 0X77U, 0X66U, 0X69U, 0X6DU, 0X72U, 0X70U, 0X79U, 0X34U, 0X35U, 0X69U, 0X34U, 0X6DU, 0X68U, 0X6BU, 0X62U, 0X75U, 0X33U, 0X62U, 0X76U, 0X76U, 0X6CU, 0X77U, 0X33U, 0X72U, 0X6DU, 0X36U, 0X36U, 0X67U, 0X65U, 0X37U, 0X65U, 0X3FU, 0X66U, 0X69U, 0X6CU, 0X65U, 0X6EU, 0X61U, 0X6DU, 0X65U, 0X3DU, 0X6EU, 0X69U, 0X63U, 0X2DU, 0X6EU, 0X66U, 0X74U, 0X2DU, 0X31U, 0X2EU, 0X6AU, 0X70U, 0X67U };
    int64_t uri_len = 89;

    PREPARE_URI_TEST(tx_out, uri_value, uri_len);

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
