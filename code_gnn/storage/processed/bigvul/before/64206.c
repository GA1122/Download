AP_DECLARE(void) ap_random_insecure_bytes(void *buf, apr_size_t size)
{
#if APR_HAS_THREADS
    if (rng_mutex)
        apr_thread_mutex_lock(rng_mutex);
#endif
     
    apr_random_insecure_bytes(rng, buf, size);
#if APR_HAS_THREADS
    if (rng_mutex)
        apr_thread_mutex_unlock(rng_mutex);
#endif
}
