// #include "../hookmacro.h"

#ifdef HAS_CALLBACK
#define PREPARE_PAYMENT_SIMPLE_SIZE 270U
#else
#define PREPARE_PAYMENT_SIMPLE_SIZE 248U
#endif

#define PREPARE_PAYMENT_SIMPLE(buf_out_master, drops_amount_raw, to_address, dest_tag_raw, src_tag_raw)\
    {\
        uint8_t* buf_out = buf_out_master;\
        uint8_t acc[20];\
        uint64_t drops_amount = (drops_amount_raw);\
        uint32_t dest_tag = (dest_tag_raw);\
        uint32_t src_tag = (src_tag_raw);\
        uint32_t cls = (uint32_t)ledger_seq();\
        hook_account(SBUF(acc));\
        _01_02_ENCODE_TT                   (buf_out, ttPAYMENT                      );      /* uint16  | size   3 */ \
        _02_02_ENCODE_FLAGS                (buf_out, tfCANONICAL                    );      /* uint32  | size   5 */ \
        _02_03_ENCODE_TAG_SRC              (buf_out, src_tag                        );      /* uint32  | size   5 */ \
        _02_04_ENCODE_SEQUENCE             (buf_out, 0                              );      /* uint32  | size   5 */ \
        _02_14_ENCODE_TAG_DST              (buf_out, dest_tag                       );      /* uint32  | size   5 */ \
        _02_26_ENCODE_FLS                  (buf_out, cls + 1                        );      /* uint32  | size   6 */ \
        _02_27_ENCODE_LLS                  (buf_out, cls + 5                        );      /* uint32  | size   6 */ \
        _06_01_ENCODE_DROPS_AMOUNT         (buf_out, drops_amount                   );      /* amount  | size   9 */ \
        uint8_t* fee_ptr = buf_out;\
        _06_08_ENCODE_DROPS_FEE            (buf_out, 0                              );      /* amount  | size   9 */ \
        _07_03_ENCODE_SIGNING_PUBKEY_NULL  (buf_out                                 );      /* pk      | size  35 */ \
        _08_01_ENCODE_ACCOUNT_SRC          (buf_out, acc                            );      /* account | size  22 */ \
        _08_03_ENCODE_ACCOUNT_DST          (buf_out, to_address                     );      /* account | size  22 */ \
        int64_t edlen = etxn_details((uint32_t)buf_out, PREPARE_PAYMENT_SIMPLE_SIZE);       /* emitdet | size 1?? */ \
        int64_t fee = etxn_fee_base(buf_out_master, PREPARE_PAYMENT_SIMPLE_SIZE);                                    \ 
        _06_08_ENCODE_DROPS_FEE            (fee_ptr, fee                            );                               \
    }