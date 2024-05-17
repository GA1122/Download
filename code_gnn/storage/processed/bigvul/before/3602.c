int ssl3_send_server_hello(SSL *s)
{
    unsigned char *buf;
    unsigned char *p, *d;
    int i, sl;
    int al = 0;
    unsigned long l;

    if (s->state == SSL3_ST_SW_SRVR_HELLO_A) {
        buf = (unsigned char *)s->init_buf->data;
#ifdef OPENSSL_NO_TLSEXT
        p = s->s3->server_random;
        if (ssl_fill_hello_random(s, 1, p, SSL3_RANDOM_SIZE) <= 0) {
            s->state = SSL_ST_ERR;
            return -1;
        }
#endif
         
        d = p = ssl_handshake_start(s);

        *(p++) = s->version >> 8;
        *(p++) = s->version & 0xff;

         
        memcpy(p, s->s3->server_random, SSL3_RANDOM_SIZE);
        p += SSL3_RANDOM_SIZE;

         
        if (!(s->ctx->session_cache_mode & SSL_SESS_CACHE_SERVER)
            && !s->hit)
            s->session->session_id_length = 0;

        sl = s->session->session_id_length;
        if (sl > (int)sizeof(s->session->session_id)) {
            SSLerr(SSL_F_SSL3_SEND_SERVER_HELLO, ERR_R_INTERNAL_ERROR);
            s->state = SSL_ST_ERR;
            return -1;
        }
        *(p++) = sl;
        memcpy(p, s->session->session_id, sl);
        p += sl;

         
        i = ssl3_put_cipher_by_char(s->s3->tmp.new_cipher, p);
        p += i;

         
#ifdef OPENSSL_NO_COMP
        *(p++) = 0;
#else
        if (s->s3->tmp.new_compression == NULL)
            *(p++) = 0;
        else
            *(p++) = s->s3->tmp.new_compression->id;
#endif
#ifndef OPENSSL_NO_TLSEXT
        if (ssl_prepare_serverhello_tlsext(s) <= 0) {
            SSLerr(SSL_F_SSL3_SEND_SERVER_HELLO, SSL_R_SERVERHELLO_TLSEXT);
            s->state = SSL_ST_ERR;
            return -1;
        }
        if ((p =
             ssl_add_serverhello_tlsext(s, p, buf + SSL3_RT_MAX_PLAIN_LENGTH,
                                        &al)) == NULL) {
            ssl3_send_alert(s, SSL3_AL_FATAL, al);
            SSLerr(SSL_F_SSL3_SEND_SERVER_HELLO, ERR_R_INTERNAL_ERROR);
            s->state = SSL_ST_ERR;
            return -1;
        }
#endif
         
        l = (p - d);
        ssl_set_handshake_header(s, SSL3_MT_SERVER_HELLO, l);
        s->state = SSL3_ST_SW_SRVR_HELLO_B;
    }

     
    return ssl_do_write(s);
}
