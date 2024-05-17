static int ssl3_check_client_certificate(SSL *s)
{
    if (!s->cert || !s->cert->key->x509 || !s->cert->key->privatekey)
        return 0;
     
    if (SSL_USE_SIGALGS(s) && !s->s3->tmp.md[s->cert->key - s->cert->pkeys])
        return 0;
     
    if (s->cert->cert_flags & SSL_CERT_FLAGS_CHECK_TLS_STRICT &&
        !tls1_check_chain(s, NULL, NULL, NULL, -2))
        return 0;
    return 1;
}