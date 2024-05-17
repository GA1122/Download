build_pa_s4u_x509_user(krb5_context context,
                       krb5_keyblock *subkey,
                       krb5_kdc_req *tgsreq,
                       void *gcvt_data)
{
    krb5_error_code code;
    krb5_pa_s4u_x509_user *s4u_user = (krb5_pa_s4u_x509_user *)gcvt_data;
    krb5_data *data = NULL;
    krb5_pa_data **padata;
    krb5_cksumtype cksumtype;
    int i;

    assert(s4u_user->cksum.contents == NULL);

    s4u_user->user_id.nonce = tgsreq->nonce;

    code = encode_krb5_s4u_userid(&s4u_user->user_id, &data);
    if (code != 0)
        goto cleanup;

     
    if (subkey->enctype == ENCTYPE_ARCFOUR_HMAC ||
        subkey->enctype == ENCTYPE_ARCFOUR_HMAC_EXP) {
        cksumtype = CKSUMTYPE_RSA_MD4;
    } else {
        code = krb5int_c_mandatory_cksumtype(context, subkey->enctype,
                                             &cksumtype);
    }
    if (code != 0)
        goto cleanup;

    code = krb5_c_make_checksum(context, cksumtype, subkey,
                                KRB5_KEYUSAGE_PA_S4U_X509_USER_REQUEST, data,
                                &s4u_user->cksum);
    if (code != 0)
        goto cleanup;

    krb5_free_data(context, data);
    data = NULL;

    code = encode_krb5_pa_s4u_x509_user(s4u_user, &data);
    if (code != 0)
        goto cleanup;

    assert(tgsreq->padata != NULL);

    for (i = 0; tgsreq->padata[i] != NULL; i++)
        ;

    padata = realloc(tgsreq->padata,
                     (i + 2) * sizeof(krb5_pa_data *));
    if (padata == NULL) {
        code = ENOMEM;
        goto cleanup;
    }
    tgsreq->padata = padata;

    padata[i] = malloc(sizeof(krb5_pa_data));
    if (padata[i] == NULL) {
        code = ENOMEM;
        goto cleanup;
    }
    padata[i]->magic = KV5M_PA_DATA;
    padata[i]->pa_type = KRB5_PADATA_S4U_X509_USER;
    padata[i]->length = data->length;
    padata[i]->contents = (krb5_octet *)data->data;

    padata[i + 1] = NULL;

    free(data);
    data = NULL;

cleanup:
    if (code != 0 && s4u_user->cksum.contents != NULL) {
        krb5_free_checksum_contents(context, &s4u_user->cksum);
        s4u_user->cksum.contents = NULL;
    }
    krb5_free_data(context, data);

    return code;
}