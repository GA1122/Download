int SSL_is_init_finished(SSL *s)
{
    return !(s->statem.in_init) && (s->statem.hand_state == TLS_ST_OK);
}
