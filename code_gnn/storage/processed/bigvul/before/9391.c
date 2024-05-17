int ssl3_check_client_hello(SSL *s)
{
    int ok;
    long n;

     
    n = s->method->ssl_get_message(s,
                                   SSL3_ST_SR_CERT_A,
                                   SSL3_ST_SR_CERT_B,
                                   -1, s->max_cert_list, &ok);
    if (!ok)
        return ((int)n);
    s->s3->tmp.reuse_message = 1;
    if (s->s3->tmp.message_type == SSL3_MT_CLIENT_HELLO) {
         
        if (s->s3->flags & SSL3_FLAGS_SGC_RESTART_DONE) {
            SSLerr(SSL_F_SSL3_CHECK_CLIENT_HELLO,
                   SSL_R_MULTIPLE_SGC_RESTARTS);
            return -1;
        }
         
#ifndef OPENSSL_NO_DH
        if (s->s3->tmp.dh != NULL) {
            DH_free(s->s3->tmp.dh);
            s->s3->tmp.dh = NULL;
        }
#endif
#ifndef OPENSSL_NO_ECDH
        if (s->s3->tmp.ecdh != NULL) {
            EC_KEY_free(s->s3->tmp.ecdh);
            s->s3->tmp.ecdh = NULL;
        }
#endif
        s->s3->flags |= SSL3_FLAGS_SGC_RESTART_DONE;
        return 2;
    }
    return 1;
}