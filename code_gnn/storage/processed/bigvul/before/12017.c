static struct thread_local_inits_st *ossl_init_get_thread_local(int alloc)
{
    struct thread_local_inits_st *local =
        CRYPTO_THREAD_get_local(&destructor_key.value);

    if (alloc) {
        if (local == NULL
            && (local = OPENSSL_zalloc(sizeof(*local))) != NULL
            && !CRYPTO_THREAD_set_local(&destructor_key.value, local)) {
            OPENSSL_free(local);
            return NULL;
        }
    } else {
        CRYPTO_THREAD_set_local(&destructor_key.value, NULL);
    }

    return local;
}
