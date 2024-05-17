static int tls1_check_cert_param(SSL *s, X509 *x, int set_ee_md)
{
    unsigned char comp_id, curve_id[2];
    EVP_PKEY *pkey;
    int rv;
    pkey = X509_get0_pubkey(x);
    if (!pkey)
        return 0;
     
    if (EVP_PKEY_id(pkey) != EVP_PKEY_EC)
        return 1;
    rv = tls1_set_ec_id(curve_id, &comp_id, EVP_PKEY_get0_EC_KEY(pkey));
    if (!rv)
        return 0;
     
    rv = tls1_check_ec_key(s, s->server ? curve_id : NULL, &comp_id);
    if (!rv)
        return 0;
     
    if (set_ee_md && tls1_suiteb(s)) {
        int check_md;
        size_t i;
        CERT *c = s->cert;
        if (curve_id[0])
            return 0;
         
        if (curve_id[1] == TLSEXT_curve_P_256)
            check_md = NID_ecdsa_with_SHA256;
        else if (curve_id[1] == TLSEXT_curve_P_384)
            check_md = NID_ecdsa_with_SHA384;
        else
            return 0;            
        for (i = 0; i < c->shared_sigalgslen; i++)
            if (check_md == c->shared_sigalgs[i].signandhash_nid)
                break;
        if (i == c->shared_sigalgslen)
            return 0;
        if (set_ee_md == 2) {
            if (check_md == NID_ecdsa_with_SHA256)
                s->s3->tmp.md[SSL_PKEY_ECC] = EVP_sha256();
            else
                s->s3->tmp.md[SSL_PKEY_ECC] = EVP_sha384();
        }
    }
    return rv;
}