int ssl_callback_NewSessionCacheEntry(SSL *ssl, SSL_SESSION *session)
{
     
    conn_rec *conn      = (conn_rec *)SSL_get_app_data(ssl);
    server_rec *s       = mySrvFromConn(conn);
    SSLSrvConfigRec *sc = mySrvConfig(s);
    long timeout        = sc->session_cache_timeout;
    BOOL rc;
    IDCONST unsigned char *id;
    unsigned int idlen;

     
    SSL_set_timeout(session, timeout);

     
#ifdef OPENSSL_NO_SSL_INTERN
    id = (unsigned char *)SSL_SESSION_get_id(session, &idlen);
#else
    id = session->session_id;
    idlen = session->session_id_length;
#endif

    rc = ssl_scache_store(s, id, idlen,
                          apr_time_from_sec(SSL_SESSION_get_time(session)
                                          + timeout),
                          session, conn->pool);

    ssl_session_log(s, "SET", id, idlen,
                    rc == TRUE ? "OK" : "BAD",
                    "caching", timeout);

     
    return 0;
}
