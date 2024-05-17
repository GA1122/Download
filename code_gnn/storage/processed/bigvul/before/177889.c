 int ssl3_get_client_key_exchange(SSL *s)
 {
     int i, al, ok;
     long n;
     unsigned long alg_k;
     unsigned char *p;
 #ifndef OPENSSL_NO_RSA
     RSA *rsa = NULL;
     EVP_PKEY *pkey = NULL;
 #endif
 #ifndef OPENSSL_NO_DH
     BIGNUM *pub = NULL;
     DH *dh_srvr, *dh_clnt = NULL;
 #endif
 #ifndef OPENSSL_NO_KRB5
     KSSL_ERR kssl_err;
 #endif                           
 
 #ifndef OPENSSL_NO_ECDH
     EC_KEY *srvr_ecdh = NULL;
     EVP_PKEY *clnt_pub_pkey = NULL;
     EC_POINT *clnt_ecpoint = NULL;
     BN_CTX *bn_ctx = NULL;
 #endif
 
     n = s->method->ssl_get_message(s,
                                    SSL3_ST_SR_KEY_EXCH_A,
                                    SSL3_ST_SR_KEY_EXCH_B,
                                    SSL3_MT_CLIENT_KEY_EXCHANGE, 2048, &ok);
 
     if (!ok)
         return ((int)n);
     p = (unsigned char *)s->init_msg;
 
     alg_k = s->s3->tmp.new_cipher->algorithm_mkey;
 
 #ifndef OPENSSL_NO_RSA
     if (alg_k & SSL_kRSA) {
         unsigned char rand_premaster_secret[SSL_MAX_MASTER_KEY_LENGTH];
         int decrypt_len;
         unsigned char decrypt_good, version_good;
         size_t j;
 
          
         if (s->s3->tmp.use_rsa_tmp) {
             if ((s->cert != NULL) && (s->cert->rsa_tmp != NULL))
                 rsa = s->cert->rsa_tmp;
              
             if (rsa == NULL) {
                 al = SSL_AD_HANDSHAKE_FAILURE;
                 SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE,
                        SSL_R_MISSING_TMP_RSA_PKEY);
                 goto f_err;
 
             }
         } else {
             pkey = s->cert->pkeys[SSL_PKEY_RSA_ENC].privatekey;
             if ((pkey == NULL) ||
                 (pkey->type != EVP_PKEY_RSA) || (pkey->pkey.rsa == NULL)) {
                 al = SSL_AD_HANDSHAKE_FAILURE;
                 SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE,
                        SSL_R_MISSING_RSA_CERTIFICATE);
                 goto f_err;
             }
             rsa = pkey->pkey.rsa;
         }
 
          
         if (s->version > SSL3_VERSION && s->version != DTLS1_BAD_VER) {
             n2s(p, i);
             if (n != i + 2) {
                 if (!(s->options & SSL_OP_TLS_D5_BUG)) {
                     al = SSL_AD_DECODE_ERROR;
                     SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE,
                            SSL_R_TLS_RSA_ENCRYPTED_VALUE_LENGTH_IS_WRONG);
                     goto f_err;
                 } else
                     p -= 2;
             } else
                 n = i;
         }
 
          
         if (n < SSL_MAX_MASTER_KEY_LENGTH) {
             al = SSL_AD_DECRYPT_ERROR;
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE,
                    SSL_R_TLS_RSA_ENCRYPTED_VALUE_LENGTH_IS_WRONG);
             goto f_err;
         }
 
          
 
          
         if (RAND_pseudo_bytes(rand_premaster_secret,
                               sizeof(rand_premaster_secret)) <= 0)
             goto err;
         decrypt_len =
             RSA_private_decrypt((int)n, p, p, rsa, RSA_PKCS1_PADDING);
         ERR_clear_error();
 
          
         decrypt_good =
             constant_time_eq_int_8(decrypt_len, SSL_MAX_MASTER_KEY_LENGTH);
 
          
         version_good =
             constant_time_eq_8(p[0], (unsigned)(s->client_version >> 8));
         version_good &=
             constant_time_eq_8(p[1], (unsigned)(s->client_version & 0xff));
 
          
         if (s->options & SSL_OP_TLS_ROLLBACK_BUG) {
             unsigned char workaround_good;
             workaround_good =
                 constant_time_eq_8(p[0], (unsigned)(s->version >> 8));
             workaround_good &=
                 constant_time_eq_8(p[1], (unsigned)(s->version & 0xff));
             version_good |= workaround_good;
         }
 
          
         decrypt_good &= version_good;
 
          
         for (j = 0; j < sizeof(rand_premaster_secret); j++) {
             p[j] = constant_time_select_8(decrypt_good, p[j],
                                           rand_premaster_secret[j]);
         }
 
         s->session->master_key_length =
             s->method->ssl3_enc->generate_master_secret(s,
                                                         s->
                                                         session->master_key,
                                                         p,
                                                         sizeof
                                                         (rand_premaster_secret));
         OPENSSL_cleanse(p, sizeof(rand_premaster_secret));
     } else
 #endif
 #ifndef OPENSSL_NO_DH
     if (alg_k & (SSL_kEDH | SSL_kDHr | SSL_kDHd)) {
         int idx = -1;
         EVP_PKEY *skey = NULL;
         if (n > 1) {
             n2s(p, i);
         } else {
             if (alg_k & SSL_kDHE) {
                 al = SSL_AD_HANDSHAKE_FAILURE;
                 SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE,
                        SSL_R_DH_PUBLIC_VALUE_LENGTH_IS_WRONG);
                 goto f_err;
             }
             i = 0;
         }
         if (n && n != i + 2) {
             if (!(s->options & SSL_OP_SSLEAY_080_CLIENT_DH_BUG)) {
                 SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE,
                        SSL_R_DH_PUBLIC_VALUE_LENGTH_IS_WRONG);
                 goto err;
             } else {
                 p -= 2;
                 i = (int)n;
             }
         }
         if (alg_k & SSL_kDHr)
             idx = SSL_PKEY_DH_RSA;
         else if (alg_k & SSL_kDHd)
             idx = SSL_PKEY_DH_DSA;
         if (idx >= 0) {
             skey = s->cert->pkeys[idx].privatekey;
             if ((skey == NULL) ||
                 (skey->type != EVP_PKEY_DH) || (skey->pkey.dh == NULL)) {
                 al = SSL_AD_HANDSHAKE_FAILURE;
                 SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE,
                        SSL_R_MISSING_RSA_CERTIFICATE);
                 goto f_err;
             }
             dh_srvr = skey->pkey.dh;
         } else if (s->s3->tmp.dh == NULL) {
             al = SSL_AD_HANDSHAKE_FAILURE;
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE,
                    SSL_R_MISSING_TMP_DH_KEY);
             goto f_err;
         } else
             dh_srvr = s->s3->tmp.dh;
 
         if (n == 0L) {
              
             EVP_PKEY *clkey = X509_get_pubkey(s->session->peer);
             if (clkey) {
                 if (EVP_PKEY_cmp_parameters(clkey, skey) == 1)
                     dh_clnt = EVP_PKEY_get1_DH(clkey);
             }
             if (dh_clnt == NULL) {
                 al = SSL_AD_HANDSHAKE_FAILURE;
                 SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE,
                        SSL_R_MISSING_TMP_DH_KEY);
                 goto f_err;
             }
             EVP_PKEY_free(clkey);
             pub = dh_clnt->pub_key;
         } else
             pub = BN_bin2bn(p, i, NULL);
         if (pub == NULL) {
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE, SSL_R_BN_LIB);
             goto err;
         }
 
         i = DH_compute_key(p, pub, dh_srvr);
 
         if (i <= 0) {
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE, ERR_R_DH_LIB);
             BN_clear_free(pub);
             goto err;
         }
 
         DH_free(s->s3->tmp.dh);
         s->s3->tmp.dh = NULL;
         if (dh_clnt)
             DH_free(dh_clnt);
         else
             BN_clear_free(pub);
         pub = NULL;
         s->session->master_key_length =
             s->method->ssl3_enc->generate_master_secret(s,
                                                         s->
                                                         session->master_key,
                                                         p, i);
         OPENSSL_cleanse(p, i);
         if (dh_clnt)
             return 2;
     } else
 #endif
 #ifndef OPENSSL_NO_KRB5
     if (alg_k & SSL_kKRB5) {
         krb5_error_code krb5rc;
         krb5_data enc_ticket;
         krb5_data authenticator;
         krb5_data enc_pms;
         KSSL_CTX *kssl_ctx = s->kssl_ctx;
         EVP_CIPHER_CTX ciph_ctx;
         const EVP_CIPHER *enc = NULL;
         unsigned char iv[EVP_MAX_IV_LENGTH];
         unsigned char pms[SSL_MAX_MASTER_KEY_LENGTH + EVP_MAX_BLOCK_LENGTH];
         int padl, outl;
         krb5_timestamp authtime = 0;
         krb5_ticket_times ttimes;
         int kerr = 0;
 
         EVP_CIPHER_CTX_init(&ciph_ctx);
 
         if (!kssl_ctx)
             kssl_ctx = kssl_ctx_new();
 
         n2s(p, i);
         enc_ticket.length = i;
 
         if (n < (long)(enc_ticket.length + 6)) {
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE,
                    SSL_R_DATA_LENGTH_TOO_LONG);
             goto err;
         }
 
         enc_ticket.data = (char *)p;
         p += enc_ticket.length;
 
         n2s(p, i);
         authenticator.length = i;
 
         if (n < (long)(enc_ticket.length + authenticator.length + 6)) {
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE,
                    SSL_R_DATA_LENGTH_TOO_LONG);
             goto err;
         }
 
         authenticator.data = (char *)p;
         p += authenticator.length;
 
         n2s(p, i);
         enc_pms.length = i;
         enc_pms.data = (char *)p;
         p += enc_pms.length;
 
          
         if (enc_pms.length > sizeof pms) {
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE,
                    SSL_R_DATA_LENGTH_TOO_LONG);
             goto err;
         }
 
         if (n != (long)(enc_ticket.length + authenticator.length +
                         enc_pms.length + 6)) {
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE,
                    SSL_R_DATA_LENGTH_TOO_LONG);
             goto err;
         }
 
         if ((krb5rc = kssl_sget_tkt(kssl_ctx, &enc_ticket, &ttimes,
                                     &kssl_err)) != 0) {
 # ifdef KSSL_DEBUG
             fprintf(stderr, "kssl_sget_tkt rtn %d [%d]\n",
                     krb5rc, kssl_err.reason);
             if (kssl_err.text)
                 fprintf(stderr, "kssl_err text= %s\n", kssl_err.text);
 # endif                          
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE, kssl_err.reason);
             goto err;
         }
 
          
         if ((krb5rc = kssl_check_authent(kssl_ctx, &authenticator,
                                          &authtime, &kssl_err)) != 0) {
 # ifdef KSSL_DEBUG
             fprintf(stderr, "kssl_check_authent rtn %d [%d]\n",
                     krb5rc, kssl_err.reason);
             if (kssl_err.text)
                 fprintf(stderr, "kssl_err text= %s\n", kssl_err.text);
 # endif                          
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE, kssl_err.reason);
             goto err;
         }
 
         if ((krb5rc = kssl_validate_times(authtime, &ttimes)) != 0) {
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE, krb5rc);
             goto err;
         }
 # ifdef KSSL_DEBUG
         kssl_ctx_show(kssl_ctx);
 # endif                          
 
         enc = kssl_map_enc(kssl_ctx->enctype);
         if (enc == NULL)
             goto err;
 
         memset(iv, 0, sizeof iv);  
 
         if (!EVP_DecryptInit_ex(&ciph_ctx, enc, NULL, kssl_ctx->key, iv)) {
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE,
                    SSL_R_DECRYPTION_FAILED);
             goto err;
         }
         if (!EVP_DecryptUpdate(&ciph_ctx, pms, &outl,
                                (unsigned char *)enc_pms.data, enc_pms.length))
         {
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE,
                    SSL_R_DECRYPTION_FAILED);
             kerr = 1;
             goto kclean;
         }
         if (outl > SSL_MAX_MASTER_KEY_LENGTH) {
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE,
                    SSL_R_DATA_LENGTH_TOO_LONG);
             kerr = 1;
             goto kclean;
         }
         if (!EVP_DecryptFinal_ex(&ciph_ctx, &(pms[outl]), &padl)) {
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE,
                    SSL_R_DECRYPTION_FAILED);
             kerr = 1;
             goto kclean;
         }
         outl += padl;
         if (outl > SSL_MAX_MASTER_KEY_LENGTH) {
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE,
                    SSL_R_DATA_LENGTH_TOO_LONG);
             kerr = 1;
             goto kclean;
         }
         if (!((pms[0] == (s->client_version >> 8))
               && (pms[1] == (s->client_version & 0xff)))) {
              
             if (!(s->options & SSL_OP_TLS_ROLLBACK_BUG)) {
                 SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE,
                        SSL_AD_DECODE_ERROR);
                 kerr = 1;
                 goto kclean;
             }
         }
 
         EVP_CIPHER_CTX_cleanup(&ciph_ctx);
 
         s->session->master_key_length =
             s->method->ssl3_enc->generate_master_secret(s,
                                                         s->
                                                         session->master_key,
                                                         pms, outl);
 
         if (kssl_ctx->client_princ) {
             size_t len = strlen(kssl_ctx->client_princ);
             if (len < SSL_MAX_KRB5_PRINCIPAL_LENGTH) {
                 s->session->krb5_client_princ_len = len;
                 memcpy(s->session->krb5_client_princ, kssl_ctx->client_princ,
                        len);
             }
         }
 
          
 
  kclean:
         OPENSSL_cleanse(pms, sizeof(pms));
         if (kerr)
             goto err;
     } else
 #endif                           
 
 #ifndef OPENSSL_NO_ECDH
     if (alg_k & (SSL_kEECDH | SSL_kECDHr | SSL_kECDHe)) {
         int ret = 1;
         int field_size = 0;
         const EC_KEY *tkey;
         const EC_GROUP *group;
         const BIGNUM *priv_key;
 
          
         if ((srvr_ecdh = EC_KEY_new()) == NULL) {
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE, ERR_R_MALLOC_FAILURE);
             goto err;
         }
 
          
         if (alg_k & (SSL_kECDHr | SSL_kECDHe)) {
              
             tkey = s->cert->pkeys[SSL_PKEY_ECC].privatekey->pkey.ec;
         } else {
              
             tkey = s->s3->tmp.ecdh;
         }
 
         group = EC_KEY_get0_group(tkey);
         priv_key = EC_KEY_get0_private_key(tkey);
 
         if (!EC_KEY_set_group(srvr_ecdh, group) ||
             !EC_KEY_set_private_key(srvr_ecdh, priv_key)) {
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE, ERR_R_EC_LIB);
             goto err;
         }
 
          
         if ((clnt_ecpoint = EC_POINT_new(group)) == NULL) {
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE, ERR_R_MALLOC_FAILURE);
             goto err;
         }
 
         if (n == 0L) {
              
 
             if (alg_k & SSL_kEECDH) {
                 al = SSL_AD_HANDSHAKE_FAILURE;
                 SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE,
                        SSL_R_MISSING_TMP_ECDH_KEY);
                 goto f_err;
             }
             if (((clnt_pub_pkey = X509_get_pubkey(s->session->peer))
                  == NULL) || (clnt_pub_pkey->type != EVP_PKEY_EC)) {
                  
                 al = SSL_AD_HANDSHAKE_FAILURE;
                 SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE,
                        SSL_R_UNABLE_TO_DECODE_ECDH_CERTS);
                 goto f_err;
             }
 
             if (EC_POINT_copy(clnt_ecpoint,
                               EC_KEY_get0_public_key(clnt_pub_pkey->
                                                      pkey.ec)) == 0) {
                 SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE, ERR_R_EC_LIB);
                 goto err;
             }
             ret = 2;             
         } else {
              
             if ((bn_ctx = BN_CTX_new()) == NULL) {
                 SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE,
                        ERR_R_MALLOC_FAILURE);
                 goto err;
             }
 
              
             i = *p;
             p += 1;
             if (n != 1 + i) {
                 SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE, ERR_R_EC_LIB);
                 goto err;
             }
             if (EC_POINT_oct2point(group, clnt_ecpoint, p, i, bn_ctx) == 0) {
                 SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE, ERR_R_EC_LIB);
                 goto err;
             }
              
             p = (unsigned char *)s->init_buf->data;
         }
 
          
         field_size = EC_GROUP_get_degree(group);
         if (field_size <= 0) {
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE, ERR_R_ECDH_LIB);
             goto err;
         }
         i = ECDH_compute_key(p, (field_size + 7) / 8, clnt_ecpoint, srvr_ecdh,
                              NULL);
         if (i <= 0) {
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE, ERR_R_ECDH_LIB);
             goto err;
         }
 
         EVP_PKEY_free(clnt_pub_pkey);
         EC_POINT_free(clnt_ecpoint);
         EC_KEY_free(srvr_ecdh);
         BN_CTX_free(bn_ctx);
         EC_KEY_free(s->s3->tmp.ecdh);
         s->s3->tmp.ecdh = NULL;
 
          
         s->session->master_key_length =
             s->method->ssl3_enc->generate_master_secret(s,
                                                         s->
                                                         session->master_key,
                                                         p, i);
 
         OPENSSL_cleanse(p, i);
         return (ret);
     } else
 #endif
 #ifndef OPENSSL_NO_PSK
     if (alg_k & SSL_kPSK) {
         unsigned char *t = NULL;
         unsigned char psk_or_pre_ms[PSK_MAX_PSK_LEN * 2 + 4];
         unsigned int pre_ms_len = 0, psk_len = 0;
         int psk_err = 1;
         char tmp_id[PSK_MAX_IDENTITY_LEN + 1];
 
         al = SSL_AD_HANDSHAKE_FAILURE;
 
         n2s(p, i);
         if (n != i + 2) {
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE, SSL_R_LENGTH_MISMATCH);
             goto psk_err;
         }
         if (i > PSK_MAX_IDENTITY_LEN) {
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE,
                    SSL_R_DATA_LENGTH_TOO_LONG);
             goto psk_err;
         }
         if (s->psk_server_callback == NULL) {
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE,
                    SSL_R_PSK_NO_SERVER_CB);
             goto psk_err;
         }
 
          
         memcpy(tmp_id, p, i);
         memset(tmp_id + i, 0, PSK_MAX_IDENTITY_LEN + 1 - i);
         psk_len = s->psk_server_callback(s, tmp_id,
                                          psk_or_pre_ms,
                                          sizeof(psk_or_pre_ms));
         OPENSSL_cleanse(tmp_id, PSK_MAX_IDENTITY_LEN + 1);
 
         if (psk_len > PSK_MAX_PSK_LEN) {
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE, ERR_R_INTERNAL_ERROR);
             goto psk_err;
         } else if (psk_len == 0) {
              
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE,
                    SSL_R_PSK_IDENTITY_NOT_FOUND);
             al = SSL_AD_UNKNOWN_PSK_IDENTITY;
             goto psk_err;
         }
 
          
         pre_ms_len = 2 + psk_len + 2 + psk_len;
         t = psk_or_pre_ms;
         memmove(psk_or_pre_ms + psk_len + 4, psk_or_pre_ms, psk_len);
         s2n(psk_len, t);
         memset(t, 0, psk_len);
         t += psk_len;
         s2n(psk_len, t);
  
          if (s->session->psk_identity != NULL)
              OPENSSL_free(s->session->psk_identity);
        s->session->psk_identity = BUF_strdup((char *)p);
