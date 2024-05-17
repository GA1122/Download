static int tls_construct_cke_psk_preamble(SSL *s, unsigned char **p,
                                          size_t *pskhdrlen, int *al)
{
#ifndef OPENSSL_NO_PSK
    int ret = 0;
     
    char identity[PSK_MAX_IDENTITY_LEN + 1];
    size_t identitylen = 0;
    unsigned char psk[PSK_MAX_PSK_LEN];
    unsigned char *tmppsk = NULL;
    char *tmpidentity = NULL;
    size_t psklen = 0;

    if (s->psk_client_callback == NULL) {
        SSLerr(SSL_F_TLS_CONSTRUCT_CKE_PSK_PREAMBLE, SSL_R_PSK_NO_CLIENT_CB);
        *al = SSL_AD_INTERNAL_ERROR;
        goto err;
    }

    memset(identity, 0, sizeof(identity));

    psklen = s->psk_client_callback(s, s->session->psk_identity_hint,
                                    identity, sizeof(identity) - 1,
                                    psk, sizeof(psk));

    if (psklen > PSK_MAX_PSK_LEN) {
        SSLerr(SSL_F_TLS_CONSTRUCT_CKE_PSK_PREAMBLE, ERR_R_INTERNAL_ERROR);
        *al = SSL_AD_HANDSHAKE_FAILURE;
        goto err;
    } else if (psklen == 0) {
        SSLerr(SSL_F_TLS_CONSTRUCT_CKE_PSK_PREAMBLE,
               SSL_R_PSK_IDENTITY_NOT_FOUND);
        *al = SSL_AD_HANDSHAKE_FAILURE;
        goto err;
    }

    identitylen = strlen(identity);
    if (identitylen > PSK_MAX_IDENTITY_LEN) {
        SSLerr(SSL_F_TLS_CONSTRUCT_CKE_PSK_PREAMBLE, ERR_R_INTERNAL_ERROR);
        *al = SSL_AD_HANDSHAKE_FAILURE;
        goto err;
    }

    tmppsk = OPENSSL_memdup(psk, psklen);
    tmpidentity = OPENSSL_strdup(identity);
    if (tmppsk == NULL || tmpidentity == NULL) {
        SSLerr(SSL_F_TLS_CONSTRUCT_CKE_PSK_PREAMBLE, ERR_R_MALLOC_FAILURE);
        *al = SSL_AD_INTERNAL_ERROR;
        goto err;
    }

    OPENSSL_free(s->s3->tmp.psk);
    s->s3->tmp.psk = tmppsk;
    s->s3->tmp.psklen = psklen;
    tmppsk = NULL;
    OPENSSL_free(s->session->psk_identity);
    s->session->psk_identity = tmpidentity;
    tmpidentity = NULL;
    s2n(identitylen, *p);
    memcpy(*p, identity, identitylen);
    *pskhdrlen = 2 + identitylen;
    *p += identitylen;

    ret = 1;

 err:
    OPENSSL_cleanse(psk, psklen);
    OPENSSL_cleanse(identity, sizeof(identity));
    OPENSSL_clear_free(tmppsk, psklen);
    OPENSSL_clear_free(tmpidentity, identitylen);

    return ret;
#else
    SSLerr(SSL_F_TLS_CONSTRUCT_CKE_PSK_PREAMBLE, ERR_R_INTERNAL_ERROR);
    *al = SSL_AD_INTERNAL_ERROR;
    return 0;
#endif
}