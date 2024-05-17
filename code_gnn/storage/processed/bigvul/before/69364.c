MSG_PROCESS_RETURN ossl_statem_client_process_message(SSL *s, PACKET *pkt)
{
    OSSL_STATEM *st = &s->statem;

    switch (st->hand_state) {
    case TLS_ST_CR_SRVR_HELLO:
        return tls_process_server_hello(s, pkt);

    case DTLS_ST_CR_HELLO_VERIFY_REQUEST:
        return dtls_process_hello_verify(s, pkt);

    case TLS_ST_CR_CERT:
        return tls_process_server_certificate(s, pkt);

    case TLS_ST_CR_CERT_STATUS:
        return tls_process_cert_status(s, pkt);

    case TLS_ST_CR_KEY_EXCH:
        return tls_process_key_exchange(s, pkt);

    case TLS_ST_CR_CERT_REQ:
        return tls_process_certificate_request(s, pkt);

    case TLS_ST_CR_SRVR_DONE:
        return tls_process_server_done(s, pkt);

    case TLS_ST_CR_CHANGE:
        return tls_process_change_cipher_spec(s, pkt);

    case TLS_ST_CR_SESSION_TICKET:
        return tls_process_new_session_ticket(s, pkt);

    case TLS_ST_CR_FINISHED:
        return tls_process_finished(s, pkt);

    default:
         
        break;
    }

    return MSG_PROCESS_ERROR;
}