void ssl_callback_DelSessionCacheEntry(SSL_CTX *ctx,
                                       SSL_SESSION *session)
{
    server_rec *s;
    SSLSrvConfigRec *sc;
    IDCONST unsigned char *id;
    unsigned int idlen;

     
    if (!(s = (server_rec *)SSL_CTX_get_app_data(ctx))) {
        return;  
    }

    sc = mySrvConfig(s);

     
#ifdef OPENSSL_NO_SSL_INTERN
    id = (unsigned char *)SSL_SESSION_get_id(session, &idlen);
#else
    id = session->session_id;
    idlen = session->session_id_length;
#endif

     
    ssl_scache_remove(s, id, idlen, sc->mc->pPool);

    ssl_session_log(s, "REM", id, idlen,
                    "OK", "dead", 0);

    return;
}
