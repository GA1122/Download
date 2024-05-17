int ssl3_get_req_cert_type(SSL *s, unsigned char *p)
{
    int ret = 0;
    const unsigned char *sig;
    size_t i, siglen;
    int have_rsa_sign = 0, have_dsa_sign = 0;
#ifndef OPENSSL_NO_ECDSA
    int have_ecdsa_sign = 0;
#endif
    int nostrict = 1;
    unsigned long alg_k;

     
    if (s->cert->ctypes) {
        memcpy(p, s->cert->ctypes, s->cert->ctype_num);
        return (int)s->cert->ctype_num;
    }
     
    siglen = tls12_get_psigalgs(s, &sig);
    if (s->cert->cert_flags & SSL_CERT_FLAGS_CHECK_TLS_STRICT)
        nostrict = 0;
    for (i = 0; i < siglen; i += 2, sig += 2) {
        switch (sig[1]) {
        case TLSEXT_signature_rsa:
            have_rsa_sign = 1;
            break;

        case TLSEXT_signature_dsa:
            have_dsa_sign = 1;
            break;
#ifndef OPENSSL_NO_ECDSA
        case TLSEXT_signature_ecdsa:
            have_ecdsa_sign = 1;
            break;
#endif
        }
    }

    alg_k = s->s3->tmp.new_cipher->algorithm_mkey;

#ifndef OPENSSL_NO_GOST
    if (s->version >= TLS1_VERSION) {
        if (alg_k & SSL_kGOST) {
            p[ret++] = TLS_CT_GOST94_SIGN;
            p[ret++] = TLS_CT_GOST01_SIGN;
            return (ret);
        }
    }
#endif

#ifndef OPENSSL_NO_DH
    if (alg_k & (SSL_kDHr | SSL_kEDH)) {
# ifndef OPENSSL_NO_RSA
         
        if (nostrict || have_rsa_sign)
            p[ret++] = SSL3_CT_RSA_FIXED_DH;
# endif
# ifndef OPENSSL_NO_DSA
        if (nostrict || have_dsa_sign)
            p[ret++] = SSL3_CT_DSS_FIXED_DH;
# endif
    }
    if ((s->version == SSL3_VERSION) &&
        (alg_k & (SSL_kEDH | SSL_kDHd | SSL_kDHr))) {
# ifndef OPENSSL_NO_RSA
        p[ret++] = SSL3_CT_RSA_EPHEMERAL_DH;
# endif
# ifndef OPENSSL_NO_DSA
        p[ret++] = SSL3_CT_DSS_EPHEMERAL_DH;
# endif
    }
#endif                           
#ifndef OPENSSL_NO_RSA
    if (have_rsa_sign)
        p[ret++] = SSL3_CT_RSA_SIGN;
#endif
#ifndef OPENSSL_NO_DSA
    if (have_dsa_sign)
        p[ret++] = SSL3_CT_DSS_SIGN;
#endif
#ifndef OPENSSL_NO_ECDH
    if ((alg_k & (SSL_kECDHr | SSL_kECDHe)) && (s->version >= TLS1_VERSION)) {
        if (nostrict || have_rsa_sign)
            p[ret++] = TLS_CT_RSA_FIXED_ECDH;
        if (nostrict || have_ecdsa_sign)
            p[ret++] = TLS_CT_ECDSA_FIXED_ECDH;
    }
#endif

#ifndef OPENSSL_NO_ECDSA
     
    if (s->version >= TLS1_VERSION) {
        if (have_ecdsa_sign)
            p[ret++] = TLS_CT_ECDSA_SIGN;
    }
#endif
    return (ret);
}
