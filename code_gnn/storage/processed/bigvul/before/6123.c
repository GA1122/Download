int ssl2_accept(SSL *s)
{
    unsigned long l = (unsigned long)time(NULL);
    BUF_MEM *buf = NULL;
    int ret = -1;
    long num1;
    void (*cb) (const SSL *ssl, int type, int val) = NULL;
    int new_state, state;

    RAND_add(&l, sizeof(l), 0);
    ERR_clear_error();
    clear_sys_error();

    if (s->info_callback != NULL)
        cb = s->info_callback;
    else if (s->ctx->info_callback != NULL)
        cb = s->ctx->info_callback;

     
    s->in_handshake++;
    if (!SSL_in_init(s) || SSL_in_before(s))
        SSL_clear(s);

    if (s->cert == NULL) {
        SSLerr(SSL_F_SSL2_ACCEPT, SSL_R_NO_CERTIFICATE_SET);
        return (-1);
    }

    clear_sys_error();
    for (;;) {
        state = s->state;

        switch (s->state) {
        case SSL_ST_BEFORE:
        case SSL_ST_ACCEPT:
        case SSL_ST_BEFORE | SSL_ST_ACCEPT:
        case SSL_ST_OK | SSL_ST_ACCEPT:

            s->server = 1;
            if (cb != NULL)
                cb(s, SSL_CB_HANDSHAKE_START, 1);

            s->version = SSL2_VERSION;
            s->type = SSL_ST_ACCEPT;

            if (s->init_buf == NULL) {
                if ((buf = BUF_MEM_new()) == NULL) {
                    ret = -1;
                    goto end;
                }
                if (!BUF_MEM_grow
                    (buf, (int)SSL2_MAX_RECORD_LENGTH_3_BYTE_HEADER)) {
                    BUF_MEM_free(buf);
                    ret = -1;
                    goto end;
                }
                s->init_buf = buf;
            }
            s->init_num = 0;
            s->ctx->stats.sess_accept++;
            s->handshake_func = ssl2_accept;
            s->state = SSL2_ST_GET_CLIENT_HELLO_A;
            BREAK;

        case SSL2_ST_GET_CLIENT_HELLO_A:
        case SSL2_ST_GET_CLIENT_HELLO_B:
        case SSL2_ST_GET_CLIENT_HELLO_C:
            s->shutdown = 0;
            ret = get_client_hello(s);
            if (ret <= 0)
                goto end;
            s->init_num = 0;
            s->state = SSL2_ST_SEND_SERVER_HELLO_A;
            BREAK;

        case SSL2_ST_SEND_SERVER_HELLO_A:
        case SSL2_ST_SEND_SERVER_HELLO_B:
            ret = server_hello(s);
            if (ret <= 0)
                goto end;
            s->init_num = 0;
            if (!s->hit) {
                s->state = SSL2_ST_GET_CLIENT_MASTER_KEY_A;
                BREAK;
            } else {
                s->state = SSL2_ST_SERVER_START_ENCRYPTION;
                BREAK;
            }
        case SSL2_ST_GET_CLIENT_MASTER_KEY_A:
        case SSL2_ST_GET_CLIENT_MASTER_KEY_B:
            ret = get_client_master_key(s);
            if (ret <= 0)
                goto end;
            s->init_num = 0;
            s->state = SSL2_ST_SERVER_START_ENCRYPTION;
            BREAK;

        case SSL2_ST_SERVER_START_ENCRYPTION:
             
            if (!ssl2_enc_init(s, 0)) {
                ret = -1;
                goto end;
            }
            s->s2->clear_text = 0;
            s->state = SSL2_ST_SEND_SERVER_VERIFY_A;
            BREAK;

        case SSL2_ST_SEND_SERVER_VERIFY_A:
        case SSL2_ST_SEND_SERVER_VERIFY_B:
            ret = server_verify(s);
            if (ret <= 0)
                goto end;
            s->init_num = 0;
            if (s->hit) {
                 
                s->state = SSL2_ST_SEND_SERVER_VERIFY_C;
                BREAK;
            } else {
                s->state = SSL2_ST_GET_CLIENT_FINISHED_A;
                break;
            }

        case SSL2_ST_SEND_SERVER_VERIFY_C:
             
            num1 = BIO_ctrl(s->wbio, BIO_CTRL_INFO, 0, NULL);
            if (num1 > 0) {
                s->rwstate = SSL_WRITING;
                num1 = BIO_flush(s->wbio);
                if (num1 <= 0) {
                    ret = -1;
                    goto end;
                }
                s->rwstate = SSL_NOTHING;
            }

             
            s->wbio = BIO_pop(s->wbio);

            s->state = SSL2_ST_GET_CLIENT_FINISHED_A;
            BREAK;

        case SSL2_ST_GET_CLIENT_FINISHED_A:
        case SSL2_ST_GET_CLIENT_FINISHED_B:
            ret = get_client_finished(s);
            if (ret <= 0)
                goto end;
            s->init_num = 0;
            s->state = SSL2_ST_SEND_REQUEST_CERTIFICATE_A;
            BREAK;

        case SSL2_ST_SEND_REQUEST_CERTIFICATE_A:
        case SSL2_ST_SEND_REQUEST_CERTIFICATE_B:
        case SSL2_ST_SEND_REQUEST_CERTIFICATE_C:
        case SSL2_ST_SEND_REQUEST_CERTIFICATE_D:
             
            if (!(s->verify_mode & SSL_VERIFY_PEER) ||
                ((s->session->peer != NULL) &&
                 (s->verify_mode & SSL_VERIFY_CLIENT_ONCE))) {
                s->state = SSL2_ST_SEND_SERVER_FINISHED_A;
                break;
            } else {
                ret = request_certificate(s);
                if (ret <= 0)
                    goto end;
                s->init_num = 0;
                s->state = SSL2_ST_SEND_SERVER_FINISHED_A;
            }
            BREAK;

        case SSL2_ST_SEND_SERVER_FINISHED_A:
        case SSL2_ST_SEND_SERVER_FINISHED_B:
            ret = server_finish(s);
            if (ret <= 0)
                goto end;
            s->init_num = 0;
            s->state = SSL_ST_OK;
            break;

        case SSL_ST_OK:
            BUF_MEM_free(s->init_buf);
            ssl_free_wbio_buffer(s);
            s->init_buf = NULL;
            s->init_num = 0;
             

            ssl_update_cache(s, SSL_SESS_CACHE_SERVER);

            s->ctx->stats.sess_accept_good++;
             
            ret = 1;

            if (cb != NULL)
                cb(s, SSL_CB_HANDSHAKE_DONE, 1);

            goto end;
             

        default:
            SSLerr(SSL_F_SSL2_ACCEPT, SSL_R_UNKNOWN_STATE);
            ret = -1;
            goto end;
             
        }

        if ((cb != NULL) && (s->state != state)) {
            new_state = s->state;
            s->state = state;
            cb(s, SSL_CB_ACCEPT_LOOP, 1);
            s->state = new_state;
        }
    }
 end:
    s->in_handshake--;
    if (cb != NULL)
        cb(s, SSL_CB_ACCEPT_EXIT, ret);
    return (ret);
}
