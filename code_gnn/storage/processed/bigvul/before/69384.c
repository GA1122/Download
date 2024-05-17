MSG_PROCESS_RETURN tls_process_server_certificate(SSL *s, PACKET *pkt)
{
    int al, i, ret = MSG_PROCESS_ERROR, exp_idx;
    unsigned long cert_list_len, cert_len;
    X509 *x = NULL;
    const unsigned char *certstart, *certbytes;
    STACK_OF(X509) *sk = NULL;
    EVP_PKEY *pkey = NULL;

    if ((sk = sk_X509_new_null()) == NULL) {
        SSLerr(SSL_F_TLS_PROCESS_SERVER_CERTIFICATE, ERR_R_MALLOC_FAILURE);
        goto err;
    }

    if (!PACKET_get_net_3(pkt, &cert_list_len)
        || PACKET_remaining(pkt) != cert_list_len) {
        al = SSL_AD_DECODE_ERROR;
        SSLerr(SSL_F_TLS_PROCESS_SERVER_CERTIFICATE, SSL_R_LENGTH_MISMATCH);
        goto f_err;
    }
    while (PACKET_remaining(pkt)) {
        if (!PACKET_get_net_3(pkt, &cert_len)
            || !PACKET_get_bytes(pkt, &certbytes, cert_len)) {
            al = SSL_AD_DECODE_ERROR;
            SSLerr(SSL_F_TLS_PROCESS_SERVER_CERTIFICATE,
                   SSL_R_CERT_LENGTH_MISMATCH);
            goto f_err;
        }

        certstart = certbytes;
        x = d2i_X509(NULL, (const unsigned char **)&certbytes, cert_len);
        if (x == NULL) {
            al = SSL_AD_BAD_CERTIFICATE;
            SSLerr(SSL_F_TLS_PROCESS_SERVER_CERTIFICATE, ERR_R_ASN1_LIB);
            goto f_err;
        }
        if (certbytes != (certstart + cert_len)) {
            al = SSL_AD_DECODE_ERROR;
            SSLerr(SSL_F_TLS_PROCESS_SERVER_CERTIFICATE,
                   SSL_R_CERT_LENGTH_MISMATCH);
            goto f_err;
        }
        if (!sk_X509_push(sk, x)) {
            SSLerr(SSL_F_TLS_PROCESS_SERVER_CERTIFICATE, ERR_R_MALLOC_FAILURE);
            goto err;
        }
        x = NULL;
    }

    i = ssl_verify_cert_chain(s, sk);
     
    if (s->verify_mode != SSL_VERIFY_NONE && i <= 0) {
        al = ssl_verify_alarm_type(s->verify_result);
        SSLerr(SSL_F_TLS_PROCESS_SERVER_CERTIFICATE,
               SSL_R_CERTIFICATE_VERIFY_FAILED);
        goto f_err;
    }
    ERR_clear_error();           
    if (i > 1) {
        SSLerr(SSL_F_TLS_PROCESS_SERVER_CERTIFICATE, i);
        al = SSL_AD_HANDSHAKE_FAILURE;
        goto f_err;
    }

    s->session->peer_chain = sk;
     
    x = sk_X509_value(sk, 0);
    sk = NULL;
     

    pkey = X509_get0_pubkey(x);

    if (pkey == NULL || EVP_PKEY_missing_parameters(pkey)) {
        x = NULL;
        al = SSL3_AL_FATAL;
        SSLerr(SSL_F_TLS_PROCESS_SERVER_CERTIFICATE,
               SSL_R_UNABLE_TO_FIND_PUBLIC_KEY_PARAMETERS);
        goto f_err;
    }

    i = ssl_cert_type(x, pkey);
    if (i < 0) {
        x = NULL;
        al = SSL3_AL_FATAL;
        SSLerr(SSL_F_TLS_PROCESS_SERVER_CERTIFICATE,
               SSL_R_UNKNOWN_CERTIFICATE_TYPE);
        goto f_err;
    }

    exp_idx = ssl_cipher_get_cert_index(s->s3->tmp.new_cipher);
    if (exp_idx >= 0 && i != exp_idx
        && (exp_idx != SSL_PKEY_GOST_EC ||
            (i != SSL_PKEY_GOST12_512 && i != SSL_PKEY_GOST12_256
             && i != SSL_PKEY_GOST01))) {
        x = NULL;
        al = SSL_AD_ILLEGAL_PARAMETER;
        SSLerr(SSL_F_TLS_PROCESS_SERVER_CERTIFICATE,
               SSL_R_WRONG_CERTIFICATE_TYPE);
        goto f_err;
    }
    s->session->peer_type = i;

    X509_free(s->session->peer);
    X509_up_ref(x);
    s->session->peer = x;
    s->session->verify_result = s->verify_result;

    x = NULL;
    ret = MSG_PROCESS_CONTINUE_READING;
    goto done;

 f_err:
    ssl3_send_alert(s, SSL3_AL_FATAL, al);
 err:
    ossl_statem_set_error(s);
 done:
    X509_free(x);
    sk_X509_pop_free(sk, X509_free);
    return ret;
}