static int tls_construct_cke_srp(SSL *s, unsigned char **p, int *len, int *al)
{
#ifndef OPENSSL_NO_SRP
    if (s->srp_ctx.A != NULL) {
         
        *len = BN_num_bytes(s->srp_ctx.A);
        s2n(*len, *p);
        BN_bn2bin(s->srp_ctx.A, *p);
        *len += 2;
    } else {
        SSLerr(SSL_F_TLS_CONSTRUCT_CKE_SRP, ERR_R_INTERNAL_ERROR);
        return 0;
    }
    OPENSSL_free(s->session->srp_username);
    s->session->srp_username = OPENSSL_strdup(s->srp_ctx.login);
    if (s->session->srp_username == NULL) {
        SSLerr(SSL_F_TLS_CONSTRUCT_CKE_SRP, ERR_R_MALLOC_FAILURE);
        return 0;
    }

    return 1;
#else
    SSLerr(SSL_F_TLS_CONSTRUCT_CKE_SRP, ERR_R_INTERNAL_ERROR);
    *al = SSL_AD_INTERNAL_ERROR;
    return 0;
#endif
}