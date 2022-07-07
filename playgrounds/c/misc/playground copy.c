#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define tfBURNABLE 0x00000001UL
#define tfONLYXRP 0x00000002UL
#define tfTRUSTLINE 0x00000004UL
#define tfTRANSFERABLE 0x00000008UL

#define ENCODE_UINT32_UNCOMMON_SIZE 6U
#define ENCODE_UINT32_UNCOMMON(buf_out, i, field) \
    {                                             \
        u_int32_t ui = i;                         \
        u_int8_t uf = field;                      \
        buf_out[0] = 0x20U;                       \
        buf_out[1] = uf;                          \
        buf_out[2] = (ui >> 24) & 0xFFU;          \
        buf_out[3] = (ui >> 16) & 0xFFU;          \
        buf_out[4] = (ui >> 8) & 0xFFU;           \
        buf_out[5] = (ui >> 0) & 0xFFU;           \
        buf_out += ENCODE_UINT32_UNCOMMON_SIZE;   \
    }

#define ENCODE_UINT32_COMMON_SIZE 5U
#define ENCODE_UINT32_COMMON(buf_out, i, field) \
    {                                           \
        u_int32_t ui = i;                       \
        u_int8_t uf = field;                    \
        buf_out[0] = 0x20U + (uf & 0x0FU);      \
        buf_out[1] = (ui >> 24) & 0xFFU;        \
        buf_out[2] = (ui >> 16) & 0xFFU;        \
        buf_out[3] = (ui >> 8) & 0xFFU;         \
        buf_out[4] = (ui >> 0) & 0xFFU;         \
        buf_out += ENCODE_UINT32_COMMON_SIZE;   \
    }
#define _02_XX_ENCODE_UINT32_COMMON(buf_out, i, field) \
    ENCODE_UINT32_COMMON(buf_out, i, field)

// NFT TOKEN TAXON
#define ENCODE_TAXON_NFT_SIZE 6
#define ENCODE_TAXON_NFT(buf_out, taxon) \
    ENCODE_UINT32_UNCOMMON(buf_out, taxon, 0x2A);
#define _02_42_ENCODE_TAXON_NFT(buf_out, taxon) \
    ENCODE_TAXON_NFT(buf_out, taxon);

// FLAGS
#define ENCODE_FLAGS_SIZE 5
#define ENCODE_FLAGS(buf_out, tag) \
    ENCODE_UINT32_COMMON(buf_out, tag, 0x2U);
#define _02_02_ENCODE_FLAGS(buf_out, tag) \
    ENCODE_FLAGS(buf_out, tag);

// VL COMMON
// #define ENCODE_VL_COMMON(buf_out, hex, size)          \
//     {                                                 \
//         int vl_length = size;                         \
//         printf("%d\n", vl_length);\
//         printf("%d\n", *hex);\
//         if (vl_length <= 192) {\
//             buf_out += vl_length;                     \
//             buf_out += *hex;                           \
//         }\
//     }
// #define _07_XX_ENCODE_VL_COMMON(buf_out, hex)         \
//     int size = 53;                                    \
//     printf("%d\n", size);                             \
//     ENCODE_VL_COMMON(buf_out, hex, size)              \

// VL COMMON
#define ENCODE_VL_UNCOMMON(buf_out, hex, field, field2, vl_len)\
    {\
        buf_out[0] = 0x75U;\
        buf_out[1] = field;\
        buf_out[2] = field2;\
        *(uint64_t*)(buf_out +  2) = *(uint64_t*)(hex +  0);\
        *(uint64_t*)(buf_out + 10) = *(uint64_t*)(hex +  8);\
        *(uint64_t*)(buf_out + 18) = *(uint64_t*)(hex + 16);\
        *(uint64_t*)(buf_out + 26) = *(uint64_t*)(hex + 24);\
        buf_out += vl_len;\
    }
#define _07_XX_ENCODE_VL_UNCOMMON(buf_out, hex, field, field2)\
    int size = sizeof(hex);\
    ENCODE_VL_UNCOMMON(buf_out, hex, field, field2, vl_len)\

#define ENCODE_VL_COMMON(buf_out, hex, vl_len)\
    {\
        buf_out[0] = 0x75U;\
        buf_out[1] = vl_len;\
        *(uint64_t*)(buf_out +  2) = *(uint64_t*)(hex +  0);\
        *(uint64_t*)(buf_out + 10) = *(uint64_t*)(hex +  8);\
        *(uint64_t*)(buf_out + 18) = *(uint64_t*)(hex + 16);\
        *(uint64_t*)(buf_out + 26) = *(uint64_t*)(hex + 24);\
        buf_out += vl_len;\
    }
