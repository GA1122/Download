static int tls_process_ske_ecdhe(SSL *s, PACKET *pkt, EVP_PKEY **pkey, int *al)
{
#ifndef OPENSSL_NO_EC
    PACKET encoded_pt;
    const unsigned char *ecparams;
    int curve_nid;
    unsigned int curve_flags;
    EVP_PKEY_CTX *pctx = NULL;

     
    if (!PACKET_get_bytes(pkt, &ecparams, 3)) {
        *al = SSL_AD_DECODE_ERROR;
        SSLerr(SSL_F_TLS_PROCESS_SKE_ECDHE, SSL_R_LENGTH_TOO_SHORT);
        return 0;
    }
     
    if (!tls1_check_curve(s, ecparams, 3)) {
        *al = SSL_AD_DECODE_ERROR;
        SSLerr(SSL_F_TLS_PROCESS_SKE_ECDHE, SSL_R_WRONG_CURVE);
        return 0;
    }

    curve_nid = tls1_ec_curve_id2nid(*(ecparams + 2), &curve_flags);

    if (curve_nid == 0) {
        *al = SSL_AD_INTERNAL_ERROR;
        SSLerr(SSL_F_TLS_PROCESS_SKE_ECDHE,
               SSL_R_UNABLE_TO_FIND_ECDH_PARAMETERS);
        return 0;
    }

    if ((curve_flags & TLS_CURVE_TYPE) == TLS_CURVE_CUSTOM) {
        EVP_PKEY *key = EVP_PKEY_new();

        if (key == NULL || !EVP_PKEY_set_type(key, curve_nid)) {
            *al = SSL_AD_INTERNAL_ERROR;
            SSLerr(SSL_F_TLS_PROCESS_SKE_ECDHE, ERR_R_EVP_LIB);
            EVP_PKEY_free(key);
            return 0;
        }
        s->s3->peer_tmp = key;
    } else {
         
        pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL);
        if (pctx == NULL
            || EVP_PKEY_paramgen_init(pctx) <= 0
            || EVP_PKEY_CTX_set_ec_paramgen_curve_nid(pctx, curve_nid) <= 0
            || EVP_PKEY_paramgen(pctx, &s->s3->peer_tmp) <= 0) {
            *al = SSL_AD_INTERNAL_ERROR;
            SSLerr(SSL_F_TLS_PROCESS_SKE_ECDHE, ERR_R_EVP_LIB);
            EVP_PKEY_CTX_free(pctx);
            return 0;
        }
        EVP_PKEY_CTX_free(pctx);
        pctx = NULL;
    }

    if (!PACKET_get_length_prefixed_1(pkt, &encoded_pt)) {
        *al = SSL_AD_DECODE_ERROR;
        SSLerr(SSL_F_TLS_PROCESS_SKE_ECDHE, SSL_R_LENGTH_MISMATCH);
        return 0;
    }

    if (!EVP_PKEY_set1_tls_encodedpoint(s->s3->peer_tmp,
                                        PACKET_data(&encoded_pt),
                                        PACKET_remaining(&encoded_pt))) {
        *al = SSL_AD_DECODE_ERROR;
        SSLerr(SSL_F_TLS_PROCESS_SKE_ECDHE, SSL_R_BAD_ECPOINT);
        return 0;
    }

     
    if (s->s3->tmp.new_cipher->algorithm_auth & SSL_aECDSA)
        *pkey = X509_get0_pubkey(s->session->peer);
    else if (s->s3->tmp.new_cipher->algorithm_auth & SSL_aRSA)
        *pkey = X509_get0_pubkey(s->session->peer);
     

    return 1;
#else
    SSLerr(SSL_F_TLS_PROCESS_SKE_ECDHE, ERR_R_INTERNAL_ERROR);
    *al = SSL_AD_INTERNAL_ERROR;
    return 0;
#endif
}