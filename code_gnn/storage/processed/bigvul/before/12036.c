size_t rand_drbg_get_entropy(RAND_DRBG *drbg,
                             unsigned char **pout,
                             int entropy, size_t min_len, size_t max_len,
                             int prediction_resistance)
{
    size_t ret = 0;
    size_t entropy_available = 0;
    RAND_POOL *pool;

    if (drbg->parent != NULL && drbg->strength > drbg->parent->strength) {
         
        RANDerr(RAND_F_RAND_DRBG_GET_ENTROPY, RAND_R_PARENT_STRENGTH_TOO_WEAK);
        return 0;
    }

    if (drbg->seed_pool != NULL) {
        pool = drbg->seed_pool;
        pool->entropy_requested = entropy;
    } else {
        pool = rand_pool_new(entropy, drbg->secure, min_len, max_len);
        if (pool == NULL)
            return 0;
    }

    if (drbg->parent != NULL) {
        size_t bytes_needed = rand_pool_bytes_needed(pool, 1  );
        unsigned char *buffer = rand_pool_add_begin(pool, bytes_needed);

        if (buffer != NULL) {
            size_t bytes = 0;

             
            rand_drbg_lock(drbg->parent);
            if (RAND_DRBG_generate(drbg->parent,
                                   buffer, bytes_needed,
                                   prediction_resistance,
                                   NULL, 0) != 0)
                bytes = bytes_needed;
            drbg->reseed_next_counter
                = tsan_load(&drbg->parent->reseed_prop_counter);
            rand_drbg_unlock(drbg->parent);

            rand_pool_add_end(pool, bytes, 8 * bytes);
            entropy_available = rand_pool_entropy_available(pool);
        }

    } else {
        if (prediction_resistance) {
             
            RANDerr(RAND_F_RAND_DRBG_GET_ENTROPY,
                    RAND_R_PREDICTION_RESISTANCE_NOT_SUPPORTED);
            goto err;
        }

         
        entropy_available = rand_pool_acquire_entropy(pool);
    }

    if (entropy_available > 0) {
        ret   = rand_pool_length(pool);
        *pout = rand_pool_detach(pool);
    }

 err:
    if (drbg->seed_pool == NULL)
        rand_pool_free(pool);
    return ret;
}
