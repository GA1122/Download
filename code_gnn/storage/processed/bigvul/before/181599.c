 unsigned char *ssl_add_serverhello_tlsext(SSL *s, unsigned char *buf,
                                           unsigned char *limit, int *al)
 {
     int extdatalen = 0;
     unsigned char *orig = buf;
     unsigned char *ret = buf;
 #ifndef OPENSSL_NO_NEXTPROTONEG
     int next_proto_neg_seen;
 #endif
 #ifndef OPENSSL_NO_EC
     unsigned long alg_k = s->s3->tmp.new_cipher->algorithm_mkey;
     unsigned long alg_a = s->s3->tmp.new_cipher->algorithm_auth;
     int using_ecc = (alg_k & SSL_kECDHE) || (alg_a & SSL_aECDSA);
     using_ecc = using_ecc && (s->session->tlsext_ecpointformatlist != NULL);
 #endif
 
     ret += 2;
     if (ret >= limit)
         return NULL;             
 
     if (s->s3->send_connection_binding) {
         int el;
 
         if (!ssl_add_serverhello_renegotiate_ext(s, 0, &el, 0)) {
             SSLerr(SSL_F_SSL_ADD_SERVERHELLO_TLSEXT, ERR_R_INTERNAL_ERROR);
             return NULL;
         }
 
          
         if (CHECKLEN(ret, 4 + el, limit))
             return NULL;
 
         s2n(TLSEXT_TYPE_renegotiate, ret);
         s2n(el, ret);
 
         if (!ssl_add_serverhello_renegotiate_ext(s, ret, &el, el)) {
             SSLerr(SSL_F_SSL_ADD_SERVERHELLO_TLSEXT, ERR_R_INTERNAL_ERROR);
             return NULL;
         }
 
         ret += el;
     }
 
      
     if (s->version == SSL3_VERSION)
         goto done;
 
     if (!s->hit && s->servername_done == 1
         && s->session->tlsext_hostname != NULL) {
          
         if (CHECKLEN(ret, 4, limit))
             return NULL;
 
         s2n(TLSEXT_TYPE_server_name, ret);
         s2n(0, ret);
     }
 #ifndef OPENSSL_NO_EC
     if (using_ecc) {
         const unsigned char *plist;
         size_t plistlen;
          
 
         tls1_get_formatlist(s, &plist, &plistlen);
 
         if (plistlen > 255) {
             SSLerr(SSL_F_SSL_ADD_SERVERHELLO_TLSEXT, ERR_R_INTERNAL_ERROR);
             return NULL;
         }
 
          
         if (CHECKLEN(ret, 5 + plistlen, limit))
             return NULL;
 
         s2n(TLSEXT_TYPE_ec_point_formats, ret);
         s2n(plistlen + 1, ret);
         *(ret++) = (unsigned char)plistlen;
         memcpy(ret, plist, plistlen);
         ret += plistlen;
 
     }
      
 #endif                           
 
     if (s->tlsext_ticket_expected && tls_use_ticket(s)) {
          
         if (CHECKLEN(ret, 4, limit))
             return NULL;
         s2n(TLSEXT_TYPE_session_ticket, ret);
         s2n(0, ret);
     } else {
          
         s->tlsext_ticket_expected = 0;
     }
 
     if (s->tlsext_status_expected) {
          
         if (CHECKLEN(ret, 4, limit))
             return NULL;
         s2n(TLSEXT_TYPE_status_request, ret);
         s2n(0, ret);
     }
 #ifndef OPENSSL_NO_SRTP
     if (SSL_IS_DTLS(s) && s->srtp_profile) {
         int el;
 
          
         if (ssl_add_serverhello_use_srtp_ext(s, 0, &el, 0)) {
             SSLerr(SSL_F_SSL_ADD_SERVERHELLO_TLSEXT, ERR_R_INTERNAL_ERROR);
             return NULL;
         }
          
         if (CHECKLEN(ret, 4 + el, limit))
             return NULL;
 
         s2n(TLSEXT_TYPE_use_srtp, ret);
         s2n(el, ret);
 
         if (ssl_add_serverhello_use_srtp_ext(s, ret, &el, el)) {
             SSLerr(SSL_F_SSL_ADD_SERVERHELLO_TLSEXT, ERR_R_INTERNAL_ERROR);
             return NULL;
         }
         ret += el;
     }
 #endif
 
     if (((s->s3->tmp.new_cipher->id & 0xFFFF) == 0x80
          || (s->s3->tmp.new_cipher->id & 0xFFFF) == 0x81)
         && (SSL_get_options(s) & SSL_OP_CRYPTOPRO_TLSEXT_BUG)) {
         const unsigned char cryptopro_ext[36] = {
             0xfd, 0xe8,          
             0x00, 0x20,          
             0x30, 0x1e, 0x30, 0x08, 0x06, 0x06, 0x2a, 0x85,
             0x03, 0x02, 0x02, 0x09, 0x30, 0x08, 0x06, 0x06,
             0x2a, 0x85, 0x03, 0x02, 0x02, 0x16, 0x30, 0x08,
             0x06, 0x06, 0x2a, 0x85, 0x03, 0x02, 0x02, 0x17
         };
 
          
         if (CHECKLEN(ret, sizeof(cryptopro_ext), limit))
             return NULL;
         memcpy(ret, cryptopro_ext, sizeof(cryptopro_ext));
         ret += sizeof(cryptopro_ext);
 
     }
 #ifndef OPENSSL_NO_HEARTBEATS
      
     if (SSL_IS_DTLS(s) && (s->tlsext_heartbeat & SSL_DTLSEXT_HB_ENABLED)) {
          
         if (CHECKLEN(ret, 5, limit))
             return NULL;
         s2n(TLSEXT_TYPE_heartbeat, ret);
         s2n(1, ret);
          
         if (s->tlsext_heartbeat & SSL_DTLSEXT_HB_DONT_RECV_REQUESTS)
             *(ret++) = SSL_DTLSEXT_HB_DONT_SEND_REQUESTS;
         else
             *(ret++) = SSL_DTLSEXT_HB_ENABLED;
 
     }
 #endif
 
 #ifndef OPENSSL_NO_NEXTPROTONEG
     next_proto_neg_seen = s->s3->next_proto_neg_seen;
     s->s3->next_proto_neg_seen = 0;
     if (next_proto_neg_seen && s->ctx->next_protos_advertised_cb) {
         const unsigned char *npa;
         unsigned int npalen;
         int r;
 
         r = s->ctx->next_protos_advertised_cb(s, &npa, &npalen,
                                               s->
                                               ctx->next_protos_advertised_cb_arg);
         if (r == SSL_TLSEXT_ERR_OK) {
              
             if (CHECKLEN(ret, 4 + npalen, limit))
                 return NULL;
             s2n(TLSEXT_TYPE_next_proto_neg, ret);
             s2n(npalen, ret);
             memcpy(ret, npa, npalen);
             ret += npalen;
             s->s3->next_proto_neg_seen = 1;
         }
     }
  #endif
      if (!custom_ext_add(s, 1, &ret, limit, al))
          return NULL;
    if (s->s3->flags & TLS1_FLAGS_ENCRYPT_THEN_MAC) {
//     if (s->tlsext_use_etm) {
           
         if (SSL_IS_DTLS(s) || s->s3->tmp.new_cipher->algorithm_mac == SSL_AEAD
              || s->s3->tmp.new_cipher->algorithm_enc == SSL_RC4
              || s->s3->tmp.new_cipher->algorithm_enc == SSL_eGOST2814789CNT
              || s->s3->tmp.new_cipher->algorithm_enc == SSL_eGOST2814789CNT12)
            s->s3->flags &= ~TLS1_FLAGS_ENCRYPT_THEN_MAC;
//             s->tlsext_use_etm = 0;
          else {
               
             if (CHECKLEN(ret, 4, limit))
                 return NULL;
             s2n(TLSEXT_TYPE_encrypt_then_mac, ret);
             s2n(0, ret);
         }
     }
     if (s->s3->flags & TLS1_FLAGS_RECEIVED_EXTMS) {
          
         if (CHECKLEN(ret, 4, limit))
             return NULL;
         s2n(TLSEXT_TYPE_extended_master_secret, ret);
         s2n(0, ret);
     }
 
     if (s->s3->alpn_selected != NULL) {
         const unsigned char *selected = s->s3->alpn_selected;
         size_t len = s->s3->alpn_selected_len;
 
          
         if (CHECKLEN(ret, 7 + len, limit))
             return NULL;
         s2n(TLSEXT_TYPE_application_layer_protocol_negotiation, ret);
         s2n(3 + len, ret);
         s2n(1 + len, ret);
         *ret++ = len;
         memcpy(ret, selected, len);
         ret += len;
     }
 
  done:
 
     if ((extdatalen = ret - orig - 2) == 0)
         return orig;
 
     s2n(extdatalen, orig);
     return ret;
 }