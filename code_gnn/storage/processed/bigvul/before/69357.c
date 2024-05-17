static ossl_inline int cert_req_allowed(SSL *s)
{
     
    if ((s->version > SSL3_VERSION
         && (s->s3->tmp.new_cipher->algorithm_auth & SSL_aNULL))
        || (s->s3->tmp.new_cipher->algorithm_auth & (SSL_aSRP | SSL_aPSK)))
        return 0;

    return 1;
}