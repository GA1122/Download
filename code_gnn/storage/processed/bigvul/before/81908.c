int wc_ecc_make_key_ex(WC_RNG* rng, int keysize, ecc_key* key, int curve_id)
{
    int            err;
#ifndef WOLFSSL_ATECC508A
    DECLARE_CURVE_SPECS(ECC_CURVE_FIELD_COUNT)
#endif

    if (key == NULL || rng == NULL) {
        return BAD_FUNC_ARG;
    }

     
    wc_ecc_reset(key);

    err = wc_ecc_set_curve(key, keysize, curve_id);
    if (err != 0) {
        return err;
    }

#if defined(WOLFSSL_ASYNC_CRYPT) && defined(WC_ASYNC_ENABLE_ECC)
    if (key->asyncDev.marker == WOLFSSL_ASYNC_MARKER_ECC) {
    #ifdef HAVE_CAVIUM
         
    #elif defined(HAVE_INTEL_QA)
         
    #else
        if (wc_AsyncTestInit(&key->asyncDev, ASYNC_TEST_ECC_MAKE)) {
            WC_ASYNC_TEST* testDev = &key->asyncDev.test;
            testDev->eccMake.rng = rng;
            testDev->eccMake.key = key;
            testDev->eccMake.size = keysize;
            testDev->eccMake.curve_id = curve_id;
            return WC_PENDING_E;
        }
    #endif
    }
#endif  

#ifdef WOLFSSL_ATECC508A
   key->type = ECC_PRIVATEKEY;
   err = atcatls_create_key(key->slot, key->pubkey_raw);
   if (err != ATCA_SUCCESS) {
      err = BAD_COND_E;
   }

    
   err = mp_read_unsigned_bin(key->pubkey.x, key->pubkey_raw,
                              ECC_MAX_CRYPTO_HW_SIZE);
   if (err = MP_OKAY)
       err = mp_read_unsigned_bin(key->pubkey.y,
                                  key->pubkey_raw + ECC_MAX_CRYPTO_HW_SIZE,
                                  ECC_MAX_CRYPTO_HW_SIZE);
#else

#ifdef WOLFSSL_HAVE_SP_ECC
#ifndef WOLFSSL_SP_NO_256
    if (key->idx != ECC_CUSTOM_IDX && ecc_sets[key->idx].id == ECC_SECP256R1) {
        err = sp_ecc_make_key_256(rng, &key->k, &key->pubkey, key->heap);
        if (err == MP_OKAY)
            key->type = ECC_PRIVATEKEY;
    }
    else
#endif
#endif
#ifdef WOLFSSL_SP_MATH
        err = WC_KEY_SIZE_E;
#else
    {
         
        err = mp_init(&key->k);

         
        if (err == MP_OKAY)
            err = wc_ecc_curve_load(key->dp, &curve, ECC_CURVE_FIELD_ALL);

         
        if (err == MP_OKAY)
            err = wc_ecc_gen_k(rng, key->dp->size, &key->k, curve->order);

         
        if (err == MP_OKAY)
            err = wc_ecc_make_pub_ex(key, curve, NULL);

        if (err == MP_OKAY)
            key->type = ECC_PRIVATEKEY;

         
        if (err != MP_OKAY) {
             
            mp_forcezero(&key->k);
        }

         
        wc_ecc_curve_free(curve);
    }
#endif

#endif  

    return err;
}
