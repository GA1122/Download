int tls1_shared_curve(SSL *s, int nmatch)
{
    const unsigned char *pref, *supp;
    size_t num_pref, num_supp, i, j;
    int k;
     
    if (s->server == 0)
        return -1;
    if (nmatch == -2) {
        if (tls1_suiteb(s)) {
             
            unsigned long cid = s->s3->tmp.new_cipher->id;
            if (cid == TLS1_CK_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256)
                return NID_X9_62_prime256v1;  
            if (cid == TLS1_CK_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384)
                return NID_secp384r1;  
             
            return NID_undef;
        }
         
        nmatch = 0;
    }
     
    if (!tls1_get_curvelist
        (s, (s->options & SSL_OP_CIPHER_SERVER_PREFERENCE) != 0, &supp,
         &num_supp))
         
        return nmatch == -1 ? 0 : NID_undef;
    if (!tls1_get_curvelist
        (s, !(s->options & SSL_OP_CIPHER_SERVER_PREFERENCE), &pref,
         &num_pref))
        return nmatch == -1 ? 0 : NID_undef;

     
    if (num_supp == 0 && (s->options & SSL_OP_CIPHER_SERVER_PREFERENCE) != 0) {
        supp = eccurves_all;
        num_supp = sizeof(eccurves_all) / 2;
    } else if (num_pref == 0 &&
        (s->options & SSL_OP_CIPHER_SERVER_PREFERENCE) == 0) {
        pref = eccurves_all;
        num_pref = sizeof(eccurves_all) / 2;
    }

    k = 0;
    for (i = 0; i < num_pref; i++, pref += 2) {
        const unsigned char *tsupp = supp;
        for (j = 0; j < num_supp; j++, tsupp += 2) {
            if (pref[0] == tsupp[0] && pref[1] == tsupp[1]) {
                if (nmatch == k) {
                    int id = (pref[0] << 8) | pref[1];
                    return tls1_ec_curve_id2nid(id);
                }
                k++;
            }
        }
    }
    if (nmatch == -1)
        return k;
     
    return NID_undef;
}
