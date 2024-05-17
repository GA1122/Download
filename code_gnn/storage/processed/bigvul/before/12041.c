void *CRYPTO_THREAD_get_local(CRYPTO_THREAD_LOCAL *key)
{
    if (*key >= OPENSSL_CRYPTO_THREAD_LOCAL_KEY_MAX)
        return NULL;

    return thread_local_storage[*key];
}
