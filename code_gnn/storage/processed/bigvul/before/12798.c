SSL_SESSION *SSL_get1_session(SSL *ssl)
 
{
    SSL_SESSION *sess;
     
    CRYPTO_w_lock(CRYPTO_LOCK_SSL_SESSION);
    sess = ssl->session;
    if (sess)
        sess->references++;
    CRYPTO_w_unlock(CRYPTO_LOCK_SSL_SESSION);
    return (sess);
}
