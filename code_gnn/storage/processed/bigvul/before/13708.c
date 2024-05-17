int ssl3_get_client_hello(SSL *s)
{
    int i, j, ok, al = SSL_AD_INTERNAL_ERROR, ret = -1, cookie_valid = 0;
    unsigned int cookie_len;
    long n;
    unsigned long id;
    unsigned char *p, *d;
    SSL_CIPHER *c;
#ifndef OPENSSL_NO_COMP
    unsigned char *q;
    SSL_COMP *comp = NULL;
#endif
    STACK_OF(SSL_CIPHER) *ciphers = NULL;

    if (s->state == SSL3_ST_SR_CLNT_HELLO_C && !s->first_packet)
        goto retry_cert;

     
    if (s->state == SSL3_ST_SR_CLNT_HELLO_A) {
        s->state = SSL3_ST_SR_CLNT_HELLO_B;
    }
    s->first_packet = 1;
    n = s->method->ssl_get_message(s,
                                   SSL3_ST_SR_CLNT_HELLO_B,
                                   SSL3_ST_SR_CLNT_HELLO_C,
                                   SSL3_MT_CLIENT_HELLO,
                                   SSL3_RT_MAX_PLAIN_LENGTH, &ok);

    if (!ok)
        return ((int)n);
    s->first_packet = 0;
    d = p = (unsigned char *)s->init_msg;

     
    if (n < 2 + SSL3_RANDOM_SIZE + 1) {
        al = SSL_AD_DECODE_ERROR;
        SSLerr(SSL_F_SSL3_GET_CLIENT_HELLO, SSL_R_LENGTH_TOO_SHORT);
        goto f_err;
    }

     
    s->client_version = (((int)p[0]) << 8) | (int)p[1];
    p += 2;

    if (SSL_IS_DTLS(s) ? (s->client_version > s->version &&
                          s->method->version != DTLS_ANY_VERSION)
        : (s->client_version < s->version)) {
        SSLerr(SSL_F_SSL3_GET_CLIENT_HELLO, SSL_R_WRONG_VERSION_NUMBER);
        if ((s->client_version >> 8) == SSL3_VERSION_MAJOR &&
            !s->enc_write_ctx && !s->write_hash) {
             
            s->version = s->client_version;
        }
        al = SSL_AD_PROTOCOL_VERSION;
        goto f_err;
    }

     
    if (SSL_get_options(s) & SSL_OP_COOKIE_EXCHANGE) {
        unsigned int session_length, cookie_length;

        session_length = *(p + SSL3_RANDOM_SIZE);

        if (p + SSL3_RANDOM_SIZE + session_length + 1 >= d + n) {
            al = SSL_AD_DECODE_ERROR;
            SSLerr(SSL_F_SSL3_GET_CLIENT_HELLO, SSL_R_LENGTH_TOO_SHORT);
            goto f_err;
        }
        cookie_length = *(p + SSL3_RANDOM_SIZE + session_length + 1);

        if (cookie_length == 0)
            return 1;
    }

     
    memcpy(s->s3->client_random, p, SSL3_RANDOM_SIZE);
    p += SSL3_RANDOM_SIZE;

     
    j = *(p++);

    if (p + j > d + n) {
        al = SSL_AD_DECODE_ERROR;
        SSLerr(SSL_F_SSL3_GET_CLIENT_HELLO, SSL_R_LENGTH_TOO_SHORT);
        goto f_err;
    }

    if ((j < 0) || (j > SSL_MAX_SSL_SESSION_ID_LENGTH)) {
        al = SSL_AD_DECODE_ERROR;
        SSLerr(SSL_F_SSL3_GET_CLIENT_HELLO, SSL_R_LENGTH_MISMATCH);
        goto f_err;
    }

    s->hit = 0;
     
    if ((s->new_session
         && (s->options & SSL_OP_NO_SESSION_RESUMPTION_ON_RENEGOTIATION))) {
        if (!ssl_get_new_session(s, 1))
            goto err;
    } else {
        i = ssl_get_prev_session(s, p, j, d + n);
         
        if (i == 1 && s->version == s->session->ssl_version) {  
            s->hit = 1;
        } else if (i == -1)
            goto err;
        else {                   

            if (!ssl_get_new_session(s, 1))
                goto err;
        }
    }

    p += j;

    if (SSL_IS_DTLS(s)) {
         
        if (p + 1 > d + n) {
            al = SSL_AD_DECODE_ERROR;
            SSLerr(SSL_F_SSL3_GET_CLIENT_HELLO, SSL_R_LENGTH_TOO_SHORT);
            goto f_err;
        }
        cookie_len = *(p++);

        if (p + cookie_len > d + n) {
            al = SSL_AD_DECODE_ERROR;
            SSLerr(SSL_F_SSL3_GET_CLIENT_HELLO, SSL_R_LENGTH_TOO_SHORT);
            goto f_err;
        }

         
        if (cookie_len > sizeof(s->d1->rcvd_cookie)) {
             
            al = SSL_AD_DECODE_ERROR;
            SSLerr(SSL_F_SSL3_GET_CLIENT_HELLO, SSL_R_COOKIE_MISMATCH);
            goto f_err;
        }

         
        if ((SSL_get_options(s) & SSL_OP_COOKIE_EXCHANGE) && cookie_len > 0) {
            memcpy(s->d1->rcvd_cookie, p, cookie_len);

            if (s->ctx->app_verify_cookie_cb != NULL) {
                if (s->ctx->app_verify_cookie_cb(s, s->d1->rcvd_cookie,
                                                 cookie_len) == 0) {
                    al = SSL_AD_HANDSHAKE_FAILURE;
                    SSLerr(SSL_F_SSL3_GET_CLIENT_HELLO,
                           SSL_R_COOKIE_MISMATCH);
                    goto f_err;
                }
                 
            }
             
            else if (memcmp(s->d1->rcvd_cookie, s->d1->cookie,
                            s->d1->cookie_len) != 0) {
                al = SSL_AD_HANDSHAKE_FAILURE;
                SSLerr(SSL_F_SSL3_GET_CLIENT_HELLO, SSL_R_COOKIE_MISMATCH);
                goto f_err;
            }
            cookie_valid = 1;
        }

        p += cookie_len;
        if (s->method->version == DTLS_ANY_VERSION) {
             
            if (s->client_version <= DTLS1_2_VERSION &&
                !(s->options & SSL_OP_NO_DTLSv1_2)) {
                s->version = DTLS1_2_VERSION;
                s->method = DTLSv1_2_server_method();
            } else if (tls1_suiteb(s)) {
                SSLerr(SSL_F_SSL3_GET_CLIENT_HELLO,
                       SSL_R_ONLY_DTLS_1_2_ALLOWED_IN_SUITEB_MODE);
                s->version = s->client_version;
                al = SSL_AD_PROTOCOL_VERSION;
                goto f_err;
            } else if (s->client_version <= DTLS1_VERSION &&
                       !(s->options & SSL_OP_NO_DTLSv1)) {
                s->version = DTLS1_VERSION;
                s->method = DTLSv1_server_method();
            } else {
                SSLerr(SSL_F_SSL3_GET_CLIENT_HELLO,
                       SSL_R_WRONG_VERSION_NUMBER);
                s->version = s->client_version;
                al = SSL_AD_PROTOCOL_VERSION;
                goto f_err;
            }
            s->session->ssl_version = s->version;
        }
    }

    if (p + 2 > d + n) {
        al = SSL_AD_DECODE_ERROR;
        SSLerr(SSL_F_SSL3_GET_CLIENT_HELLO, SSL_R_LENGTH_TOO_SHORT);
        goto f_err;
    }
    n2s(p, i);

    if (i == 0) {
        al = SSL_AD_ILLEGAL_PARAMETER;
        SSLerr(SSL_F_SSL3_GET_CLIENT_HELLO, SSL_R_NO_CIPHERS_SPECIFIED);
        goto f_err;
    }

     
    if ((p + i + 1) > (d + n)) {
         
        al = SSL_AD_DECODE_ERROR;
        SSLerr(SSL_F_SSL3_GET_CLIENT_HELLO, SSL_R_LENGTH_MISMATCH);
        goto f_err;
    }
    if (ssl_bytes_to_cipher_list(s, p, i, &(ciphers)) == NULL) {
        goto err;
    }
    p += i;

     
    if (s->hit) {
        j = 0;
        id = s->session->cipher->id;

#ifdef CIPHER_DEBUG
        fprintf(stderr, "client sent %d ciphers\n",
                sk_SSL_CIPHER_num(ciphers));
#endif
        for (i = 0; i < sk_SSL_CIPHER_num(ciphers); i++) {
            c = sk_SSL_CIPHER_value(ciphers, i);
#ifdef CIPHER_DEBUG
            fprintf(stderr, "client [%2d of %2d]:%s\n",
                    i, sk_SSL_CIPHER_num(ciphers), SSL_CIPHER_get_name(c));
#endif
            if (c->id == id) {
                j = 1;
                break;
            }
        }
         
#if 0
        if (j == 0 && (s->options & SSL_OP_NETSCAPE_REUSE_CIPHER_CHANGE_BUG)
            && (sk_SSL_CIPHER_num(ciphers) == 1)) {
             
            c = sk_SSL_CIPHER_value(ciphers, 0);
            if (sk_SSL_CIPHER_find(SSL_get_ciphers(s), c) >= 0) {
                s->session->cipher = c;
                j = 1;
            }
        }
#endif
        if (j == 0) {
             
            al = SSL_AD_ILLEGAL_PARAMETER;
            SSLerr(SSL_F_SSL3_GET_CLIENT_HELLO,
                   SSL_R_REQUIRED_CIPHER_MISSING);
            goto f_err;
        }
    }

     
    i = *(p++);
    if ((p + i) > (d + n)) {
         
        al = SSL_AD_DECODE_ERROR;
        SSLerr(SSL_F_SSL3_GET_CLIENT_HELLO, SSL_R_LENGTH_MISMATCH);
        goto f_err;
    }
#ifndef OPENSSL_NO_COMP
    q = p;
#endif
    for (j = 0; j < i; j++) {
        if (p[j] == 0)
            break;
    }

    p += i;
    if (j >= i) {
         
        al = SSL_AD_DECODE_ERROR;
        SSLerr(SSL_F_SSL3_GET_CLIENT_HELLO, SSL_R_NO_COMPRESSION_SPECIFIED);
        goto f_err;
    }
#ifndef OPENSSL_NO_TLSEXT
     
    if (s->version >= SSL3_VERSION) {
        if (!ssl_parse_clienthello_tlsext(s, &p, d + n)) {
            SSLerr(SSL_F_SSL3_GET_CLIENT_HELLO, SSL_R_PARSE_TLSEXT);
            goto err;
        }
    }

     
    {
        unsigned char *pos;
        pos = s->s3->server_random;
        if (ssl_fill_hello_random(s, 1, pos, SSL3_RANDOM_SIZE) <= 0) {
            goto f_err;
        }
    }

    if (!s->hit && s->version >= TLS1_VERSION && s->tls_session_secret_cb) {
        SSL_CIPHER *pref_cipher = NULL;

        s->session->master_key_length = sizeof(s->session->master_key);
        if (s->tls_session_secret_cb(s, s->session->master_key,
                                     &s->session->master_key_length, ciphers,
                                     &pref_cipher,
                                     s->tls_session_secret_cb_arg)) {
            s->hit = 1;
            s->session->ciphers = ciphers;
            s->session->verify_result = X509_V_OK;

            ciphers = NULL;

             
            pref_cipher =
                pref_cipher ? pref_cipher : ssl3_choose_cipher(s,
                                                               s->
                                                               session->ciphers,
                                                               SSL_get_ciphers
                                                               (s));
            if (pref_cipher == NULL) {
                al = SSL_AD_HANDSHAKE_FAILURE;
                SSLerr(SSL_F_SSL3_GET_CLIENT_HELLO, SSL_R_NO_SHARED_CIPHER);
                goto f_err;
            }

            s->session->cipher = pref_cipher;

            if (s->cipher_list)
                sk_SSL_CIPHER_free(s->cipher_list);

            if (s->cipher_list_by_id)
                sk_SSL_CIPHER_free(s->cipher_list_by_id);

            s->cipher_list = sk_SSL_CIPHER_dup(s->session->ciphers);
            s->cipher_list_by_id = sk_SSL_CIPHER_dup(s->session->ciphers);
        }
    }
#endif

     
    s->s3->tmp.new_compression = NULL;
#ifndef OPENSSL_NO_COMP
     
    if (s->session->compress_meth != 0) {
        int m, comp_id = s->session->compress_meth;
         
         
        if (s->options & SSL_OP_NO_COMPRESSION) {
            SSLerr(SSL_F_SSL3_GET_CLIENT_HELLO,
                   SSL_R_INCONSISTENT_COMPRESSION);
            goto f_err;
        }
         
        for (m = 0; m < sk_SSL_COMP_num(s->ctx->comp_methods); m++) {
            comp = sk_SSL_COMP_value(s->ctx->comp_methods, m);
            if (comp_id == comp->id) {
                s->s3->tmp.new_compression = comp;
                break;
            }
        }
        if (s->s3->tmp.new_compression == NULL) {
            SSLerr(SSL_F_SSL3_GET_CLIENT_HELLO,
                   SSL_R_INVALID_COMPRESSION_ALGORITHM);
            goto f_err;
        }
         
        for (m = 0; m < i; m++) {
            if (q[m] == comp_id)
                break;
        }
        if (m >= i) {
            al = SSL_AD_ILLEGAL_PARAMETER;
            SSLerr(SSL_F_SSL3_GET_CLIENT_HELLO,
                   SSL_R_REQUIRED_COMPRESSSION_ALGORITHM_MISSING);
            goto f_err;
        }
    } else if (s->hit)
        comp = NULL;
    else if (!(s->options & SSL_OP_NO_COMPRESSION) && s->ctx->comp_methods) {
         
        int m, nn, o, v, done = 0;

        nn = sk_SSL_COMP_num(s->ctx->comp_methods);
        for (m = 0; m < nn; m++) {
            comp = sk_SSL_COMP_value(s->ctx->comp_methods, m);
            v = comp->id;
            for (o = 0; o < i; o++) {
                if (v == q[o]) {
                    done = 1;
                    break;
                }
            }
            if (done)
                break;
        }
        if (done)
            s->s3->tmp.new_compression = comp;
        else
            comp = NULL;
    }
#else
     
    if (s->session->compress_meth != 0) {
        SSLerr(SSL_F_SSL3_GET_CLIENT_HELLO, SSL_R_INCONSISTENT_COMPRESSION);
        goto f_err;
    }
#endif

     

    if (!s->hit) {
#ifdef OPENSSL_NO_COMP
        s->session->compress_meth = 0;
#else
        s->session->compress_meth = (comp == NULL) ? 0 : comp->id;
#endif
        if (s->session->ciphers != NULL)
            sk_SSL_CIPHER_free(s->session->ciphers);
        s->session->ciphers = ciphers;
        if (ciphers == NULL) {
            al = SSL_AD_INTERNAL_ERROR;
            SSLerr(SSL_F_SSL3_GET_CLIENT_HELLO, ERR_R_INTERNAL_ERROR);
            goto f_err;
        }
        ciphers = NULL;
        if (!tls1_set_server_sigalgs(s)) {
            SSLerr(SSL_F_SSL3_GET_CLIENT_HELLO, SSL_R_CLIENTHELLO_TLSEXT);
            goto err;
        }
         
 retry_cert:
        if (s->cert->cert_cb) {
            int rv = s->cert->cert_cb(s, s->cert->cert_cb_arg);
            if (rv == 0) {
                al = SSL_AD_INTERNAL_ERROR;
                SSLerr(SSL_F_SSL3_GET_CLIENT_HELLO, SSL_R_CERT_CB_ERROR);
                goto f_err;
            }
            if (rv < 0) {
                s->rwstate = SSL_X509_LOOKUP;
                return -1;
            }
            s->rwstate = SSL_NOTHING;
        }
        c = ssl3_choose_cipher(s, s->session->ciphers, SSL_get_ciphers(s));

        if (c == NULL) {
            al = SSL_AD_HANDSHAKE_FAILURE;
            SSLerr(SSL_F_SSL3_GET_CLIENT_HELLO, SSL_R_NO_SHARED_CIPHER);
            goto f_err;
        }
        s->s3->tmp.new_cipher = c;
    } else {
         
#ifdef REUSE_CIPHER_BUG
        STACK_OF(SSL_CIPHER) *sk;
        SSL_CIPHER *nc = NULL;
        SSL_CIPHER *ec = NULL;

        if (s->options & SSL_OP_NETSCAPE_DEMO_CIPHER_CHANGE_BUG) {
            sk = s->session->ciphers;
            for (i = 0; i < sk_SSL_CIPHER_num(sk); i++) {
                c = sk_SSL_CIPHER_value(sk, i);
                if (c->algorithm_enc & SSL_eNULL)
                    nc = c;
                if (SSL_C_IS_EXPORT(c))
                    ec = c;
            }
            if (nc != NULL)
                s->s3->tmp.new_cipher = nc;
            else if (ec != NULL)
                s->s3->tmp.new_cipher = ec;
            else
                s->s3->tmp.new_cipher = s->session->cipher;
        } else
#endif
            s->s3->tmp.new_cipher = s->session->cipher;
    }

    if (!SSL_USE_SIGALGS(s) || !(s->verify_mode & SSL_VERIFY_PEER)) {
        if (!ssl3_digest_cached_records(s))
            goto f_err;
    }

     

     
    if (s->version >= SSL3_VERSION) {
        if (ssl_check_clienthello_tlsext_late(s) <= 0) {
            SSLerr(SSL_F_SSL3_GET_CLIENT_HELLO, SSL_R_CLIENTHELLO_TLSEXT);
            goto err;
        }
    }

    ret = cookie_valid ? 2 : 1;
    if (0) {
 f_err:
        ssl3_send_alert(s, SSL3_AL_FATAL, al);
 err:
        s->state = SSL_ST_ERR;
    }

    if (ciphers != NULL)
        sk_SSL_CIPHER_free(ciphers);
    return ret;
}
