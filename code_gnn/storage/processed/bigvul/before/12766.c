int SSL_CTX_add_session(SSL_CTX *ctx, SSL_SESSION *c)
{
    int ret = 0;
    SSL_SESSION *s;

     
    CRYPTO_add(&c->references, 1, CRYPTO_LOCK_SSL_SESSION);
     

    CRYPTO_w_lock(CRYPTO_LOCK_SSL_CTX);
    s = lh_SSL_SESSION_insert(ctx->sessions, c);

     
    if (s != NULL && s != c) {
         
        SSL_SESSION_list_remove(ctx, s);
        SSL_SESSION_free(s);
         
        s = NULL;
    }

     
    if (s == NULL)
        SSL_SESSION_list_add(ctx, c);

    if (s != NULL) {
         

        SSL_SESSION_free(s);     
        ret = 0;
    } else {
         

        ret = 1;

        if (SSL_CTX_sess_get_cache_size(ctx) > 0) {
            while (SSL_CTX_sess_number(ctx) >
                   SSL_CTX_sess_get_cache_size(ctx)) {
                if (!remove_session_lock(ctx, ctx->session_cache_tail, 0))
                    break;
                else
                    ctx->stats.sess_cache_full++;
            }
        }
    }
    CRYPTO_w_unlock(CRYPTO_LOCK_SSL_CTX);
    return (ret);
}
