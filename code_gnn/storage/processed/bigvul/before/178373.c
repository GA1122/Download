 int ssl_get_prev_session(SSL *s, unsigned char *session_id, int len,
                          const unsigned char *limit)
 {
      
 
     SSL_SESSION *ret = NULL;
     int fatal = 0;
     int try_session_cache = 1;
 #ifndef OPENSSL_NO_TLSEXT
      int r;
  #endif
  
    if (session_id + len > limit) {
//     if (limit - session_id < len) {
          fatal = 1;
          goto err;
      }
 
     if (len == 0)
         try_session_cache = 0;
 
 #ifndef OPENSSL_NO_TLSEXT
      
     r = tls1_process_ticket(s, session_id, len, limit, &ret);
     switch (r) {
     case -1:                    
         fatal = 1;
         goto err;
     case 0:                     
     case 1:                     
         break;                   
     case 2:                     
     case 3:                     
         try_session_cache = 0;
         break;
     default:
         abort();
     }
 #endif
 
     if (try_session_cache &&
         ret == NULL &&
         !(s->session_ctx->session_cache_mode &
           SSL_SESS_CACHE_NO_INTERNAL_LOOKUP)) {
         SSL_SESSION data;
         data.ssl_version = s->version;
         data.session_id_length = len;
         if (len == 0)
             return 0;
         memcpy(data.session_id, session_id, len);
         CRYPTO_r_lock(CRYPTO_LOCK_SSL_CTX);
         ret = lh_SSL_SESSION_retrieve(s->session_ctx->sessions, &data);
         if (ret != NULL) {
              
             CRYPTO_add(&ret->references, 1, CRYPTO_LOCK_SSL_SESSION);
         }
         CRYPTO_r_unlock(CRYPTO_LOCK_SSL_CTX);
         if (ret == NULL)
             s->session_ctx->stats.sess_miss++;
     }
 
     if (try_session_cache &&
         ret == NULL && s->session_ctx->get_session_cb != NULL) {
         int copy = 1;
 
         if ((ret = s->session_ctx->get_session_cb(s, session_id, len, &copy))) {
             s->session_ctx->stats.sess_cb_hit++;
 
              
             if (copy)
                 CRYPTO_add(&ret->references, 1, CRYPTO_LOCK_SSL_SESSION);
 
              
             if (!
                 (s->session_ctx->session_cache_mode &
                  SSL_SESS_CACHE_NO_INTERNAL_STORE))
                  
                 SSL_CTX_add_session(s->session_ctx, ret);
         }
     }
 
     if (ret == NULL)
         goto err;
 
      
 
     if (ret->sid_ctx_length != s->sid_ctx_length
         || memcmp(ret->sid_ctx, s->sid_ctx, ret->sid_ctx_length)) {
          
         goto err;                
     }
 
     if ((s->verify_mode & SSL_VERIFY_PEER) && s->sid_ctx_length == 0) {
          
 
         SSLerr(SSL_F_SSL_GET_PREV_SESSION,
                SSL_R_SESSION_ID_CONTEXT_UNINITIALIZED);
         fatal = 1;
         goto err;
     }
 
     if (ret->cipher == NULL) {
         unsigned char buf[5], *p;
         unsigned long l;
 
         p = buf;
         l = ret->cipher_id;
         l2n(l, p);
         if ((ret->ssl_version >> 8) >= SSL3_VERSION_MAJOR)
             ret->cipher = ssl_get_cipher_by_char(s, &(buf[2]));
         else
             ret->cipher = ssl_get_cipher_by_char(s, &(buf[1]));
         if (ret->cipher == NULL)
             goto err;
     }
 
     if (ret->timeout < (long)(time(NULL) - ret->time)) {  
         s->session_ctx->stats.sess_timeout++;
         if (try_session_cache) {
              
             SSL_CTX_remove_session(s->session_ctx, ret);
         }
         goto err;
     }
 
     s->session_ctx->stats.sess_hit++;
 
     if (s->session != NULL)
         SSL_SESSION_free(s->session);
     s->session = ret;
     s->verify_result = s->session->verify_result;
     return 1;
 
  err:
     if (ret != NULL) {
         SSL_SESSION_free(ret);
 #ifndef OPENSSL_NO_TLSEXT
         if (!try_session_cache) {
              
             s->tlsext_ticket_expected = 1;
         }
 #endif
     }
     if (fatal)
         return -1;
     else
         return 0;
 }