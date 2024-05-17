void ossl_statem_clear(SSL *s)
{
    s->statem.state = MSG_FLOW_UNINITED;
    s->statem.hand_state = TLS_ST_BEFORE;
    s->statem.in_init = 1;
    s->statem.no_cert_verify = 0;
}
