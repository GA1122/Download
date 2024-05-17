void rand_drbg_cleanup_nonce(RAND_DRBG *drbg,
                             unsigned char *out, size_t outlen)
{
    OPENSSL_clear_free(out, outlen);
}
