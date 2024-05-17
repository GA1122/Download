static int statem_do_write(SSL *s)
{
    OSSL_STATEM *st = &s->statem;

    if (st->hand_state == TLS_ST_CW_CHANGE
        || st->hand_state == TLS_ST_SW_CHANGE) {
        if (SSL_IS_DTLS(s))
            return dtls1_do_write(s, SSL3_RT_CHANGE_CIPHER_SPEC);
        else
            return ssl3_do_write(s, SSL3_RT_CHANGE_CIPHER_SPEC);
    } else {
        return ssl_do_write(s);
    }
}
