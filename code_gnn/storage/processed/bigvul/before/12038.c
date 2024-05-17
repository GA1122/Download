int CRYPTO_THREAD_cleanup_local(CRYPTO_THREAD_LOCAL *key)
{
    *key = OPENSSL_CRYPTO_THREAD_LOCAL_KEY_MAX + 1;
    return 1;
}
