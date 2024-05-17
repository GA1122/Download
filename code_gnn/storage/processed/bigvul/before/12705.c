int dtls1_heartbeat(SSL *s)
{
    unsigned char *buf, *p;
    int ret = -1;
    unsigned int payload = 18;   
    unsigned int padding = 16;   
    unsigned int size;

     
    if (!(s->tlsext_heartbeat & SSL_DTLSEXT_HB_ENABLED) ||
        s->tlsext_heartbeat & SSL_DTLSEXT_HB_DONT_SEND_REQUESTS) {
        SSLerr(SSL_F_DTLS1_HEARTBEAT, SSL_R_TLS_HEARTBEAT_PEER_DOESNT_ACCEPT);
        return -1;
    }

     
    if (s->tlsext_hb_pending) {
        SSLerr(SSL_F_DTLS1_HEARTBEAT, SSL_R_TLS_HEARTBEAT_PENDING);
        return -1;
    }

     
    if (SSL_in_init(s) || ossl_statem_get_in_handshake(s)) {
        SSLerr(SSL_F_DTLS1_HEARTBEAT, SSL_R_UNEXPECTED_MESSAGE);
        return -1;
    }

     
    size = HEARTBEAT_SIZE(payload, padding);
    buf = OPENSSL_malloc(size);
    if (buf == NULL) {
        SSLerr(SSL_F_DTLS1_HEARTBEAT, ERR_R_MALLOC_FAILURE);
        return -1;
    }
    p = buf;
     
    *p++ = TLS1_HB_REQUEST;
     
    s2n(payload, p);
     
    s2n(s->tlsext_hb_seq, p);
     
    if (RAND_bytes(p, 16) <= 0) {
        SSLerr(SSL_F_DTLS1_HEARTBEAT, ERR_R_INTERNAL_ERROR);
        goto err;
    }
    p += 16;
     
    if (RAND_bytes(p, padding) <= 0) {
        SSLerr(SSL_F_DTLS1_HEARTBEAT, ERR_R_INTERNAL_ERROR);
        goto err;
    }

    ret = dtls1_write_bytes(s, DTLS1_RT_HEARTBEAT, buf, size);
    if (ret >= 0) {
        if (s->msg_callback)
            s->msg_callback(1, s->version, DTLS1_RT_HEARTBEAT,
                            buf, size, s, s->msg_callback_arg);

        dtls1_start_timer(s);
        s->tlsext_hb_pending = 1;
    }

 err:
    OPENSSL_free(buf);

    return ret;
}
