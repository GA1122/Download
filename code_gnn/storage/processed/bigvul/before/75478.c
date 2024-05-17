kdc_make_s4u2self_rep(krb5_context context,
                      krb5_keyblock *tgs_subkey,
                      krb5_keyblock *tgs_session,
                      krb5_pa_s4u_x509_user *req_s4u_user,
                      krb5_kdc_rep *reply,
                      krb5_enc_kdc_rep_part *reply_encpart)
{
    krb5_error_code             code;
    krb5_data                   *der_user_id = NULL, *der_s4u_x509_user = NULL;
    krb5_pa_s4u_x509_user       rep_s4u_user;
    krb5_pa_data                *pa;
    krb5_enctype                enctype;
    krb5_keyusage               usage;

    memset(&rep_s4u_user, 0, sizeof(rep_s4u_user));

    rep_s4u_user.user_id.nonce   = req_s4u_user->user_id.nonce;
    rep_s4u_user.user_id.user    = req_s4u_user->user_id.user;
    rep_s4u_user.user_id.options =
        req_s4u_user->user_id.options & KRB5_S4U_OPTS_USE_REPLY_KEY_USAGE;

    code = encode_krb5_s4u_userid(&rep_s4u_user.user_id, &der_user_id);
    if (code != 0)
        goto cleanup;

    if (req_s4u_user->user_id.options & KRB5_S4U_OPTS_USE_REPLY_KEY_USAGE)
        usage = KRB5_KEYUSAGE_PA_S4U_X509_USER_REPLY;
    else
        usage = KRB5_KEYUSAGE_PA_S4U_X509_USER_REQUEST;

    code = krb5_c_make_checksum(context, req_s4u_user->cksum.checksum_type,
                                tgs_subkey != NULL ? tgs_subkey : tgs_session,
                                usage, der_user_id, &rep_s4u_user.cksum);
    if (code != 0)
        goto cleanup;

    code = encode_krb5_pa_s4u_x509_user(&rep_s4u_user, &der_s4u_x509_user);
    if (code != 0)
        goto cleanup;

     
    code = alloc_pa_data(KRB5_PADATA_S4U_X509_USER, 0, &pa);
    if (code != 0)
        goto cleanup;
    pa->length = der_s4u_x509_user->length;
    pa->contents = (uint8_t *)der_s4u_x509_user->data;
    der_s4u_x509_user->data = NULL;
     
    code = add_pa_data_element(&reply->padata, pa);
    if (code != 0)
        goto cleanup;

    if (tgs_subkey != NULL)
        enctype = tgs_subkey->enctype;
    else
        enctype = tgs_session->enctype;

     
    if ((req_s4u_user->user_id.options & KRB5_S4U_OPTS_USE_REPLY_KEY_USAGE) &&
        enctype_requires_etype_info_2(enctype) == FALSE) {
        code = alloc_pa_data(KRB5_PADATA_S4U_X509_USER,
                             req_s4u_user->cksum.length +
                             rep_s4u_user.cksum.length, &pa);
        if (code != 0)
            goto cleanup;
        memcpy(pa->contents,
               req_s4u_user->cksum.contents, req_s4u_user->cksum.length);
        memcpy(&pa->contents[req_s4u_user->cksum.length],
               rep_s4u_user.cksum.contents, rep_s4u_user.cksum.length);

         
        code = add_pa_data_element(&reply_encpart->enc_padata, pa);
        if (code != 0)
            goto cleanup;
    }

cleanup:
    if (rep_s4u_user.cksum.contents != NULL)
        krb5_free_checksum_contents(context, &rep_s4u_user.cksum);
    krb5_free_data(context, der_user_id);
    krb5_free_data(context, der_s4u_x509_user);

    return code;
}