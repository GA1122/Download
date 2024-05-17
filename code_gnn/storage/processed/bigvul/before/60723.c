client_create_dh(krb5_context context,
                 pkinit_plg_crypto_context plg_cryptoctx,
                 pkinit_req_crypto_context cryptoctx,
                 pkinit_identity_crypto_context id_cryptoctx,
                 int dh_size,
                 unsigned char **dh_params,
                 unsigned int *dh_params_len,
                 unsigned char **dh_pubkey,
                 unsigned int *dh_pubkey_len)
{
    krb5_error_code retval = KRB5KDC_ERR_PREAUTH_FAILED;
    unsigned char *buf = NULL;
    int dh_err = 0;
    ASN1_INTEGER *pub_key = NULL;
    const BIGNUM *pubkey_bn, *p, *q, *g;

    if (cryptoctx->dh == NULL) {
        if (dh_size == 1024)
            cryptoctx->dh = make_oakley_dh(oakley_1024, sizeof(oakley_1024));
        else if (dh_size == 2048)
            cryptoctx->dh = make_oakley_dh(oakley_2048, sizeof(oakley_2048));
        else if (dh_size == 4096)
            cryptoctx->dh = make_oakley_dh(oakley_4096, sizeof(oakley_4096));
        if (cryptoctx->dh == NULL)
            goto cleanup;
    }

    DH_generate_key(cryptoctx->dh);
    DH_get0_key(cryptoctx->dh, &pubkey_bn, NULL);

    DH_check(cryptoctx->dh, &dh_err);
    if (dh_err != 0) {
        pkiDebug("Warning: dh_check failed with %d\n", dh_err);
        if (dh_err & DH_CHECK_P_NOT_PRIME)
            pkiDebug("p value is not prime\n");
        if (dh_err & DH_CHECK_P_NOT_SAFE_PRIME)
            pkiDebug("p value is not a safe prime\n");
        if (dh_err & DH_UNABLE_TO_CHECK_GENERATOR)
            pkiDebug("unable to check the generator value\n");
        if (dh_err & DH_NOT_SUITABLE_GENERATOR)
            pkiDebug("the g value is not a generator\n");
    }
#ifdef DEBUG_DH
    print_dh(cryptoctx->dh, "client's DH params\n");
    print_pubkey(cryptoctx->dh->pub_key, "client's pub_key=");
#endif

    DH_check_pub_key(cryptoctx->dh, pubkey_bn, &dh_err);
    if (dh_err != 0) {
        pkiDebug("dh_check_pub_key failed with %d\n", dh_err);
        goto cleanup;
    }

     
     
    DH_get0_pqg(cryptoctx->dh, &p, &q, &g);
    retval = pkinit_encode_dh_params(p, g, q, dh_params, dh_params_len);
    if (retval)
        goto cleanup;

     
     
    pub_key = BN_to_ASN1_INTEGER(pubkey_bn, NULL);
    if (pub_key == NULL) {
        retval = ENOMEM;
        goto cleanup;
    }
    *dh_pubkey_len = i2d_ASN1_INTEGER(pub_key, NULL);
    if ((buf = *dh_pubkey = malloc(*dh_pubkey_len)) == NULL) {
        retval  = ENOMEM;
        goto cleanup;
    }
    i2d_ASN1_INTEGER(pub_key, &buf);

    if (pub_key != NULL)
        ASN1_INTEGER_free(pub_key);

    retval = 0;
    return retval;

cleanup:
    if (cryptoctx->dh != NULL)
        DH_free(cryptoctx->dh);
    cryptoctx->dh = NULL;
    free(*dh_params);
    *dh_params = NULL;
    free(*dh_pubkey);
    *dh_pubkey = NULL;
    if (pub_key != NULL)
        ASN1_INTEGER_free(pub_key);

    return retval;
}