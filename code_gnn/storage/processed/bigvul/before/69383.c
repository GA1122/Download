MSG_PROCESS_RETURN tls_process_new_session_ticket(SSL *s, PACKET *pkt)
{
    int al;
    unsigned int ticklen;
    unsigned long ticket_lifetime_hint;

    if (!PACKET_get_net_4(pkt, &ticket_lifetime_hint)
        || !PACKET_get_net_2(pkt, &ticklen)
        || PACKET_remaining(pkt) != ticklen) {
        al = SSL_AD_DECODE_ERROR;
        SSLerr(SSL_F_TLS_PROCESS_NEW_SESSION_TICKET, SSL_R_LENGTH_MISMATCH);
        goto f_err;
    }

     
    if (ticklen == 0)
        return MSG_PROCESS_CONTINUE_READING;

    if (s->session->session_id_length > 0) {
        int i = s->session_ctx->session_cache_mode;
        SSL_SESSION *new_sess;
         
        if (i & SSL_SESS_CACHE_CLIENT) {
             
            SSL_CTX_remove_session(s->session_ctx, s->session);
        }

        if ((new_sess = ssl_session_dup(s->session, 0)) == 0) {
            al = SSL_AD_INTERNAL_ERROR;
            SSLerr(SSL_F_TLS_PROCESS_NEW_SESSION_TICKET, ERR_R_MALLOC_FAILURE);
            goto f_err;
        }

        SSL_SESSION_free(s->session);
        s->session = new_sess;
    }

    OPENSSL_free(s->session->tlsext_tick);
    s->session->tlsext_ticklen = 0;

    s->session->tlsext_tick = OPENSSL_malloc(ticklen);
    if (s->session->tlsext_tick == NULL) {
        SSLerr(SSL_F_TLS_PROCESS_NEW_SESSION_TICKET, ERR_R_MALLOC_FAILURE);
        goto err;
    }
    if (!PACKET_copy_bytes(pkt, s->session->tlsext_tick, ticklen)) {
        al = SSL_AD_DECODE_ERROR;
        SSLerr(SSL_F_TLS_PROCESS_NEW_SESSION_TICKET, SSL_R_LENGTH_MISMATCH);
        goto f_err;
    }

    s->session->tlsext_tick_lifetime_hint = ticket_lifetime_hint;
    s->session->tlsext_ticklen = ticklen;
     
    if (!EVP_Digest(s->session->tlsext_tick, ticklen,
                    s->session->session_id, &s->session->session_id_length,
                    EVP_sha256(), NULL)) {
        SSLerr(SSL_F_TLS_PROCESS_NEW_SESSION_TICKET, ERR_R_EVP_LIB);
        goto err;
    }
    return MSG_PROCESS_CONTINUE_READING;
 f_err:
    ssl3_send_alert(s, SSL3_AL_FATAL, al);
 err:
    ossl_statem_set_error(s);
    return MSG_PROCESS_ERROR;
}
