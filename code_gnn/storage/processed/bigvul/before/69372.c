static int tls_construct_cke_rsa(SSL *s, unsigned char **p, int *len, int *al)
{
#ifndef OPENSSL_NO_RSA
    unsigned char *q;
    EVP_PKEY *pkey = NULL;
    EVP_PKEY_CTX *pctx = NULL;
    size_t enclen;
    unsigned char *pms = NULL;
    size_t pmslen = 0;

    if (s->session->peer == NULL) {
         
        SSLerr(SSL_F_TLS_CONSTRUCT_CKE_RSA, ERR_R_INTERNAL_ERROR);
        return 0;
    }

    pkey = X509_get0_pubkey(s->session->peer);
    if (EVP_PKEY_get0_RSA(pkey) == NULL) {
        SSLerr(SSL_F_TLS_CONSTRUCT_CKE_RSA, ERR_R_INTERNAL_ERROR);
        return 0;
    }

    pmslen = SSL_MAX_MASTER_KEY_LENGTH;
    pms = OPENSSL_malloc(pmslen);
    if (pms == NULL) {
        SSLerr(SSL_F_TLS_CONSTRUCT_CKE_RSA, ERR_R_MALLOC_FAILURE);
        *al = SSL_AD_INTERNAL_ERROR;
        return 0;
    }

    pms[0] = s->client_version >> 8;
    pms[1] = s->client_version & 0xff;
    if (RAND_bytes(pms + 2, pmslen - 2) <= 0) {
        goto err;
    }

    q = *p;
     
    if (s->version > SSL3_VERSION)
        *p += 2;
    pctx = EVP_PKEY_CTX_new(pkey, NULL);
    if (pctx == NULL || EVP_PKEY_encrypt_init(pctx) <= 0
        || EVP_PKEY_encrypt(pctx, NULL, &enclen, pms, pmslen) <= 0) {
        SSLerr(SSL_F_TLS_CONSTRUCT_CKE_RSA, ERR_R_EVP_LIB);
        goto err;
    }
    if (EVP_PKEY_encrypt(pctx, *p, &enclen, pms, pmslen) <= 0) {
        SSLerr(SSL_F_TLS_CONSTRUCT_CKE_RSA, SSL_R_BAD_RSA_ENCRYPT);
        goto err;
    }
    *len = enclen;
    EVP_PKEY_CTX_free(pctx);
    pctx = NULL;
# ifdef PKCS1_CHECK
    if (s->options & SSL_OP_PKCS1_CHECK_1)
        (*p)[1]++;
    if (s->options & SSL_OP_PKCS1_CHECK_2)
        tmp_buf[0] = 0x70;
# endif

     
    if (s->version > SSL3_VERSION) {
        s2n(*len, q);
        *len += 2;
    }

    s->s3->tmp.pms = pms;
    s->s3->tmp.pmslen = pmslen;

    return 1;
 err:
    OPENSSL_clear_free(pms, pmslen);
    EVP_PKEY_CTX_free(pctx);

    return 0;
#else
    SSLerr(SSL_F_TLS_CONSTRUCT_CKE_RSA, ERR_R_INTERNAL_ERROR);
    *al = SSL_AD_INTERNAL_ERROR;
    return 0;
#endif
}