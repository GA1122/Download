SSL_SESSION *SSL_get_session(const SSL *ssl)
 
{
    return (ssl->session);
}
