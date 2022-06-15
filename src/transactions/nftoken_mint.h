

#define PREPARE_NFT_MINT_SIMPLE_SIZE 240
#define PREPARE_NFT_MINT_SIMPLE(buf_out_master, flags, taxon, uri) \
{                                                                                            \
    uint8_t *buf_out = buf_out_master;                                                       \
    uint8_t acc[20];                                                                         \
    uint32_t cls = (uint32_t)ledger_seq();                                                   \
    hook_account(SBUF(acc));                                                                 \
    _01_02_ENCODE_TT(buf_out, ttNFTOKEN_MINT);                                              /* uint16  | size   3 */\
    _02_02_ENCODE_FLAGS(buf_out, flags);                                                    /* uint32  | size   5 */\
    _02_04_ENCODE_SEQUENCE(buf_out, 0);                                                     /* uint32  | size   5 */\
    _02_26_ENCODE_FLS(buf_out, cls + 1);                                                    /* uint32  | size   6 */\
    _02_27_ENCODE_LLS(buf_out, cls + 5);                                                    /* uint32  | size   6 */\
    _02_42_ENCODE_TAXON_NFT(buf_out, taxon);                                                /* amount  | size   6 */\
    _07_XX_ENCODE_VL_COMMON(buf_out, uri);                                                  /* vl.     | size   5 */\
    _06_08_ENCODE_DROPS_FEE(buf_out, 120);                                                    /* amount  | size   9 */\
    uint8_t* fee_ptr = buf_out;\
    _07_03_ENCODE_SIGNING_PUBKEY_NULL(buf_out);                                             /* pk      | size  35 */\
    _08_01_ENCODE_ACCOUNT_SRC(buf_out, acc);                                                /* account | size  22 */\
    int64_t edlen = etxn_details((uint32_t)buf_out, PREPARE_NFT_MINT_SIMPLE_SIZE);          /* emitdet | size 1?? */\
}