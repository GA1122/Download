void ossl_statem_set_sctp_read_sock(SSL *s, int read_sock)
{
    s->statem.in_sctp_read_sock = read_sock;
}
