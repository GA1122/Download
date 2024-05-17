int CRYPTO_THREAD_write_lock(CRYPTO_RWLOCK *lock)
{
    EnterCriticalSection(lock);
    return 1;
}
