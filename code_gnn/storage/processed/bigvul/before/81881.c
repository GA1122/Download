int wc_ecc_free(ecc_key* key)
{
    if (key == NULL) {
        return 0;
    }

#ifdef WOLFSSL_ASYNC_CRYPT
    #ifdef WC_ASYNC_ENABLE_ECC
    wolfAsync_DevCtxFree(&key->asyncDev, WOLFSSL_ASYNC_MARKER_ECC);
    #endif
    wc_ecc_free_async(key);
#endif

#ifdef WOLFSSL_ATECC508A
    atmel_ecc_free(key->slot);
    key->slot = -1;
#else

    mp_clear(key->pubkey.x);
    mp_clear(key->pubkey.y);
    mp_clear(key->pubkey.z);

    mp_forcezero(&key->k);
#endif  

#ifdef WOLFSSL_CUSTOM_CURVES
    if (key->deallocSet && key->dp != NULL)
        wc_ecc_free_curve(key->dp, key->heap);
#endif

    return 0;
}