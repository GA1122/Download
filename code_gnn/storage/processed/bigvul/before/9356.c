int ossl_statem_get_in_handshake(SSL *s)
{
    return s->statem.in_handshake;
}
