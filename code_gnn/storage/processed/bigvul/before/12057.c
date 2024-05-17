CRYPTO_RWLOCK *CRYPTO_THREAD_lock_new(void)
{
    CRYPTO_RWLOCK *lock;

    if ((lock = OPENSSL_zalloc(sizeof(CRITICAL_SECTION))) == NULL) {
         
        return NULL;
    }

#if !defined(_WIN32_WCE)
     
    if (!InitializeCriticalSectionAndSpinCount(lock, 0x400)) {
        OPENSSL_free(lock);
        return NULL;
    }
#else
    InitializeCriticalSection(lock);
#endif

    return lock;
}
