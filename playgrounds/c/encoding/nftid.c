#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define PREPARE_URI_TEST_SIZE ?
#define GET_NEXT_NFT_ID_SIZE 33U
#define GET_NEXT_NFT_ID(buf_out, flags, fee, hook_accid, taxon, sequence)\
    {\
        (buf_out)[1] = 0x00;\
        (buf_out)[2] = flags;\
        (buf_out)[3] = (fee)[0];\
        (buf_out)[4] = (fee)[1];\
        *(uint64_t*)(buf_out +  2) = *(uint64_t*)(hook_accid +  0);\
        *(uint64_t*)(buf_out + 10) = *(uint64_t*)(hook_accid +  8);\
        *(uint32_t*)(buf_out + 18) = *(uint32_t*)(hook_accid + 16);\
        (buf_out)[25] = ((taxon) >> 24) & 0xFFU;\
        (buf_out)[26] = ((taxon) >> 16) & 0xFFU;\
        (buf_out)[27] = ((taxon) >> 8) & 0xFFU;\
        (buf_out)[28] = ((taxon) >> 0) & 0xFFU;\
        (buf_out)[29] = ((sequence) >> 24) & 0xFFU;\
        (buf_out)[30] = ((sequence) >> 16) & 0xFFU;\
        (buf_out)[31] = ((sequence) >> 8) & 0xFFU;\
        (buf_out)[32] = ((sequence) >> 0) & 0xFFU;\
        (buf_out) += GET_NEXT_NFT_ID_SIZE;\
    }

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

    uint32_t nft_taxon = 0;
    uint8_t taxon[4] = { 0x00U, 0x00U, 0x00U, 0x01U };
    nft_taxon = UINT32_FROM_BUF(taxon);
    
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
