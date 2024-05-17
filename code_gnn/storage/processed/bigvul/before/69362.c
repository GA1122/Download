WORK_STATE ossl_statem_client_post_work(SSL *s, WORK_STATE wst)
{
    OSSL_STATEM *st = &s->statem;

    s->init_num = 0;

    switch (st->hand_state) {
    case TLS_ST_CW_CLNT_HELLO:
        if (wst == WORK_MORE_A && statem_flush(s) != 1)
            return WORK_MORE_A;

        if (SSL_IS_DTLS(s)) {
             
            s->first_packet = 1;
        }
        break;

    case TLS_ST_CW_KEY_EXCH:
        if (tls_client_key_exchange_post_work(s) == 0)
            return WORK_ERROR;
        break;

    case TLS_ST_CW_CHANGE:
        s->session->cipher = s->s3->tmp.new_cipher;
#ifdef OPENSSL_NO_COMP
        s->session->compress_meth = 0;
#else
        if (s->s3->tmp.new_compression == NULL)
            s->session->compress_meth = 0;
        else
            s->session->compress_meth = s->s3->tmp.new_compression->id;
#endif
        if (!s->method->ssl3_enc->setup_key_block(s))
            return WORK_ERROR;

        if (!s->method->ssl3_enc->change_cipher_state(s,
                                                      SSL3_CHANGE_CIPHER_CLIENT_WRITE))
            return WORK_ERROR;

        if (SSL_IS_DTLS(s)) {
#ifndef OPENSSL_NO_SCTP
            if (s->hit) {
                 
                BIO_ctrl(SSL_get_wbio(s), BIO_CTRL_DGRAM_SCTP_NEXT_AUTH_KEY,
                         0, NULL);
            }
#endif

            dtls1_reset_seq_numbers(s, SSL3_CC_WRITE);
        }
        break;

    case TLS_ST_CW_FINISHED:
#ifndef OPENSSL_NO_SCTP
        if (wst == WORK_MORE_A && SSL_IS_DTLS(s) && s->hit == 0) {
             
            BIO_ctrl(SSL_get_wbio(s), BIO_CTRL_DGRAM_SCTP_NEXT_AUTH_KEY,
                     0, NULL);
        }
#endif
        if (statem_flush(s) != 1)
            return WORK_MORE_B;
        break;

    default:
         
        break;
    }

    return WORK_FINISHED_CONTINUE;
}