size_t rand_drbg_get_nonce(RAND_DRBG *drbg,
                           unsigned char **pout,
                           int entropy, size_t min_len, size_t max_len)
{
    size_t ret = 0;
    RAND_POOL *pool;

    struct {
        void * instance;
        int count;
    } data;

    memset(&data, 0, sizeof(data));
    pool = rand_pool_new(0, 0, min_len, max_len);
    if (pool == NULL)
        return 0;

    if (rand_pool_add_nonce_data(pool) == 0)
        goto err;

    data.instance = drbg;
    CRYPTO_atomic_add(&rand_nonce_count, 1, &data.count, rand_nonce_lock);

    if (rand_pool_add(pool, (unsigned char *)&data, sizeof(data), 0) == 0)
        goto err;

    ret   = rand_pool_length(pool);
    *pout = rand_pool_detach(pool);

 err:
    rand_pool_free(pool);

    return ret;
}
