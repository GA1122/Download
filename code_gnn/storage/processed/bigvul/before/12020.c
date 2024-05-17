static void ossl_init_thread_stop(struct thread_local_inits_st *locals)
{
     
    if (locals == NULL)
        return;

    if (locals->async) {
#ifdef OPENSSL_INIT_DEBUG
        fprintf(stderr, "OPENSSL_INIT: ossl_init_thread_stop: "
                        "async_delete_thread_state()\n");
#endif
        async_delete_thread_state();
    }

    if (locals->err_state) {
#ifdef OPENSSL_INIT_DEBUG
        fprintf(stderr, "OPENSSL_INIT: ossl_init_thread_stop: "
                        "err_delete_thread_state()\n");
#endif
        err_delete_thread_state();
    }

    if (locals->rand) {
#ifdef OPENSSL_INIT_DEBUG
        fprintf(stderr, "OPENSSL_INIT: ossl_init_thread_stop: "
                        "drbg_delete_thread_state()\n");
#endif
        drbg_delete_thread_state();
    }

    OPENSSL_free(locals);
}
