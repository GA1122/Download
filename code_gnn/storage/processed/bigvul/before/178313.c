 int ssl3_send_alert(SSL *s, int level, int desc)
 {
      
     desc = s->method->ssl3_enc->alert_value(desc);
     if (s->version == SSL3_VERSION && desc == SSL_AD_PROTOCOL_VERSION)
         desc = SSL_AD_HANDSHAKE_FAILURE;  
                                            * protocol_version alerts */
      if (desc < 0)
          return -1;
      
    if ((level == 2) && (s->session != NULL))
        SSL_CTX_remove_session(s->session_ctx, s->session);
//     if (level == SSL3_AL_FATAL) {
//         if (s->session != NULL)
//             SSL_CTX_remove_session(s->session_ctx, s->session);
//         s->state = SSL_ST_ERR;
//     }
  
      s->s3->alert_dispatch = 1;
      s->s3->send_alert[0] = level;
      * else data is still being written out, we will get written some time in
      * the future
      */
     return -1;
 }