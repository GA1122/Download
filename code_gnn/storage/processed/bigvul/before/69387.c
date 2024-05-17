static int tls_process_ske_dhe(SSL *s, PACKET *pkt, EVP_PKEY **pkey, int *al)
{
#ifndef OPENSSL_NO_DH
    PACKET prime, generator, pub_key;
    EVP_PKEY *peer_tmp = NULL;

    DH *dh = NULL;
    BIGNUM *p = NULL, *g = NULL, *bnpub_key = NULL;

    if (!PACKET_get_length_prefixed_2(pkt, &prime)
        || !PACKET_get_length_prefixed_2(pkt, &generator)
        || !PACKET_get_length_prefixed_2(pkt, &pub_key)) {
        *al = SSL_AD_DECODE_ERROR;
        SSLerr(SSL_F_TLS_PROCESS_SKE_DHE, SSL_R_LENGTH_MISMATCH);
        return 0;
    }

    peer_tmp = EVP_PKEY_new();
    dh = DH_new();

    if (peer_tmp == NULL || dh == NULL) {
        *al = SSL_AD_INTERNAL_ERROR;
        SSLerr(SSL_F_TLS_PROCESS_SKE_DHE, ERR_R_MALLOC_FAILURE);
        goto err;
    }

    p = BN_bin2bn(PACKET_data(&prime), PACKET_remaining(&prime), NULL);
    g = BN_bin2bn(PACKET_data(&generator), PACKET_remaining(&generator), NULL);
    bnpub_key = BN_bin2bn(PACKET_data(&pub_key), PACKET_remaining(&pub_key),
                          NULL);
    if (p == NULL || g == NULL || bnpub_key == NULL) {
        *al = SSL_AD_INTERNAL_ERROR;
        SSLerr(SSL_F_TLS_PROCESS_SKE_DHE, ERR_R_BN_LIB);
        goto err;
    }

    if (BN_is_zero(p) || BN_is_zero(g) || BN_is_zero(bnpub_key)) {
        *al = SSL_AD_DECODE_ERROR;
        SSLerr(SSL_F_TLS_PROCESS_SKE_DHE, SSL_R_BAD_DH_VALUE);
        goto err;
    }

    if (!DH_set0_pqg(dh, p, NULL, g)) {
        *al = SSL_AD_INTERNAL_ERROR;
        SSLerr(SSL_F_TLS_PROCESS_SKE_DHE, ERR_R_BN_LIB);
        goto err;
    }
    p = g = NULL;

    if (!DH_set0_key(dh, bnpub_key, NULL)) {
        *al = SSL_AD_INTERNAL_ERROR;
        SSLerr(SSL_F_TLS_PROCESS_SKE_DHE, ERR_R_BN_LIB);
        goto err;
    }
    bnpub_key = NULL;

    if (!ssl_security(s, SSL_SECOP_TMP_DH, DH_security_bits(dh), 0, dh)) {
        *al = SSL_AD_HANDSHAKE_FAILURE;
        SSLerr(SSL_F_TLS_PROCESS_SKE_DHE, SSL_R_DH_KEY_TOO_SMALL);
        goto err;
    }

    if (EVP_PKEY_assign_DH(peer_tmp, dh) == 0) {
        *al = SSL_AD_INTERNAL_ERROR;
        SSLerr(SSL_F_TLS_PROCESS_SKE_DHE, ERR_R_EVP_LIB);
        goto err;
    }

    s->s3->peer_tmp = peer_tmp;

     
    if (s->s3->tmp.new_cipher->algorithm_auth & (SSL_aRSA | SSL_aDSS))
        *pkey = X509_get0_pubkey(s->session->peer);
     

    return 1;

 err:
    BN_free(p);
    BN_free(g);
    BN_free(bnpub_key);
    DH_free(dh);
    EVP_PKEY_free(peer_tmp);

    return 0;
#else
    SSLerr(SSL_F_TLS_PROCESS_SKE_DHE, ERR_R_INTERNAL_ERROR);
    *al = SSL_AD_INTERNAL_ERROR;
    return 0;
#endif
}