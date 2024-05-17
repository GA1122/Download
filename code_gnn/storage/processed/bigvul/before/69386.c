MSG_PROCESS_RETURN tls_process_server_hello(SSL *s, PACKET *pkt)
{
    STACK_OF(SSL_CIPHER) *sk;
    const SSL_CIPHER *c;
    PACKET session_id;
    size_t session_id_len;
    const unsigned char *cipherchars;
    int i, al = SSL_AD_INTERNAL_ERROR;
    unsigned int compression;
    unsigned int sversion;
    int protverr;
#ifndef OPENSSL_NO_COMP
    SSL_COMP *comp;
#endif

    if (!PACKET_get_net_2(pkt, &sversion)) {
        al = SSL_AD_DECODE_ERROR;
        SSLerr(SSL_F_TLS_PROCESS_SERVER_HELLO, SSL_R_LENGTH_MISMATCH);
        goto f_err;
    }

    protverr = ssl_choose_client_version(s, sversion);
    if (protverr != 0) {
        al = SSL_AD_PROTOCOL_VERSION;
        SSLerr(SSL_F_TLS_PROCESS_SERVER_HELLO, protverr);
        goto f_err;
    }

     
     
    if (!PACKET_copy_bytes(pkt, s->s3->server_random, SSL3_RANDOM_SIZE)) {
        al = SSL_AD_DECODE_ERROR;
        SSLerr(SSL_F_TLS_PROCESS_SERVER_HELLO, SSL_R_LENGTH_MISMATCH);
        goto f_err;
    }

    s->hit = 0;

     
    if (!PACKET_get_length_prefixed_1(pkt, &session_id)) {
        al = SSL_AD_DECODE_ERROR;
        SSLerr(SSL_F_TLS_PROCESS_SERVER_HELLO, SSL_R_LENGTH_MISMATCH);
        goto f_err;
    }
    session_id_len = PACKET_remaining(&session_id);
    if (session_id_len > sizeof s->session->session_id
        || session_id_len > SSL3_SESSION_ID_SIZE) {
        al = SSL_AD_ILLEGAL_PARAMETER;
        SSLerr(SSL_F_TLS_PROCESS_SERVER_HELLO, SSL_R_SSL3_SESSION_ID_TOO_LONG);
        goto f_err;
    }

    if (!PACKET_get_bytes(pkt, &cipherchars, TLS_CIPHER_LEN)) {
        SSLerr(SSL_F_TLS_PROCESS_SERVER_HELLO, SSL_R_LENGTH_MISMATCH);
        al = SSL_AD_DECODE_ERROR;
        goto f_err;
    }

     
    if (s->version >= TLS1_VERSION && s->tls_session_secret_cb &&
        s->session->tlsext_tick) {
        const SSL_CIPHER *pref_cipher = NULL;
        s->session->master_key_length = sizeof(s->session->master_key);
        if (s->tls_session_secret_cb(s, s->session->master_key,
                                     &s->session->master_key_length,
                                     NULL, &pref_cipher,
                                     s->tls_session_secret_cb_arg)) {
            s->session->cipher = pref_cipher ?
                pref_cipher : ssl_get_cipher_by_char(s, cipherchars);
        } else {
            SSLerr(SSL_F_TLS_PROCESS_SERVER_HELLO, ERR_R_INTERNAL_ERROR);
            al = SSL_AD_INTERNAL_ERROR;
            goto f_err;
        }
    }

    if (session_id_len != 0 && session_id_len == s->session->session_id_length
        && memcmp(PACKET_data(&session_id), s->session->session_id,
                  session_id_len) == 0) {
        if (s->sid_ctx_length != s->session->sid_ctx_length
            || memcmp(s->session->sid_ctx, s->sid_ctx, s->sid_ctx_length)) {
             
            al = SSL_AD_ILLEGAL_PARAMETER;
            SSLerr(SSL_F_TLS_PROCESS_SERVER_HELLO,
                   SSL_R_ATTEMPT_TO_REUSE_SESSION_IN_DIFFERENT_CONTEXT);
            goto f_err;
        }
        s->hit = 1;
    } else {
         
        if (s->session->session_id_length > 0) {
            s->ctx->stats.sess_miss++;
            if (!ssl_get_new_session(s, 0)) {
                goto f_err;
            }
        }

        s->session->ssl_version = s->version;
        s->session->session_id_length = session_id_len;
         
        memcpy(s->session->session_id, PACKET_data(&session_id),
               session_id_len);
    }

     
    if (s->version != s->session->ssl_version) {
        al = SSL_AD_PROTOCOL_VERSION;

        SSLerr(SSL_F_TLS_PROCESS_SERVER_HELLO,
               SSL_R_SSL_SESSION_VERSION_MISMATCH);
        goto f_err;
    }

    c = ssl_get_cipher_by_char(s, cipherchars);
    if (c == NULL) {
         
        al = SSL_AD_ILLEGAL_PARAMETER;
        SSLerr(SSL_F_TLS_PROCESS_SERVER_HELLO, SSL_R_UNKNOWN_CIPHER_RETURNED);
        goto f_err;
    }
     
    s->s3->tmp.min_ver = s->version;
    s->s3->tmp.max_ver = s->version;
     
    if (ssl_cipher_disabled(s, c, SSL_SECOP_CIPHER_CHECK)) {
        al = SSL_AD_ILLEGAL_PARAMETER;
        SSLerr(SSL_F_TLS_PROCESS_SERVER_HELLO, SSL_R_WRONG_CIPHER_RETURNED);
        goto f_err;
    }

    sk = ssl_get_ciphers_by_id(s);
    i = sk_SSL_CIPHER_find(sk, c);
    if (i < 0) {
         
        al = SSL_AD_ILLEGAL_PARAMETER;
        SSLerr(SSL_F_TLS_PROCESS_SERVER_HELLO, SSL_R_WRONG_CIPHER_RETURNED);
        goto f_err;
    }

     
    if (s->session->cipher)
        s->session->cipher_id = s->session->cipher->id;
    if (s->hit && (s->session->cipher_id != c->id)) {
        al = SSL_AD_ILLEGAL_PARAMETER;
        SSLerr(SSL_F_TLS_PROCESS_SERVER_HELLO,
               SSL_R_OLD_SESSION_CIPHER_NOT_RETURNED);
        goto f_err;
    }
    s->s3->tmp.new_cipher = c;
     
     
    if (!PACKET_get_1(pkt, &compression)) {
        SSLerr(SSL_F_TLS_PROCESS_SERVER_HELLO, SSL_R_LENGTH_MISMATCH);
        al = SSL_AD_DECODE_ERROR;
        goto f_err;
    }
#ifdef OPENSSL_NO_COMP
    if (compression != 0) {
        al = SSL_AD_ILLEGAL_PARAMETER;
        SSLerr(SSL_F_TLS_PROCESS_SERVER_HELLO,
               SSL_R_UNSUPPORTED_COMPRESSION_ALGORITHM);
        goto f_err;
    }
     
    if (s->session->compress_meth != 0) {
        SSLerr(SSL_F_TLS_PROCESS_SERVER_HELLO, SSL_R_INCONSISTENT_COMPRESSION);
        goto f_err;
    }
#else
    if (s->hit && compression != s->session->compress_meth) {
        al = SSL_AD_ILLEGAL_PARAMETER;
        SSLerr(SSL_F_TLS_PROCESS_SERVER_HELLO,
               SSL_R_OLD_SESSION_COMPRESSION_ALGORITHM_NOT_RETURNED);
        goto f_err;
    }
    if (compression == 0)
        comp = NULL;
    else if (!ssl_allow_compression(s)) {
        al = SSL_AD_ILLEGAL_PARAMETER;
        SSLerr(SSL_F_TLS_PROCESS_SERVER_HELLO, SSL_R_COMPRESSION_DISABLED);
        goto f_err;
    } else {
        comp = ssl3_comp_find(s->ctx->comp_methods, compression);
    }

    if (compression != 0 && comp == NULL) {
        al = SSL_AD_ILLEGAL_PARAMETER;
        SSLerr(SSL_F_TLS_PROCESS_SERVER_HELLO,
               SSL_R_UNSUPPORTED_COMPRESSION_ALGORITHM);
        goto f_err;
    } else {
        s->s3->tmp.new_compression = comp;
    }
#endif

     
    if (!ssl_parse_serverhello_tlsext(s, pkt)) {
        SSLerr(SSL_F_TLS_PROCESS_SERVER_HELLO, SSL_R_PARSE_TLSEXT);
        goto err;
    }

    if (PACKET_remaining(pkt) != 0) {
         
        al = SSL_AD_DECODE_ERROR;
        SSLerr(SSL_F_TLS_PROCESS_SERVER_HELLO, SSL_R_BAD_PACKET_LENGTH);
        goto f_err;
    }
#ifndef OPENSSL_NO_SCTP
    if (SSL_IS_DTLS(s) && s->hit) {
        unsigned char sctpauthkey[64];
        char labelbuffer[sizeof(DTLS1_SCTP_AUTH_LABEL)];

         
        memcpy(labelbuffer, DTLS1_SCTP_AUTH_LABEL,
               sizeof(DTLS1_SCTP_AUTH_LABEL));

        if (SSL_export_keying_material(s, sctpauthkey,
                                       sizeof(sctpauthkey),
                                       labelbuffer,
                                       sizeof(labelbuffer), NULL, 0, 0) <= 0)
            goto err;

        BIO_ctrl(SSL_get_wbio(s),
                 BIO_CTRL_DGRAM_SCTP_ADD_AUTH_KEY,
                 sizeof(sctpauthkey), sctpauthkey);
    }
#endif

    return MSG_PROCESS_CONTINUE_READING;
 f_err:
    ssl3_send_alert(s, SSL3_AL_FATAL, al);
 err:
    ossl_statem_set_error(s);
    return MSG_PROCESS_ERROR;
}
