SSL_SESSION *ssl_callback_GetSessionCacheEntry(SSL *ssl,
                                               IDCONST unsigned char *id,
                                               int idlen, int *do_copy)
{
     
    conn_rec *conn = (conn_rec *)SSL_get_app_data(ssl);
    server_rec *s  = mySrvFromConn(conn);
    SSL_SESSION *session;

     
    session = ssl_scache_retrieve(s, id, idlen, conn->pool);

    ssl_session_log(s, "GET", id, idlen,
                    session ? "FOUND" : "MISSED",
                    session ? "reuse" : "renewal", 0);

     
    *do_copy = 0;

    return session;
}
