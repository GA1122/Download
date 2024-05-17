MSG_PROCESS_RETURN tls_process_server_done(SSL *s, PACKET *pkt)
{
    if (PACKET_remaining(pkt) > 0) {
         
        ssl3_send_alert(s, SSL3_AL_FATAL, SSL_AD_DECODE_ERROR);
        SSLerr(SSL_F_TLS_PROCESS_SERVER_DONE, SSL_R_LENGTH_MISMATCH);
        ossl_statem_set_error(s);
        return MSG_PROCESS_ERROR;
    }
#ifndef OPENSSL_NO_SRP
    if (s->s3->tmp.new_cipher->algorithm_mkey & SSL_kSRP) {
        if (SRP_Calc_A_param(s) <= 0) {
            SSLerr(SSL_F_TLS_PROCESS_SERVER_DONE, SSL_R_SRP_A_CALC);
            ssl3_send_alert(s, SSL3_AL_FATAL, SSL_AD_INTERNAL_ERROR);
            ossl_statem_set_error(s);
            return MSG_PROCESS_ERROR;
        }
    }
#endif

     
    if (!ssl3_check_cert_and_algorithm(s)) {
        ssl3_send_alert(s, SSL3_AL_FATAL, SSL_AD_HANDSHAKE_FAILURE);
        ossl_statem_set_error(s);
        return MSG_PROCESS_ERROR;
    }

     
    if (s->tlsext_status_type != -1 && s->ctx->tlsext_status_cb != NULL) {
        int ret;
        ret = s->ctx->tlsext_status_cb(s, s->ctx->tlsext_status_arg);
        if (ret == 0) {
            ssl3_send_alert(s, SSL3_AL_FATAL,
                            SSL_AD_BAD_CERTIFICATE_STATUS_RESPONSE);
            SSLerr(SSL_F_TLS_PROCESS_SERVER_DONE,
                   SSL_R_INVALID_STATUS_RESPONSE);
            return MSG_PROCESS_ERROR;
        }
        if (ret < 0) {
            ssl3_send_alert(s, SSL3_AL_FATAL, SSL_AD_INTERNAL_ERROR);
            SSLerr(SSL_F_TLS_PROCESS_SERVER_DONE, ERR_R_MALLOC_FAILURE);
            return MSG_PROCESS_ERROR;
        }
    }
#ifndef OPENSSL_NO_CT
    if (s->ct_validation_callback != NULL) {
         
        if (!ssl_validate_ct(s) && (s->verify_mode & SSL_VERIFY_PEER)) {
            ssl3_send_alert(s, SSL3_AL_FATAL, SSL_AD_HANDSHAKE_FAILURE);
            return MSG_PROCESS_ERROR;
        }
    }
#endif

#ifndef OPENSSL_NO_SCTP
     
    if (SSL_IS_DTLS(s) && BIO_dgram_is_sctp(SSL_get_wbio(s))
        && s->renegotiate != 0)
        return MSG_PROCESS_CONTINUE_PROCESSING;
    else
#endif
        return MSG_PROCESS_FINISHED_READING;
}