#define _07_XX_ENCODE_VL_COMMON(buf_out, hex)\
    int size = sizeof(hex);\
    ENCODE_VL_COMMON(buf_out, hex, size)\

// #define ENCODE_VL_COMMON_SIZE 5U
// #define ENCODE_VL_COMMON(buf_out, i, field)          \
//     {    \
//         u_int32_t ui = *i;                       \
//         u_int8_t uf = field;    \
//         buf_out[0] = 0x75U;                \
//         buf_out[0] = 0x75U + (uf & 0x0FU);      \
//         buf_out[2] = (ui >> 16) & 0xFFU;          \
//         buf_out[3] = (ui >> 8) & 0xFFU;           \
//         buf_out[4] = (ui >> 0) & 0xFFU;           \
//         buf_out += ENCODE_VL_COMMON_SIZE;  \
//     }
// #define _07_XX_ENCODE_VL_COMMON(buf_out, i, field)   \
//     ENCODE_VL_COMMON(buf_out, i, field)              \


// HASH256 COMMON
// #define ENCODE_HASH256_COMMON_SIZE 33U
// #define ENCODE_HASH256_COMMON(buf_out, i, field) \
//     {                                             \
//         u_int32_t ui = *i;                       \
//         u_int8_t uf = field;                    \
//         buf_out[0] = 0x50U + (uf & 0x0FU);      \
//         buf_out[2] = (ui >> 24) & 0xFFU;          \
//         buf_out[3] = (ui >> 16) & 0xFFU;          \
//         buf_out[4] = (ui >> 8) & 0xFFU;           \
//         buf_out[5] = (ui >> 0) & 0xFFU;           \
//         buf_out += ENCODE_HASH256_COMMON_SIZE;  \
//     }
// #define _05_XX_ENCODE_HASH256_COMMON(buf_out, i, field)\
//     ENCODE_HASH256_COMMON(buf_out, i, field);


// HASH256 COMMON
#define ENCODE_HASH256_COMMON_SIZE 33U
#define ENCODE_HASH256_COMMON(buf_out, flags, fee, field)\
    {\
        uint32_t ui = (i);\
        uint8_t uf = field;\
        (buf_out)[0] = 0x50U + (uf & 0x0FU);\
        (buf_out)[2] = flags & 0xFFU;\
        (buf_out)[3] = fee & 0xFFU;\
        (buf_out) += ENCODE_HASH256_COMMON_SIZE;\
    }
#define _05_XX_ENCODE_HASH256_COMMON(buf_out, flags, fee, field)\
    ENCODE_HASH256_COMMON(buf_out, flags, fee, field);

// NFTOKEN ID
#define ENCODE_TOKEN_ID_SIZE 33
#define ENCODE_TOKEN_ID(buf_out, nftoken_id) \
    ENCODE_HASH256_COMMON(buf_out, flags, fee, 0xAU);
#define _05_10_ENCODE_TOKEN_ID(buf_out, flags, fee)\
    ENCODE_TOKEN_ID(buf_out, flags, fee);

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
    }\
    // ENCODE_VL_COMMON(buf_out, hex, 0x3U);
#define _07_05_ENCODE_URI(buf_out, hex)\
    ENCODE_URI(buf_out, hex);

// ---------------------------------------------------

// NFTOKEN_ID
// #define ENCODE_NFTOKEN_ID_SIZE 22
// #define ENCODE_NFTOKEN_ID(buf_out, account_id)\
//     {\
//         uint8_t uat = account_type;\
//         buf_out[0] = 0x80U + uat;\
//         buf_out[1] = 0x14U;\
//         *(uint64_t*)(buf_out +  2) = *(uint64_t*)(account_id +  0);\
//         *(uint64_t*)(buf_out + 10) = *(uint64_t*)(account_id +  8);\
//         *(uint32_t*)(buf_out + 18) = *(uint32_t*)(account_id + 16);\
//         buf_out += ENCODE_ACCOUNT_SIZE;\
//     }
// #define _08_XX_ENCODE_ACCOUNT(buf_out, account_id, account_type)\
//     ENCODE_ACCOUNT(buf_out, account_id, account_type);


