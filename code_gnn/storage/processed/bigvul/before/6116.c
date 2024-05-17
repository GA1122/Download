int ssl2_shutdown(SSL *s)
{
    s->shutdown = (SSL_SENT_SHUTDOWN | SSL_RECEIVED_SHUTDOWN);
    return (1);
}
