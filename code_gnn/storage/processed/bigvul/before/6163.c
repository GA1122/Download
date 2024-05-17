int tls1_check_ec_tmp_key(SSL *s, unsigned long cid)
{
    unsigned char curve_id[2];
    EC_KEY *ec = s->cert->ecdh_tmp;
#  ifdef OPENSSL_SSL_DEBUG_BROKEN_PROTOCOL
     
    if (s->cert->cert_flags & SSL_CERT_FLAG_BROKEN_PROTOCOL)
        return 1;
#  endif
     
    if (tls1_suiteb(s)) {
         
        if (cid == TLS1_CK_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256)
            curve_id[1] = TLSEXT_curve_P_256;
        else if (cid == TLS1_CK_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384)
            curve_id[1] = TLSEXT_curve_P_384;
        else
            return 0;
        curve_id[0] = 0;
         
        if (!tls1_check_ec_key(s, curve_id, NULL))
            return 0;
         
        if (s->cert->ecdh_tmp_auto || s->cert->ecdh_tmp_cb)
            return 1;
         
        else {
            unsigned char curve_tmp[2];
            if (!ec)
                return 0;
            if (!tls1_set_ec_id(curve_tmp, NULL, ec))
                return 0;
            if (!curve_tmp[0] || curve_tmp[1] == curve_id[1])
                return 1;
            return 0;
        }

    }
    if (s->cert->ecdh_tmp_auto) {
         
        if (tls1_shared_curve(s, 0))
            return 1;
        else
            return 0;
    }
    if (!ec) {
        if (s->cert->ecdh_tmp_cb)
            return 1;
        else
            return 0;
    }
    if (!tls1_set_ec_id(curve_id, NULL, ec))
        return 0;
 
#  if 0
    return 1;
#  else
    return tls1_check_ec_key(s, curve_id, NULL);
#  endif
}
