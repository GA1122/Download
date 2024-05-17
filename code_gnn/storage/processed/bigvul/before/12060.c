int CRYPTO_THREAD_unlock(CRYPTO_RWLOCK *lock)
{
    LeaveCriticalSection(lock);
    return 1;
}