#define PREPARE_ANY_TX_TEST_SIZE 33
#define PREPARE_ANY_TX_TEST(buf_out_master, flags, fee) \
    {                                                   \
        u_int8_t *buf_out = buf_out_master;             \
        _05_10_ENCODE_TOKEN_ID(buf_out, flags, fee);    \
    }

// ---------------------------------------------------

#define PREPARE_URI_TEST_SIZE 9U
#define PREPARE_URI_TEST(buf_out_master, uri)  \
    {                                          \
        u_int8_t *buf_out = buf_out_master;    \
        _07_05_ENCODE_URI(buf_out, uri); \
    }

// #define PREPARE_TAXON_TEST_SIZE 6
// #define PREPARE_TAXON_TEST(buf_out_master, taxon) \
//     {                                             \
//         u_int8_t *buf_out = buf_out_master;       \
//         _02_42_ENCODE_TAXON_NFT(buf_out, taxon);  \
//     }

// #define PREPARE_FLAG_TEST_SIZE 5
// #define PREPARE_FLAG_TEST(buf_out_master, taxon) \
//     {                                            \
//         u_int8_t *buf_out = buf_out_master;      \
//         _02_02_ENCODE_FLAGS(buf_out, taxon);     \
//     }

// #define PREPARE_FLAG_TEST_SIZE 5
// #define PREPARE_FLAG_TEST(buf_out_master, taxon) \
//     {                                            \
//         u_int8_t *buf_out = buf_out_master;      \
//         _02_02_ENCODE_FLAGS(buf_out, taxon);     \
//     }

// TEST FLAGS
// int testFlags(void)
// {
//     int flags = tfBURNABLE + tfONLYXRP;
//     u_int8_t ltxn_out[PREPARE_FLAG_TEST_SIZE];
//     // PREPARE_TAXON_TEST(ltxn_out, 0);
//     PREPARE_FLAG_TEST(ltxn_out, flags);
//     // printf("%s\n", ltxn_out);
//     for (int i = 0; i < sizeof(ltxn_out); i++)
//     {
//         // printf("%02x\n", ltxn_out[i]);
//         char hex_string[20];
//         sprintf(hex_string, "%02X", ltxn_out[i]);
//         printf("%s", hex_string);
//     }
//     return 0;
// }

// // TEST TOKEN TAXON
// int testNFTokenTaxon(void)
// {
//     u_int8_t ltxn_out[PREPARE_TAXON_TEST_SIZE];
//     PREPARE_TAXON_TEST(ltxn_out, 0);
//     for (int i = 0; i < sizeof(ltxn_out); i++)
//     {
//         char hex_string[20];
//         sprintf(hex_string, "%02X", ltxn_out[i]);
//         printf("%s", hex_string);
//     }
//     return 0;
// }

// TEST URI
int testURI(void)
{
    int flags = tfBURNABLE + tfTRANSFERABLE;
    uint8_t ltxn_out[PREPARE_URI_TEST_SIZE];
    // uint8_t hex[193] = {105, 112, 102, 115, 58, 47, 47, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 48, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 57, 56, 55, 54, 53, 52, 51, 50, 49, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 48, 57, 56, 55, 54, 53, 52, 51, 50, 49};
    uint8_t hex[7] = {105, 112, 102, 115, 58, 47, 47};
    printf("%lu\n", sizeof(hex));
    PREPARE_URI_TEST(ltxn_out, hex);
    for (int i = 0; i < sizeof(ltxn_out); i++)
    {
        char hex_string[20];
        sprintf(hex_string, "%02X", ltxn_out[i]);
        printf("%s", hex_string);
    }
    printf("\nDONE\n");
    return 0;
}

// void revcpy(char* to, char*from)
// {
//    int length = strlen(from);
//    char* ptr = from + length;
//    while(length-- >= 0)
//       *to++ = *ptr--;
//    *to = 0; // null-terminate the string

// }

void myMemCpy(void *dest, void *src, size_t index, size_t n)
{
   // Typecast src and dest addresses to (char *)
   uint8_t *csrc = (uint8_t *)src;
   uint8_t *cdest = (uint8_t *)dest;
  
   // Copy contents of src[] to dest[]
   for (int i = index; i - index < n; i++) {
        printf("%d\n", i);
        printf("%d\n", csrc[i]);
        cdest[i] = csrc[i];
   }
}

