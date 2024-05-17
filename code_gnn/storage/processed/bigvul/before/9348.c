int SSL_in_before(SSL *s)
{
     
    return (s->statem.hand_state == TLS_ST_BEFORE)
        && (s->statem.state == MSG_FLOW_UNINITED);
}
