int tls_client_key_exchange_post_work(SSL *s)
{
    unsigned char *pms = NULL;
    size_t pmslen = 0;

    pms = s->s3->tmp.pms;
    pmslen = s->s3->tmp.pmslen;

#ifndef OPENSSL_NO_SRP
     
    if (s->s3->tmp.new_cipher->algorithm_mkey & SSL_kSRP) {
        if (!srp_generate_client_master_secret(s)) {
            SSLerr(SSL_F_TLS_CLIENT_KEY_EXCHANGE_POST_WORK,
                   ERR_R_INTERNAL_ERROR);
            goto err;
        }
        return 1;
    }
#endif

    if (pms == NULL && !(s->s3->tmp.new_cipher->algorithm_mkey & SSL_kPSK)) {
        ssl3_send_alert(s, SSL3_AL_FATAL, SSL_AD_INTERNAL_ERROR);
        SSLerr(SSL_F_TLS_CLIENT_KEY_EXCHANGE_POST_WORK, ERR_R_MALLOC_FAILURE);
        goto err;
    }
    if (!ssl_generate_master_secret(s, pms, pmslen, 1)) {
        ssl3_send_alert(s, SSL3_AL_FATAL, SSL_AD_INTERNAL_ERROR);
        SSLerr(SSL_F_TLS_CLIENT_KEY_EXCHANGE_POST_WORK, ERR_R_INTERNAL_ERROR);
         
        pms = NULL;
        pmslen = 0;
        goto err;
    }
    pms = NULL;
    pmslen = 0;

#ifndef OPENSSL_NO_SCTP
    if (SSL_IS_DTLS(s)) {
        unsigned char sctpauthkey[64];
        char labelbuffer[sizeof(DTLS1_SCTP_AUTH_LABEL)];

         
        memcpy(labelbuffer, DTLS1_SCTP_AUTH_LABEL,
               sizeof(DTLS1_SCTP_AUTH_LABEL));

        if (SSL_export_keying_material(s, sctpauthkey,
                                       sizeof(sctpauthkey), labelbuffer,
                                       sizeof(labelbuffer), NULL, 0, 0) <= 0)
            goto err;

        BIO_ctrl(SSL_get_wbio(s), BIO_CTRL_DGRAM_SCTP_ADD_AUTH_KEY,
                 sizeof(sctpauthkey), sctpauthkey);
    }
#endif

    return 1;
 err:
    OPENSSL_clear_free(pms, pmslen);
    s->s3->tmp.pms = NULL;
    return 0;
}