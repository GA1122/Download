WORK_STATE ossl_statem_client_pre_work(SSL *s, WORK_STATE wst)
{
    OSSL_STATEM *st = &s->statem;

    switch (st->hand_state) {
    case TLS_ST_CW_CLNT_HELLO:
        s->shutdown = 0;
        if (SSL_IS_DTLS(s)) {
             
            if (!ssl3_init_finished_mac(s)) {
                ossl_statem_set_error(s);
                return WORK_ERROR;
            }
        }
        break;

    case TLS_ST_CW_CHANGE:
        if (SSL_IS_DTLS(s)) {
            if (s->hit) {
                 
                st->use_timer = 0;
            }
#ifndef OPENSSL_NO_SCTP
            if (BIO_dgram_is_sctp(SSL_get_wbio(s)))
                return dtls_wait_for_dry(s);
#endif
        }
        return WORK_FINISHED_CONTINUE;

    case TLS_ST_OK:
        return tls_finish_handshake(s, wst);

    default:
         
        break;
    }

    return WORK_FINISHED_CONTINUE;
}