// TEST ANY TX
int testAnyTx(void)
{
    // 00090000E3E2649FB84DFB055036605B49DE305618BA72972DCBAB9D00000002
    printf("\nTESTING TOKEN ID\n");
    u_int8_t ltxn_out[PREPARE_ANY_TX_TEST_SIZE];
    uint8_t tokenid[32] = {0, 9, 0, 0, 227, 226, 100, 159, 184, 77, 251, 5, 80, 54, 96, 91, 73, 222, 48, 86, 24, 186, 114, 151, 45, 203, 171, 157, 0, 0, 0, 2};
    int t;
    // memcpy(&t, tokenid + 4, sizeof(t));
    // printf("%d", t);
    // PREPARE_ANY_TX_TEST(ltxn_out, tokenid);
    // printf("%s\n", ltxn_out);
    for (int i = 0; i < sizeof(ltxn_out); i++)
    {
        char hex_string[20];
        sprintf(hex_string, "%02X", ltxn_out[i]);
        printf("%s", hex_string);
    }

    // uint8_t ltxn_out[32];
    // u_int8_t *buf_out = ltxn_out;
    uint8_t flags[4];
    uint8_t fee[4];
    // sprintf(flags, "%04d", tokenid);
    myMemCpy(&flags, tokenid, 3, sizeof(t));
    // myMemCpy(&fee, tokenid, 4, sizeof(t));
    // memcpy(&flags, tokenid, sizeof(t));
    // putc(flags);
    printf("HERE\n");
    for (int i = 4 - 1; sizeof(flags) > i; i--)
    {
        printf("%d", flags[i]);
    }
    // buf_out[0] = tokenid[0];
    // buf_out[0] = tokenid[2];
    // buf_out[0] = tokenid[3];
    // buf_out[0] = tokenid[4];
    // buf_out[1] = 0x14U;
    // buf_out += ENCODE_ACCOUNT_SIZE;
    printf("\nDONE\n");
    return 0;
}

#define tfFlag 0x00000011UL


// TEST ANY TX
int testAnyTx1(void)
{
    // uint8_t tokenid[32] = { 0x9, 0x26, 0x1f, 0xdf, 0x45, 0x31, 0xe, 0x57, 0xef, 0xfd, 0xc5, 0x75, 0xa0, 0xc7, 0x19, 0xb2, 0xd8, 0xfb, 0x38, 0x87 }
    // 0009 - Flags
    // 0000 - Fee
    // E3E2649FB84DFB055036605B49DE305618BA7297 - Account
    // 2DCBAB9D - Taxon
    // 00000002 - Sequence

    // unsigned char nftid[256] = "00090000E3E2649FB84DFB055036605B49DE305618BA72972DCBAB9D00000002";

    uint8_t hook_accid[20] = {
    0x05U, 0xb5U, 0xf4U, 0x3aU, 0xf7U, 
    0x17U, 0xb8U, 0x19U, 0x48U, 0x49U, 0x1fU, 0xb7U, 0x07U, 0x9eU, 0x4fU, 0x17U, 0x3fU, 0x4eU, 0xceU, 0xb3U};

    uint8_t fee = 0x00;
    uint8_t taxon = 0x00;
    uint8_t sequence = 0x00 + 1;
    uint8_t flags = tfBURNABLE + tfTRANSFERABLE;

    // TRACEVAR(flags);
    // TRACEVAR(fee);
    // TRACEVAR(hook_accid);
    // TRACEVAR(taxon);
    // TRACEVAR(sequence);
    put(flags);
    put(fee);
    put(hook_accid);
    put(taxon);
    put(sequence);

    uint8_t data_in_ptr = flags + fee + hook_accid + taxon + sequence;

    // uint8_t data_in_ptr[32] = { 0x9, 0x26, 0x1f, 0xdf, 0x45, 0x31, 0xe, 0x57, 0xef, 0xfd, 0xc5, 0x75, 0xa0, 0xc7, 0x19, 0xb2, 0xd8, 0xfb, 0x38, 0x87 };
    
    uint8_t hash_out[32];
    if (util_sha512h(hash_out, 32, data_in_ptr, 32) < 0)
        rollback("Could not generate Hash", 23, 1);

    // TRACESTR(hash_out)
    // TRACEVAR(hash_out)
    // TRACEHEX(hash_out)

    unsigned char id_out[ENCODE_HASH256_COMMON_SIZE];
    uint8_t *buf_out = id_out;
    PREPARE_ANY_TX_TEST(buf_out, flags, fee);
    // TRACEHEX(id_out);
    // TRACEHEX(buf_out);
    printf("%s", id_out);


    printf("\nDONE\n");
    return 0;
}

int main(void)
{
//   testURI();
  testAnyTx1();
  return 0;
}
