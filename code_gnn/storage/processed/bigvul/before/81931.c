int wc_ecc_verify_hash(const byte* sig, word32 siglen, const byte* hash,
                       word32 hashlen, int* res, ecc_key* key)
{
    int err;
    mp_int *r = NULL, *s = NULL;
#ifndef WOLFSSL_ASYNC_CRYPT
    mp_int r_lcl, s_lcl;
#endif

    if (sig == NULL || hash == NULL || res == NULL || key == NULL) {
        return ECC_BAD_ARG_E;
    }

#ifdef WOLF_CRYPTO_DEV
    if (key->devId != INVALID_DEVID) {
        err = wc_CryptoDev_EccVerify(sig, siglen, hash, hashlen, res, key);
        if (err != NOT_COMPILED_IN)
            return err;
    }
#endif

#ifdef WOLFSSL_ASYNC_CRYPT
    err = wc_ecc_alloc_async(key);
    if (err != 0)
        return err;
    r = key->r;
    s = key->s;
#else
    r = &r_lcl;
    s = &s_lcl;
#endif

    switch(key->state) {
        case ECC_STATE_NONE:
        case ECC_STATE_VERIFY_DECODE:
            key->state = ECC_STATE_VERIFY_DECODE;

             
            *res = 0;

             
             
            err = DecodeECC_DSA_Sig(sig, siglen, r, s);
            if (err < 0) {
                break;
            }
            FALL_THROUGH;

        case ECC_STATE_VERIFY_DO:
            key->state = ECC_STATE_VERIFY_DO;

            err = wc_ecc_verify_hash_ex(r, s, hash, hashlen, res, key);

        #ifndef WOLFSSL_ASYNC_CRYPT
             
            mp_clear(r);
            mp_clear(s);
        #endif

            if (err < 0) {
                break;
            }
            FALL_THROUGH;

        case ECC_STATE_VERIFY_RES:
            key->state = ECC_STATE_VERIFY_RES;
            err = 0;
            break;

        default:
            err = BAD_STATE_E;
    }

     
    if (err == WC_PENDING_E) {
        key->state++;
        return err;
    }

     
#ifdef WOLFSSL_ASYNC_CRYPT
    wc_ecc_free_async(key);
#endif
    key->state = ECC_STATE_NONE;

    return err;
}
