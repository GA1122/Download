void ossl_statem_set_hello_verify_done(SSL *s)
{
    s->statem.state = MSG_FLOW_UNINITED;
    s->statem.in_init = 1;
     
    s->statem.hand_state = TLS_ST_SR_CLNT_HELLO;
}
