void OPENSSL_cleanup(void)
{
    OPENSSL_INIT_STOP *currhandler, *lasthandler;
    CRYPTO_THREAD_LOCAL key;

     
    if (!base_inited)
        return;

     
    if (stopped)
        return;
    stopped = 1;

     
    ossl_init_thread_stop(ossl_init_get_thread_local(0));

    currhandler = stop_handlers;
    while (currhandler != NULL) {
        currhandler->handler();
        lasthandler = currhandler;
        currhandler = currhandler->next;
        OPENSSL_free(lasthandler);
    }
    stop_handlers = NULL;

    CRYPTO_THREAD_lock_free(init_lock);
    init_lock = NULL;

     

#ifndef OPENSSL_NO_COMP
    if (zlib_inited) {
#ifdef OPENSSL_INIT_DEBUG
        fprintf(stderr, "OPENSSL_INIT: OPENSSL_cleanup: "
                        "comp_zlib_cleanup_int()\n");
#endif
        comp_zlib_cleanup_int();
    }
#endif

    if (async_inited) {
# ifdef OPENSSL_INIT_DEBUG
        fprintf(stderr, "OPENSSL_INIT: OPENSSL_cleanup: "
                        "async_deinit()\n");
# endif
        async_deinit();
    }

    if (load_crypto_strings_inited) {
#ifdef OPENSSL_INIT_DEBUG
        fprintf(stderr, "OPENSSL_INIT: OPENSSL_cleanup: "
                        "err_free_strings_int()\n");
#endif
        err_free_strings_int();
    }

    key = destructor_key.value;
    destructor_key.sane = -1;
    CRYPTO_THREAD_cleanup_local(&key);

#ifdef OPENSSL_INIT_DEBUG
    fprintf(stderr, "OPENSSL_INIT: OPENSSL_cleanup: "
                    "rand_cleanup_int()\n");
    fprintf(stderr, "OPENSSL_INIT: OPENSSL_cleanup: "
                    "conf_modules_free_int()\n");
#ifndef OPENSSL_NO_ENGINE
    fprintf(stderr, "OPENSSL_INIT: OPENSSL_cleanup: "
                    "engine_cleanup_int()\n");
#endif
    fprintf(stderr, "OPENSSL_INIT: OPENSSL_cleanup: "
                    "crypto_cleanup_all_ex_data_int()\n");
    fprintf(stderr, "OPENSSL_INIT: OPENSSL_cleanup: "
                    "bio_sock_cleanup_int()\n");
    fprintf(stderr, "OPENSSL_INIT: OPENSSL_cleanup: "
                    "bio_cleanup()\n");
    fprintf(stderr, "OPENSSL_INIT: OPENSSL_cleanup: "
                    "evp_cleanup_int()\n");
    fprintf(stderr, "OPENSSL_INIT: OPENSSL_cleanup: "
                    "obj_cleanup_int()\n");
    fprintf(stderr, "OPENSSL_INIT: OPENSSL_cleanup: "
                    "err_cleanup()\n");
#endif
     
    rand_cleanup_int();
    rand_drbg_cleanup_int();
    conf_modules_free_int();
#ifndef OPENSSL_NO_ENGINE
    engine_cleanup_int();
#endif
    ossl_store_cleanup_int();
    crypto_cleanup_all_ex_data_int();
    bio_cleanup();
    evp_cleanup_int();
    obj_cleanup_int();
    err_cleanup();

    CRYPTO_secure_malloc_done();

    base_inited = 0;
}
