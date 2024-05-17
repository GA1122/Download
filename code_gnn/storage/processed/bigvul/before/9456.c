int tls1_check_ec_tmp_key(SSL *s, unsigned long cid)
{
     
    if (tls1_suiteb(s)) {
        unsigned char curve_id[2];
         
        if (cid == TLS1_CK_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256)
            curve_id[1] = TLSEXT_curve_P_256;
        else if (cid == TLS1_CK_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384)
            curve_id[1] = TLSEXT_curve_P_384;
        else
            return 0;
        curve_id[0] = 0;
         
        if (!tls1_check_ec_key(s, curve_id, NULL))
            return 0;
        return 1;
    }
     
    if (tls1_shared_curve(s, 0))
        return 1;
    return 0;
}