//         s->session->psk_identity = BUF_strndup((char *)p, i);
          if (s->session->psk_identity == NULL) {
              SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE, ERR_R_MALLOC_FAILURE);
              goto psk_err;
         }
 
         if (s->session->psk_identity_hint != NULL)
             OPENSSL_free(s->session->psk_identity_hint);
         s->session->psk_identity_hint = BUF_strdup(s->ctx->psk_identity_hint);
         if (s->ctx->psk_identity_hint != NULL &&
             s->session->psk_identity_hint == NULL) {
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE, ERR_R_MALLOC_FAILURE);
             goto psk_err;
         }
 
         s->session->master_key_length =
             s->method->ssl3_enc->generate_master_secret(s,
                                                         s->
                                                         session->master_key,
                                                         psk_or_pre_ms,
                                                         pre_ms_len);
         psk_err = 0;
  psk_err:
         OPENSSL_cleanse(psk_or_pre_ms, sizeof(psk_or_pre_ms));
         if (psk_err != 0)
             goto f_err;
     } else
 #endif
 #ifndef OPENSSL_NO_SRP
     if (alg_k & SSL_kSRP) {
         int param_len;
 
         n2s(p, i);
         param_len = i + 2;
         if (param_len > n) {
             al = SSL_AD_DECODE_ERROR;
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE,
                    SSL_R_BAD_SRP_A_LENGTH);
             goto f_err;
         }
         if (!(s->srp_ctx.A = BN_bin2bn(p, i, NULL))) {
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE, ERR_R_BN_LIB);
             goto err;
         }
         if (BN_ucmp(s->srp_ctx.A, s->srp_ctx.N) >= 0
             || BN_is_zero(s->srp_ctx.A)) {
             al = SSL_AD_ILLEGAL_PARAMETER;
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE,
                    SSL_R_BAD_SRP_PARAMETERS);
             goto f_err;
         }
         if (s->session->srp_username != NULL)
             OPENSSL_free(s->session->srp_username);
         s->session->srp_username = BUF_strdup(s->srp_ctx.login);
         if (s->session->srp_username == NULL) {
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE, ERR_R_MALLOC_FAILURE);
             goto err;
         }
 
         if ((s->session->master_key_length =
              SRP_generate_server_master_secret(s,
                                                s->session->master_key)) < 0) {
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE, ERR_R_INTERNAL_ERROR);
             goto err;
         }
 
         p += i;
     } else
 #endif                           
     if (alg_k & SSL_kGOST) {
         int ret = 0;
         EVP_PKEY_CTX *pkey_ctx;
         EVP_PKEY *client_pub_pkey = NULL, *pk = NULL;
         unsigned char premaster_secret[32], *start;
         size_t outlen = 32, inlen;
         unsigned long alg_a;
         int Ttag, Tclass;
         long Tlen;
 
          
         alg_a = s->s3->tmp.new_cipher->algorithm_auth;
         if (alg_a & SSL_aGOST94)
             pk = s->cert->pkeys[SSL_PKEY_GOST94].privatekey;
         else if (alg_a & SSL_aGOST01)
             pk = s->cert->pkeys[SSL_PKEY_GOST01].privatekey;
 
         pkey_ctx = EVP_PKEY_CTX_new(pk, NULL);
         EVP_PKEY_decrypt_init(pkey_ctx);
          
         client_pub_pkey = X509_get_pubkey(s->session->peer);
         if (client_pub_pkey) {
             if (EVP_PKEY_derive_set_peer(pkey_ctx, client_pub_pkey) <= 0)
                 ERR_clear_error();
         }
          
         if (ASN1_get_object
             ((const unsigned char **)&p, &Tlen, &Ttag, &Tclass,
              n) != V_ASN1_CONSTRUCTED || Ttag != V_ASN1_SEQUENCE
             || Tclass != V_ASN1_UNIVERSAL) {
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE,
                    SSL_R_DECRYPTION_FAILED);
             goto gerr;
         }
         start = p;
         inlen = Tlen;
         if (EVP_PKEY_decrypt
             (pkey_ctx, premaster_secret, &outlen, start, inlen) <= 0) {
             SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE,
                    SSL_R_DECRYPTION_FAILED);
             goto gerr;
         }
          
         s->session->master_key_length =
             s->method->ssl3_enc->generate_master_secret(s,
                                                         s->
                                                         session->master_key,
                                                         premaster_secret, 32);
         OPENSSL_cleanse(premaster_secret, sizeof(premaster_secret));
          
         if (EVP_PKEY_CTX_ctrl
             (pkey_ctx, -1, -1, EVP_PKEY_CTRL_PEER_KEY, 2, NULL) > 0)
             ret = 2;
         else
             ret = 1;
  gerr:
         EVP_PKEY_free(client_pub_pkey);
         EVP_PKEY_CTX_free(pkey_ctx);
         if (ret)
             return ret;
         else
             goto err;
     } else {
         al = SSL_AD_HANDSHAKE_FAILURE;
         SSLerr(SSL_F_SSL3_GET_CLIENT_KEY_EXCHANGE, SSL_R_UNKNOWN_CIPHER_TYPE);
         goto f_err;
     }
 
     return (1);
  f_err:
     ssl3_send_alert(s, SSL3_AL_FATAL, al);
 #if !defined(OPENSSL_NO_DH) || !defined(OPENSSL_NO_RSA) || !defined(OPENSSL_NO_ECDH) || defined(OPENSSL_NO_SRP)
  err:
 #endif
 #ifndef OPENSSL_NO_ECDH
     EVP_PKEY_free(clnt_pub_pkey);
     EC_POINT_free(clnt_ecpoint);
     if (srvr_ecdh != NULL)
         EC_KEY_free(srvr_ecdh);
     BN_CTX_free(bn_ctx);
 #endif
     s->state = SSL_ST_ERR;
     return (-1);
 }