int wc_ecc_sign_hash(const byte* in, word32 inlen, byte* out, word32 *outlen,
                     WC_RNG* rng, ecc_key* key)
{
    int err;
    mp_int *r = NULL, *s = NULL;
#ifndef WOLFSSL_ASYNC_CRYPT
    mp_int r_lcl, s_lcl;
#endif

    if (in == NULL || out == NULL || outlen == NULL || key == NULL ||
                                                                rng == NULL) {
        return ECC_BAD_ARG_E;
    }

#ifdef WOLF_CRYPTO_DEV
    if (key->devId != INVALID_DEVID) {
        err = wc_CryptoDev_EccSign(in, inlen, out, outlen, rng, key);
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
        case ECC_STATE_SIGN_DO:
            key->state = ECC_STATE_SIGN_DO;

            if ((err = mp_init_multi(r, s, NULL, NULL, NULL, NULL)) != MP_OKAY){
                break;
            }

         
        #if defined(WOLFSSL_ATECC508A) || defined(PLUTON_CRYPTO_ECC)
            err = wc_ecc_sign_hash_hw(in, inlen, r, s, out, outlen, rng, key);
        #else
            err = wc_ecc_sign_hash_ex(in, inlen, rng, key, r, s);
        #endif
            if (err < 0) {
                break;
            }

            FALL_THROUGH;

        case ECC_STATE_SIGN_ENCODE:
            key->state = ECC_STATE_SIGN_ENCODE;

        #if defined(WOLFSSL_ASYNC_CRYPT) && defined(WC_ASYNC_ENABLE_ECC)
            if (key->asyncDev.marker == WOLFSSL_ASYNC_MARKER_ECC) {
                #ifdef HAVE_CAVIUM_V
                     
                    NitroxEccRsSplit(key, &r->raw, &s->raw);
                #endif
                #ifndef WOLFSSL_ASYNC_CRYPT_TEST
                     
                    wc_bigint_to_mp(&r->raw, r);
                    wc_bigint_to_mp(&s->raw, s);
                #endif
            }
        #endif  

             
            err = StoreECC_DSA_Sig(out, outlen, r, s);

             
            mp_clear(r);
            mp_clear(s);
            break;

        default:
            err = BAD_STATE_E;
            break;
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