int CRYPTO_atomic_add(int *val, int amount, int *ret, CRYPTO_RWLOCK *lock)
{
    *ret = InterlockedExchangeAdd(val, amount) + amount;
    return 1;
}
