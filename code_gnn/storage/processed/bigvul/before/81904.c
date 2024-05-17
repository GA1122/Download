int wc_ecc_init_ex(ecc_key* key, void* heap, int devId)
{
    int ret = 0;

    if (key == NULL) {
        return BAD_FUNC_ARG;
    }

#ifdef ECC_DUMP_OID
    wc_ecc_dump_oids();
#endif

    XMEMSET(key, 0, sizeof(ecc_key));
    key->state = ECC_STATE_NONE;

#if defined(PLUTON_CRYPTO_ECC) || defined(WOLF_CRYPTO_DEV)
    key->devId = devId;
#else
    (void)devId;
#endif

#ifdef WOLFSSL_ATECC508A
    key->slot = atmel_ecc_alloc();
    if (key->slot == ATECC_INVALID_SLOT) {
        return ECC_BAD_ARG_E;
    }
#else
#ifdef ALT_ECC_SIZE
    key->pubkey.x = (mp_int*)&key->pubkey.xyz[0];
    key->pubkey.y = (mp_int*)&key->pubkey.xyz[1];
    key->pubkey.z = (mp_int*)&key->pubkey.xyz[2];
    alt_fp_init(key->pubkey.x);
    alt_fp_init(key->pubkey.y);
    alt_fp_init(key->pubkey.z);
    ret = mp_init(&key->k);
#else
    ret = mp_init_multi(&key->k, key->pubkey.x, key->pubkey.y, key->pubkey.z,
                                                                    NULL, NULL);
#endif  
    if (ret != MP_OKAY) {
        return MEMORY_E;
    }
#endif  

#ifdef WOLFSSL_HEAP_TEST
    key->heap = (void*)WOLFSSL_HEAP_TEST;
#else
    key->heap = heap;
#endif

#if defined(WOLFSSL_ASYNC_CRYPT) && defined(WC_ASYNC_ENABLE_ECC)
     
    ret = wolfAsync_DevCtxInit(&key->asyncDev, WOLFSSL_ASYNC_MARKER_ECC,
                                                            key->heap, devId);
#endif

    return ret;
}