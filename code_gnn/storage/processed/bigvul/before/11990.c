DEFINE_RUN_ONCE_STATIC(ossl_init_base)
{
    CRYPTO_THREAD_LOCAL key;

#ifdef OPENSSL_INIT_DEBUG
    fprintf(stderr, "OPENSSL_INIT: ossl_init_base: Setting up stop handlers\n");
#endif
#ifndef OPENSSL_NO_CRYPTO_MDEBUG
    ossl_malloc_setup_failures();
#endif
    if (!CRYPTO_THREAD_init_local(&key, ossl_init_thread_destructor))
        return 0;
    if ((init_lock = CRYPTO_THREAD_lock_new()) == NULL)
        goto err;
    OPENSSL_cpuid_setup();

    destructor_key.value = key;
    base_inited = 1;
    return 1;

err:
#ifdef OPENSSL_INIT_DEBUG
    fprintf(stderr, "OPENSSL_INIT: ossl_init_base not ok!\n");
#endif
    CRYPTO_THREAD_lock_free(init_lock);
    init_lock = NULL;

    CRYPTO_THREAD_cleanup_local(&key);
    return 0;
}
