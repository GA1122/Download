static int state_machine(SSL *s, int server)
{
    BUF_MEM *buf = NULL;
    unsigned long Time = (unsigned long)time(NULL);
    void (*cb) (const SSL *ssl, int type, int val) = NULL;
    OSSL_STATEM *st = &s->statem;
    int ret = -1;
    int ssret;

    if (st->state == MSG_FLOW_ERROR) {
         
        return -1;
    }

    RAND_add(&Time, sizeof(Time), 0);
    ERR_clear_error();
    clear_sys_error();

    cb = get_callback(s);

    st->in_handshake++;
    if (!SSL_in_init(s) || SSL_in_before(s)) {
        if (!SSL_clear(s))
            return -1;
    }
#ifndef OPENSSL_NO_SCTP
    if (SSL_IS_DTLS(s)) {
         
        BIO_ctrl(SSL_get_wbio(s), BIO_CTRL_DGRAM_SCTP_SET_IN_HANDSHAKE,
                 st->in_handshake, NULL);
    }
#endif

#ifndef OPENSSL_NO_HEARTBEATS
     
    if (s->tlsext_hb_pending) {
        if (SSL_IS_DTLS(s))
            dtls1_stop_timer(s);
        s->tlsext_hb_pending = 0;
        s->tlsext_hb_seq++;
    }
#endif

     

    if (st->state == MSG_FLOW_RENEGOTIATE) {
        s->renegotiate = 1;
        if (!server)
            s->ctx->stats.sess_connect_renegotiate++;
    }

    if (st->state == MSG_FLOW_UNINITED || st->state == MSG_FLOW_RENEGOTIATE) {
        if (st->state == MSG_FLOW_UNINITED) {
            st->hand_state = TLS_ST_BEFORE;
        }

        s->server = server;
        if (cb != NULL)
            cb(s, SSL_CB_HANDSHAKE_START, 1);

        if (SSL_IS_DTLS(s)) {
            if ((s->version & 0xff00) != (DTLS1_VERSION & 0xff00) &&
                (server || (s->version & 0xff00) != (DTLS1_BAD_VER & 0xff00))) {
                SSLerr(SSL_F_STATE_MACHINE, ERR_R_INTERNAL_ERROR);
                goto end;
            }
        } else {
            if ((s->version >> 8) != SSL3_VERSION_MAJOR) {
                SSLerr(SSL_F_STATE_MACHINE, ERR_R_INTERNAL_ERROR);
                goto end;
            }
        }

        if (!ssl_security(s, SSL_SECOP_VERSION, 0, s->version, NULL)) {
            SSLerr(SSL_F_STATE_MACHINE, SSL_R_VERSION_TOO_LOW);
            goto end;
        }

        if (s->init_buf == NULL) {
            if ((buf = BUF_MEM_new()) == NULL) {
                goto end;
            }
            if (!BUF_MEM_grow(buf, SSL3_RT_MAX_PLAIN_LENGTH)) {
                goto end;
            }
            s->init_buf = buf;
            buf = NULL;
        }

        if (!ssl3_setup_buffers(s)) {
            goto end;
        }
        s->init_num = 0;

         
        s->s3->change_cipher_spec = 0;

         
#ifndef OPENSSL_NO_SCTP
        if (!SSL_IS_DTLS(s) || !BIO_dgram_is_sctp(SSL_get_wbio(s)))
#endif
            if (!ssl_init_wbio_buffer(s)) {
                goto end;
            }

        if (!server || st->state != MSG_FLOW_RENEGOTIATE) {
            if (!ssl3_init_finished_mac(s)) {
                ossl_statem_set_error(s);
                goto end;
            }
        }

        if (server) {
            if (st->state != MSG_FLOW_RENEGOTIATE) {
                s->ctx->stats.sess_accept++;
            } else if (!s->s3->send_connection_binding &&
                       !(s->options &
                         SSL_OP_ALLOW_UNSAFE_LEGACY_RENEGOTIATION)) {
                 
                SSLerr(SSL_F_STATE_MACHINE,
                       SSL_R_UNSAFE_LEGACY_RENEGOTIATION_DISABLED);
                ssl3_send_alert(s, SSL3_AL_FATAL, SSL_AD_HANDSHAKE_FAILURE);
                ossl_statem_set_error(s);
                goto end;
            } else {
                 
                s->ctx->stats.sess_accept_renegotiate++;
            }
        } else {
            s->ctx->stats.sess_connect++;

             
            memset(s->s3->client_random, 0, sizeof(s->s3->client_random));
            s->hit = 0;

            s->s3->tmp.cert_request = 0;

            if (SSL_IS_DTLS(s)) {
                st->use_timer = 1;
            }
        }

        st->state = MSG_FLOW_WRITING;
        init_write_state_machine(s);
        st->read_state_first_init = 1;
    }

    while (st->state != MSG_FLOW_FINISHED) {
        if (st->state == MSG_FLOW_READING) {
            ssret = read_state_machine(s);
            if (ssret == SUB_STATE_FINISHED) {
                st->state = MSG_FLOW_WRITING;
                init_write_state_machine(s);
            } else {
                 
                goto end;
            }
        } else if (st->state == MSG_FLOW_WRITING) {
            ssret = write_state_machine(s);
            if (ssret == SUB_STATE_FINISHED) {
                st->state = MSG_FLOW_READING;
                init_read_state_machine(s);
            } else if (ssret == SUB_STATE_END_HANDSHAKE) {
                st->state = MSG_FLOW_FINISHED;
            } else {
                 
                goto end;
            }
        } else {
             
            ossl_statem_set_error(s);
            goto end;
        }
    }

    st->state = MSG_FLOW_UNINITED;
    ret = 1;

 end:
    st->in_handshake--;

#ifndef OPENSSL_NO_SCTP
    if (SSL_IS_DTLS(s)) {
         
        BIO_ctrl(SSL_get_wbio(s), BIO_CTRL_DGRAM_SCTP_SET_IN_HANDSHAKE,
                 st->in_handshake, NULL);
    }
#endif

    BUF_MEM_free(buf);
    if (cb != NULL) {
        if (server)
            cb(s, SSL_CB_ACCEPT_EXIT, ret);
        else
            cb(s, SSL_CB_CONNECT_EXIT, ret);
    }
    return ret;
}
