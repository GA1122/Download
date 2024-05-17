 CRYPTO_RWLOCK *CRYPTO_THREAD_lock_new(void)
 {
     CRYPTO_RWLOCK *lock;

    *(unsigned int *)lock = 1;

    return lock;
}
