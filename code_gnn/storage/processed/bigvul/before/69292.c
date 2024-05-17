static int tls1_PRF(SSL *s,
                    const void *seed1, int seed1_len,
                    const void *seed2, int seed2_len,
                    const void *seed3, int seed3_len,
                    const void *seed4, int seed4_len,
                    const void *seed5, int seed5_len,
                    const unsigned char *sec, int slen,
                    unsigned char *out, int olen)
{
    const EVP_MD *md = ssl_prf_md(s);
    EVP_PKEY_CTX *pctx = NULL;

    int ret = 0;
    size_t outlen = olen;

    if (md == NULL) {
         
        SSLerr(SSL_F_TLS1_PRF, ERR_R_INTERNAL_ERROR);
        return 0;
    }
    pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_TLS1_PRF, NULL);
    if (pctx == NULL || EVP_PKEY_derive_init(pctx) <= 0
        || EVP_PKEY_CTX_set_tls1_prf_md(pctx, md) <= 0
        || EVP_PKEY_CTX_set1_tls1_prf_secret(pctx, sec, slen) <= 0)
        goto err;

    if (EVP_PKEY_CTX_add1_tls1_prf_seed(pctx, seed1, seed1_len) <= 0)
        goto err;
    if (EVP_PKEY_CTX_add1_tls1_prf_seed(pctx, seed2, seed2_len) <= 0)
        goto err;
    if (EVP_PKEY_CTX_add1_tls1_prf_seed(pctx, seed3, seed3_len) <= 0)
        goto err;
    if (EVP_PKEY_CTX_add1_tls1_prf_seed(pctx, seed4, seed4_len) <= 0)
        goto err;
    if (EVP_PKEY_CTX_add1_tls1_prf_seed(pctx, seed5, seed5_len) <= 0)
        goto err;

    if (EVP_PKEY_derive(pctx, out, &outlen) <= 0)
        goto err;
    ret = 1;

 err:
    EVP_PKEY_CTX_free(pctx);
    return ret;
}