 int ssl3_client_hello(SSL *s)
 {
     unsigned char *buf;
     unsigned char *p, *d;
     int i;
     unsigned long l;
     int al = 0;
 #ifndef OPENSSL_NO_COMP
     int j;
     SSL_COMP *comp;
 #endif
 
     buf = (unsigned char *)s->init_buf->data;
     if (s->state == SSL3_ST_CW_CLNT_HELLO_A) {
         SSL_SESSION *sess = s->session;
         if ((sess == NULL) ||
             (sess->ssl_version != s->version) ||
             !sess->session_id_length || (sess->not_resumable)) {
             if (!ssl_get_new_session(s, 0))
                 goto err;
         }
         if (s->method->version == DTLS_ANY_VERSION) {
              
             int options = s->options;
              
             if (options & SSL_OP_NO_DTLSv1_2) {
                 if (tls1_suiteb(s)) {
                     SSLerr(SSL_F_SSL3_CLIENT_HELLO,
                            SSL_R_ONLY_DTLS_1_2_ALLOWED_IN_SUITEB_MODE);
                     goto err;
                 }
                  
                 if (options & SSL_OP_NO_DTLSv1) {
                     SSLerr(SSL_F_SSL3_CLIENT_HELLO, SSL_R_WRONG_SSL_VERSION);
                     goto err;
                 }
                  
                 s->method = DTLSv1_client_method();
                 s->version = DTLS1_VERSION;
             } else {
                  
                 if (options & SSL_OP_NO_DTLSv1)
                     s->method = DTLSv1_2_client_method();
                 s->version = DTLS1_2_VERSION;
             }
             s->client_version = s->version;
         }
          
 
         p = s->s3->client_random;
 
          
         if (SSL_IS_DTLS(s)) {
             size_t idx;
             i = 1;
             for (idx = 0; idx < sizeof(s->s3->client_random); idx++) {
                 if (p[idx]) {
                     i = 0;
                     break;
                 }
             }
          } else
              i = 1;
  
        if (i)
            ssl_fill_hello_random(s, 0, p, sizeof(s->s3->client_random));
//         if (i && ssl_fill_hello_random(s, 0, p,
//                                        sizeof(s->s3->client_random)) <= 0)
//             goto err;
  
           
          d = p = ssl_handshake_start(s);
          
         *(p++) = s->client_version >> 8;
         *(p++) = s->client_version & 0xff;
 
          
         memcpy(p, s->s3->client_random, SSL3_RANDOM_SIZE);
         p += SSL3_RANDOM_SIZE;
 
          
         if (s->new_session)
             i = 0;
         else
             i = s->session->session_id_length;
         *(p++) = i;
         if (i != 0) {
             if (i > (int)sizeof(s->session->session_id)) {
                 SSLerr(SSL_F_SSL3_CLIENT_HELLO, ERR_R_INTERNAL_ERROR);
                 goto err;
             }
             memcpy(p, s->session->session_id, i);
             p += i;
         }
 
          
         if (SSL_IS_DTLS(s)) {
             if (s->d1->cookie_len > sizeof(s->d1->cookie)) {
                 SSLerr(SSL_F_SSL3_CLIENT_HELLO, ERR_R_INTERNAL_ERROR);
                 goto err;
             }
             *(p++) = s->d1->cookie_len;
             memcpy(p, s->d1->cookie, s->d1->cookie_len);
             p += s->d1->cookie_len;
         }
 
          
         i = ssl_cipher_list_to_bytes(s, SSL_get_ciphers(s), &(p[2]), 0);
         if (i == 0) {
             SSLerr(SSL_F_SSL3_CLIENT_HELLO, SSL_R_NO_CIPHERS_AVAILABLE);
             goto err;
         }
 #ifdef OPENSSL_MAX_TLS1_2_CIPHER_LENGTH
          
         if (TLS1_get_version(s) >= TLS1_2_VERSION
             && i > OPENSSL_MAX_TLS1_2_CIPHER_LENGTH)
             i = OPENSSL_MAX_TLS1_2_CIPHER_LENGTH & ~1;
 #endif
         s2n(i, p);
         p += i;
 
          
 #ifdef OPENSSL_NO_COMP
         *(p++) = 1;
 #else
 
         if (!ssl_allow_compression(s) || !s->ctx->comp_methods)
             j = 0;
         else
             j = sk_SSL_COMP_num(s->ctx->comp_methods);
         *(p++) = 1 + j;
         for (i = 0; i < j; i++) {
             comp = sk_SSL_COMP_value(s->ctx->comp_methods, i);
             *(p++) = comp->id;
         }
 #endif
         *(p++) = 0;              
 
 #ifndef OPENSSL_NO_TLSEXT
          
         if (ssl_prepare_clienthello_tlsext(s) <= 0) {
             SSLerr(SSL_F_SSL3_CLIENT_HELLO, SSL_R_CLIENTHELLO_TLSEXT);
             goto err;
         }
         if ((p =
              ssl_add_clienthello_tlsext(s, p, buf + SSL3_RT_MAX_PLAIN_LENGTH,
                                         &al)) == NULL) {
             ssl3_send_alert(s, SSL3_AL_FATAL, al);
             SSLerr(SSL_F_SSL3_CLIENT_HELLO, ERR_R_INTERNAL_ERROR);
             goto err;
         }
 #endif
 
         l = p - d;
         ssl_set_handshake_header(s, SSL3_MT_CLIENT_HELLO, l);
         s->state = SSL3_ST_CW_CLNT_HELLO_B;
     }
 
      
     return ssl_do_write(s);
  err:
     return (-1);
 }