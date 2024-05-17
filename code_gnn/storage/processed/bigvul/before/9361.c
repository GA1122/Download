void ossl_statem_set_in_handshake(SSL *s, int inhand)
{
    if (inhand)
        s->statem.in_handshake++;
    else
        s->statem.in_handshake--;
}
