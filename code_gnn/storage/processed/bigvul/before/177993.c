 int dtls1_listen(SSL *s, struct sockaddr *client)
  {
      int ret;
  
//      
//     SSL_clear(s);
// 
      SSL_set_options(s, SSL_OP_COOKIE_EXCHANGE);
      s->d1->listen = 1;
 
     (void)BIO_dgram_get_peer(SSL_get_rbio(s), client);
     return 1;
 }