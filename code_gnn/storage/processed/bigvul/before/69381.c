MSG_PROCESS_RETURN tls_process_certificate_request(SSL *s, PACKET *pkt)
{
    int ret = MSG_PROCESS_ERROR;
    unsigned int list_len, ctype_num, i, name_len;
    X509_NAME *xn = NULL;
    const unsigned char *data;
    const unsigned char *namestart, *namebytes;
    STACK_OF(X509_NAME) *ca_sk = NULL;

    if ((ca_sk = sk_X509_NAME_new(ca_dn_cmp)) == NULL) {
        SSLerr(SSL_F_TLS_PROCESS_CERTIFICATE_REQUEST, ERR_R_MALLOC_FAILURE);
        goto err;
    }

     
    if (!PACKET_get_1(pkt, &ctype_num)
        || !PACKET_get_bytes(pkt, &data, ctype_num)) {
        ssl3_send_alert(s, SSL3_AL_FATAL, SSL_AD_DECODE_ERROR);
        SSLerr(SSL_F_TLS_PROCESS_CERTIFICATE_REQUEST, SSL_R_LENGTH_MISMATCH);
        goto err;
    }
    OPENSSL_free(s->cert->ctypes);
    s->cert->ctypes = NULL;
    if (ctype_num > SSL3_CT_NUMBER) {
         
        s->cert->ctypes = OPENSSL_malloc(ctype_num);
        if (s->cert->ctypes == NULL) {
            SSLerr(SSL_F_TLS_PROCESS_CERTIFICATE_REQUEST, ERR_R_MALLOC_FAILURE);
            goto err;
        }
        memcpy(s->cert->ctypes, data, ctype_num);
        s->cert->ctype_num = (size_t)ctype_num;
        ctype_num = SSL3_CT_NUMBER;
    }
    for (i = 0; i < ctype_num; i++)
        s->s3->tmp.ctype[i] = data[i];

    if (SSL_USE_SIGALGS(s)) {
        if (!PACKET_get_net_2(pkt, &list_len)
            || !PACKET_get_bytes(pkt, &data, list_len)) {
            ssl3_send_alert(s, SSL3_AL_FATAL, SSL_AD_DECODE_ERROR);
            SSLerr(SSL_F_TLS_PROCESS_CERTIFICATE_REQUEST,
                   SSL_R_LENGTH_MISMATCH);
            goto err;
        }

         
        for (i = 0; i < SSL_PKEY_NUM; i++) {
            s->s3->tmp.md[i] = NULL;
            s->s3->tmp.valid_flags[i] = 0;
        }
        if ((list_len & 1) || !tls1_save_sigalgs(s, data, list_len)) {
            ssl3_send_alert(s, SSL3_AL_FATAL, SSL_AD_DECODE_ERROR);
            SSLerr(SSL_F_TLS_PROCESS_CERTIFICATE_REQUEST,
                   SSL_R_SIGNATURE_ALGORITHMS_ERROR);
            goto err;
        }
        if (!tls1_process_sigalgs(s)) {
            ssl3_send_alert(s, SSL3_AL_FATAL, SSL_AD_INTERNAL_ERROR);
            SSLerr(SSL_F_TLS_PROCESS_CERTIFICATE_REQUEST, ERR_R_MALLOC_FAILURE);
            goto err;
        }
    } else {
        ssl_set_default_md(s);
    }

     
    if (!PACKET_get_net_2(pkt, &list_len)
        || PACKET_remaining(pkt) != list_len) {
        ssl3_send_alert(s, SSL3_AL_FATAL, SSL_AD_DECODE_ERROR);
        SSLerr(SSL_F_TLS_PROCESS_CERTIFICATE_REQUEST, SSL_R_LENGTH_MISMATCH);
        goto err;
    }

    while (PACKET_remaining(pkt)) {
        if (!PACKET_get_net_2(pkt, &name_len)
            || !PACKET_get_bytes(pkt, &namebytes, name_len)) {
            ssl3_send_alert(s, SSL3_AL_FATAL, SSL_AD_DECODE_ERROR);
            SSLerr(SSL_F_TLS_PROCESS_CERTIFICATE_REQUEST,
                   SSL_R_LENGTH_MISMATCH);
            goto err;
        }

        namestart = namebytes;

        if ((xn = d2i_X509_NAME(NULL, (const unsigned char **)&namebytes,
                                name_len)) == NULL) {
            ssl3_send_alert(s, SSL3_AL_FATAL, SSL_AD_DECODE_ERROR);
            SSLerr(SSL_F_TLS_PROCESS_CERTIFICATE_REQUEST, ERR_R_ASN1_LIB);
            goto err;
        }

        if (namebytes != (namestart + name_len)) {
            ssl3_send_alert(s, SSL3_AL_FATAL, SSL_AD_DECODE_ERROR);
            SSLerr(SSL_F_TLS_PROCESS_CERTIFICATE_REQUEST,
                   SSL_R_CA_DN_LENGTH_MISMATCH);
            goto err;
        }
        if (!sk_X509_NAME_push(ca_sk, xn)) {
            SSLerr(SSL_F_TLS_PROCESS_CERTIFICATE_REQUEST, ERR_R_MALLOC_FAILURE);
            goto err;
        }
        xn = NULL;
    }

     
    s->s3->tmp.cert_req = 1;
    s->s3->tmp.ctype_num = ctype_num;
    sk_X509_NAME_pop_free(s->s3->tmp.ca_names, X509_NAME_free);
    s->s3->tmp.ca_names = ca_sk;
    ca_sk = NULL;

    ret = MSG_PROCESS_CONTINUE_PROCESSING;
    goto done;
 err:
    ossl_statem_set_error(s);
 done:
    X509_NAME_free(xn);
    sk_X509_NAME_pop_free(ca_sk, X509_NAME_free);
    return ret;
}