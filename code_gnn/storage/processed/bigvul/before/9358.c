int ossl_statem_in_sctp_read_sock(SSL *s)
{
    return s->statem.in_sctp_read_sock;
}
