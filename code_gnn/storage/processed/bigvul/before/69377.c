int tls_construct_client_verify(SSL *s)
{
    unsigned char *p;
    EVP_PKEY *pkey;
    const EVP_MD *md = s->s3->tmp.md[s->cert->key - s->cert->pkeys];
    EVP_MD_CTX *mctx;
    unsigned u = 0;
    unsigned long n = 0;
    long hdatalen = 0;
    void *hdata;

    mctx = EVP_MD_CTX_new();
    if (mctx == NULL) {
        SSLerr(SSL_F_TLS_CONSTRUCT_CLIENT_VERIFY, ERR_R_MALLOC_FAILURE);
        goto err;
    }

    p = ssl_handshake_start(s);
    pkey = s->cert->key->privatekey;

    hdatalen = BIO_get_mem_data(s->s3->handshake_buffer, &hdata);
    if (hdatalen <= 0) {
        SSLerr(SSL_F_TLS_CONSTRUCT_CLIENT_VERIFY, ERR_R_INTERNAL_ERROR);
        goto err;
    }
    if (SSL_USE_SIGALGS(s)) {
        if (!tls12_get_sigandhash(p, pkey, md)) {
            SSLerr(SSL_F_TLS_CONSTRUCT_CLIENT_VERIFY, ERR_R_INTERNAL_ERROR);
            goto err;
        }
        p += 2;
        n = 2;
    }
#ifdef SSL_DEBUG
    fprintf(stderr, "Using client alg %s\n", EVP_MD_name(md));
#endif
    if (!EVP_SignInit_ex(mctx, md, NULL)
        || !EVP_SignUpdate(mctx, hdata, hdatalen)
        || (s->version == SSL3_VERSION
            && !EVP_MD_CTX_ctrl(mctx, EVP_CTRL_SSL3_MASTER_SECRET,
                                s->session->master_key_length,
                                s->session->master_key))
        || !EVP_SignFinal(mctx, p + 2, &u, pkey)) {
        SSLerr(SSL_F_TLS_CONSTRUCT_CLIENT_VERIFY, ERR_R_EVP_LIB);
        goto err;
    }
#ifndef OPENSSL_NO_GOST
    {
        int pktype = EVP_PKEY_id(pkey);
        if (pktype == NID_id_GostR3410_2001
            || pktype == NID_id_GostR3410_2012_256
            || pktype == NID_id_GostR3410_2012_512)
            BUF_reverse(p + 2, NULL, u);
    }
#endif

    s2n(u, p);
    n += u + 2;
     
    if (!ssl3_digest_cached_records(s, 0))
        goto err;
    if (!ssl_set_handshake_header(s, SSL3_MT_CERTIFICATE_VERIFY, n)) {
        SSLerr(SSL_F_TLS_CONSTRUCT_CLIENT_VERIFY, ERR_R_INTERNAL_ERROR);
        goto err;
    }

    EVP_MD_CTX_free(mctx);
    return 1;
 err:
    EVP_MD_CTX_free(mctx);
    return 0;
}