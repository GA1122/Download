void ossl_statem_set_renegotiate(SSL *s)
{
    s->statem.state = MSG_FLOW_RENEGOTIATE;
    s->statem.in_init = 1;
}
