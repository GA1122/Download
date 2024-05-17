gen_session_key(kdc_realm_t *kdc_active_realm, krb5_kdc_req *req,
                krb5_db_entry *server, krb5_keyblock *skey,
                const char **status)
{
    krb5_error_code retval;
    krb5_enctype useenctype = 0;

     
    if (req->kdc_options & KDC_OPT_ENC_TKT_IN_SKEY) {
        retval = get_2ndtkt_enctype(kdc_active_realm, req, &useenctype,
                                    status);
        if (retval != 0)
            goto cleanup;
    }
    if (useenctype == 0) {
        useenctype = select_session_keytype(kdc_active_realm, server,
                                            req->nktypes,
                                            req->ktype);
    }
    if (useenctype == 0) {
         
        *status = "BAD_ENCRYPTION_TYPE";
        retval = KRB5KDC_ERR_ETYPE_NOSUPP;
        goto cleanup;
    }
    retval = krb5_c_make_random_key(kdc_context, useenctype, skey);
    if (retval != 0) {
         
        *status = "MAKE_RANDOM_KEY";
        goto cleanup;
    }
cleanup:
    return retval;
}
