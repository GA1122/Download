 static int tls_decrypt_ticket(SSL *s, const unsigned char *etick,
                               int eticklen, const unsigned char *sess_id,
                               int sesslen, SSL_SESSION **psess)
 {
     SSL_SESSION *sess;
     unsigned char *sdec;
     const unsigned char *p;
     int slen, mlen, renew_ticket = 0, ret = -1;
     unsigned char tick_hmac[EVP_MAX_MD_SIZE];
      HMAC_CTX *hctx = NULL;
      EVP_CIPHER_CTX *ctx;
      SSL_CTX *tctx = s->initial_ctx;
     
    if (eticklen < 48)
        return 2;
// 
       
      hctx = HMAC_CTX_new();
      if (hctx == NULL)
     hctx = HMAC_CTX_new();
     if (hctx == NULL)
         return -2;
     ctx = EVP_CIPHER_CTX_new();
     if (ctx == NULL) {
         ret = -2;
         goto err;
     }
     if (tctx->tlsext_ticket_key_cb) {
         unsigned char *nctick = (unsigned char *)etick;
         int rv = tctx->tlsext_ticket_key_cb(s, nctick, nctick + 16,
                                             ctx, hctx, 0);
         if (rv < 0)
             goto err;
         if (rv == 0) {
             ret = 2;
             goto err;
         }
         if (rv == 2)
             renew_ticket = 1;
     } else {
          
         if (memcmp(etick, tctx->tlsext_tick_key_name,
                    sizeof(tctx->tlsext_tick_key_name)) != 0) {
             ret = 2;
             goto err;
         }
         if (HMAC_Init_ex(hctx, tctx->tlsext_tick_hmac_key,
                          sizeof(tctx->tlsext_tick_hmac_key),
                          EVP_sha256(), NULL) <= 0
             || EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL,
                                   tctx->tlsext_tick_aes_key,
                                   etick + sizeof(tctx->tlsext_tick_key_name)) <=
             0) {
             goto err;
         }
     }
      
//     if (eticklen <=
//         TLSEXT_KEYNAME_LENGTH + EVP_CIPHER_CTX_iv_length(ctx) + mlen) {
//         ret = 2;
//         goto err;
//     }
//      eticklen -= mlen;
//       
      if (HMAC_Update(hctx, etick, eticklen) <= 0
     if (CRYPTO_memcmp(tick_hmac, etick + eticklen, mlen)) {
         EVP_CIPHER_CTX_free(ctx);
         return 2;
     }
      
      
     p = etick + 16 + EVP_CIPHER_CTX_iv_length(ctx);
     eticklen -= 16 + EVP_CIPHER_CTX_iv_length(ctx);
     sdec = OPENSSL_malloc(eticklen);
     if (sdec == NULL || EVP_DecryptUpdate(ctx, sdec, &slen, p, eticklen) <= 0) {
         EVP_CIPHER_CTX_free(ctx);
         OPENSSL_free(sdec);
         return -1;
     }
     if (EVP_DecryptFinal(ctx, sdec + slen, &mlen) <= 0) {
         EVP_CIPHER_CTX_free(ctx);
         OPENSSL_free(sdec);
         return 2;
     }
     slen += mlen;
     EVP_CIPHER_CTX_free(ctx);
     ctx = NULL;
     p = sdec;
 
     sess = d2i_SSL_SESSION(NULL, &p, slen);
     OPENSSL_free(sdec);
     if (sess) {
          
         if (sesslen)
             memcpy(sess->session_id, sess_id, sesslen);
         sess->session_id_length = sesslen;
         *psess = sess;
         if (renew_ticket)
             return 4;
         else
             return 3;
     }
     ERR_clear_error();
      
     return 2;
  err:
     EVP_CIPHER_CTX_free(ctx);
     HMAC_CTX_free(hctx);
     return ret;
 }