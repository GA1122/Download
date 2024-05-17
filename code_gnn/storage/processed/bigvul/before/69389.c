static int tls_process_ske_psk_preamble(SSL *s, PACKET *pkt, int *al)
{
#ifndef OPENSSL_NO_PSK
    PACKET psk_identity_hint;

     

    if (!PACKET_get_length_prefixed_2(pkt, &psk_identity_hint)) {
        *al = SSL_AD_DECODE_ERROR;
        SSLerr(SSL_F_TLS_PROCESS_SKE_PSK_PREAMBLE, SSL_R_LENGTH_MISMATCH);
        return 0;
    }

     
    if (PACKET_remaining(&psk_identity_hint) > PSK_MAX_IDENTITY_LEN) {
        *al = SSL_AD_HANDSHAKE_FAILURE;
        SSLerr(SSL_F_TLS_PROCESS_SKE_PSK_PREAMBLE, SSL_R_DATA_LENGTH_TOO_LONG);
        return 0;
    }

    if (PACKET_remaining(&psk_identity_hint) == 0) {
        OPENSSL_free(s->session->psk_identity_hint);
        s->session->psk_identity_hint = NULL;
    } else if (!PACKET_strndup(&psk_identity_hint,
                               &s->session->psk_identity_hint)) {
        *al = SSL_AD_INTERNAL_ERROR;
        return 0;
    }

    return 1;
#else
    SSLerr(SSL_F_TLS_PROCESS_SKE_PSK_PREAMBLE, ERR_R_INTERNAL_ERROR);
    *al = SSL_AD_INTERNAL_ERROR;
    return 0;
#endif
}