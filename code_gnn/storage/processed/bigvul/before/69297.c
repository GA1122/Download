int tls1_generate_master_secret(SSL *s, unsigned char *out, unsigned char *p,
                                int len)
{
    if (s->session->flags & SSL_SESS_FLAG_EXTMS) {
        unsigned char hash[EVP_MAX_MD_SIZE * 2];
        int hashlen;
         
        if (!ssl3_digest_cached_records(s, 1))
            return -1;
        hashlen = ssl_handshake_hash(s, hash, sizeof(hash));
#ifdef SSL_DEBUG
        fprintf(stderr, "Handshake hashes:\n");
        BIO_dump_fp(stderr, (char *)hash, hashlen);
#endif
        tls1_PRF(s,
                 TLS_MD_EXTENDED_MASTER_SECRET_CONST,
                 TLS_MD_EXTENDED_MASTER_SECRET_CONST_SIZE,
                 hash, hashlen,
                 NULL, 0,
                 NULL, 0,
                 NULL, 0, p, len, s->session->master_key,
                 SSL3_MASTER_SECRET_SIZE);
        OPENSSL_cleanse(hash, hashlen);
    } else {
        tls1_PRF(s,
                 TLS_MD_MASTER_SECRET_CONST,
                 TLS_MD_MASTER_SECRET_CONST_SIZE,
                 s->s3->client_random, SSL3_RANDOM_SIZE,
                 NULL, 0,
                 s->s3->server_random, SSL3_RANDOM_SIZE,
                 NULL, 0, p, len, s->session->master_key,
                 SSL3_MASTER_SECRET_SIZE);
    }
#ifdef SSL_DEBUG
    fprintf(stderr, "Premaster Secret:\n");
    BIO_dump_fp(stderr, (char *)p, len);
    fprintf(stderr, "Client Random:\n");
    BIO_dump_fp(stderr, (char *)s->s3->client_random, SSL3_RANDOM_SIZE);
    fprintf(stderr, "Server Random:\n");
    BIO_dump_fp(stderr, (char *)s->s3->server_random, SSL3_RANDOM_SIZE);
    fprintf(stderr, "Master Secret:\n");
    BIO_dump_fp(stderr, (char *)s->session->master_key,
                SSL3_MASTER_SECRET_SIZE);
#endif

#ifdef OPENSSL_SSL_TRACE_CRYPTO
    if (s->msg_callback) {
        s->msg_callback(2, s->version, TLS1_RT_CRYPTO_PREMASTER,
                        p, len, s, s->msg_callback_arg);
        s->msg_callback(2, s->version, TLS1_RT_CRYPTO_CLIENT_RANDOM,
                        s->s3->client_random, SSL3_RANDOM_SIZE,
                        s, s->msg_callback_arg);
        s->msg_callback(2, s->version, TLS1_RT_CRYPTO_SERVER_RANDOM,
                        s->s3->server_random, SSL3_RANDOM_SIZE,
                        s, s->msg_callback_arg);
        s->msg_callback(2, s->version, TLS1_RT_CRYPTO_MASTER,
                        s->session->master_key,
                        SSL3_MASTER_SECRET_SIZE, s, s->msg_callback_arg);
    }
#endif

    return (SSL3_MASTER_SECRET_SIZE);
}