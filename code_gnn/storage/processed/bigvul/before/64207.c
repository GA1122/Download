AP_CORE_DECLARE(void) ap_random_parent_after_fork(void)
{
     
    apr_uint16_t data;
    apr_random_insecure_bytes(rng, &data, sizeof(data));
}
