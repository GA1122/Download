X509_CRL *X509_CRL_diff(X509_CRL *base, X509_CRL *newer,
                        EVP_PKEY *skey, const EVP_MD *md, unsigned int flags)
{
    X509_CRL *crl = NULL;
    int i;
    STACK_OF(X509_REVOKED) *revs = NULL;
     
    if (base->base_crl_number || newer->base_crl_number) {
        X509err(X509_F_X509_CRL_DIFF, X509_R_CRL_ALREADY_DELTA);
        return NULL;
    }
     
    if (!base->crl_number || !newer->crl_number) {
        X509err(X509_F_X509_CRL_DIFF, X509_R_NO_CRL_NUMBER);
        return NULL;
    }
     
    if (X509_NAME_cmp(X509_CRL_get_issuer(base), X509_CRL_get_issuer(newer))) {
        X509err(X509_F_X509_CRL_DIFF, X509_R_ISSUER_MISMATCH);
        return NULL;
    }
     
    if (!crl_extension_match(base, newer, NID_authority_key_identifier)) {
        X509err(X509_F_X509_CRL_DIFF, X509_R_AKID_MISMATCH);
        return NULL;
    }
    if (!crl_extension_match(base, newer, NID_issuing_distribution_point)) {
        X509err(X509_F_X509_CRL_DIFF, X509_R_IDP_MISMATCH);
        return NULL;
    }
     
    if (ASN1_INTEGER_cmp(newer->crl_number, base->crl_number) <= 0) {
        X509err(X509_F_X509_CRL_DIFF, X509_R_NEWER_CRL_NOT_NEWER);
        return NULL;
    }
     
    if (skey && (X509_CRL_verify(base, skey) <= 0 ||
                 X509_CRL_verify(newer, skey) <= 0)) {
        X509err(X509_F_X509_CRL_DIFF, X509_R_CRL_VERIFY_FAILURE);
        return NULL;
    }
     
    crl = X509_CRL_new();
    if (!crl || !X509_CRL_set_version(crl, 1))
        goto memerr;
     
    if (!X509_CRL_set_issuer_name(crl, X509_CRL_get_issuer(newer)))
        goto memerr;

    if (!X509_CRL_set_lastUpdate(crl, X509_CRL_get_lastUpdate(newer)))
        goto memerr;
    if (!X509_CRL_set_nextUpdate(crl, X509_CRL_get_nextUpdate(newer)))
        goto memerr;

     

    if (!X509_CRL_add1_ext_i2d(crl, NID_delta_crl, base->crl_number, 1, 0))
        goto memerr;

     

    for (i = 0; i < X509_CRL_get_ext_count(newer); i++) {
        X509_EXTENSION *ext;
        ext = X509_CRL_get_ext(newer, i);
        if (!X509_CRL_add_ext(crl, ext, -1))
            goto memerr;
    }

     

    revs = X509_CRL_get_REVOKED(newer);

    for (i = 0; i < sk_X509_REVOKED_num(revs); i++) {
        X509_REVOKED *rvn, *rvtmp;
        rvn = sk_X509_REVOKED_value(revs, i);
         
        if (!X509_CRL_get0_by_serial(base, &rvtmp, rvn->serialNumber)) {
            rvtmp = X509_REVOKED_dup(rvn);
            if (!rvtmp)
                goto memerr;
            if (!X509_CRL_add0_revoked(crl, rvtmp)) {
                X509_REVOKED_free(rvtmp);
                goto memerr;
            }
        }
    }
     

    if (skey && md && !X509_CRL_sign(crl, skey, md))
        goto memerr;

    return crl;

 memerr:
    X509err(X509_F_X509_CRL_DIFF, ERR_R_MALLOC_FAILURE);
    X509_CRL_free(crl);
    return NULL;
}