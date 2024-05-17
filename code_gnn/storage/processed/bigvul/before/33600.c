find_alternate_tgs(krb5_kdc_req *request, krb5_db_entry **server_ptr)
{
    krb5_error_code retval;
    krb5_principal *plist = NULL, *pl2, tmpprinc;
    krb5_data tmp;
    krb5_db_entry *server = NULL;

    *server_ptr = NULL;

     
    if ((retval = krb5_walk_realm_tree(kdc_context,
                                       krb5_princ_realm(kdc_context, request->server),
                                       krb5_princ_component(kdc_context, request->server, 1),
                                       &plist, KRB5_REALM_BRANCH_CHAR)))
        return retval;

     
    for (pl2 = plist; *pl2; pl2++);

     
    while (--pl2 > plist) {
        tmp = *krb5_princ_realm(kdc_context, *pl2);
        krb5_princ_set_realm(kdc_context, *pl2,
                             krb5_princ_realm(kdc_context, tgs_server));
        retval = krb5_db_get_principal(kdc_context, *pl2, 0, &server);
        krb5_princ_set_realm(kdc_context, *pl2, &tmp);
        if (retval == KRB5_KDB_NOENTRY)
            continue;
        else if (retval)
            goto cleanup;

         
        tmp = *krb5_princ_realm(kdc_context, *pl2);
        krb5_princ_set_realm(kdc_context, *pl2,
                             krb5_princ_realm(kdc_context, tgs_server));
        retval = krb5_copy_principal(kdc_context, *pl2, &tmpprinc);
        if (retval)
            goto cleanup;
        krb5_princ_set_realm(kdc_context, *pl2, &tmp);

        krb5_free_principal(kdc_context, request->server);
        request->server = tmpprinc;
        log_tgs_alt_tgt(request->server);
        *server_ptr = server;
        server = NULL;
        goto cleanup;
    }
    retval = KRB5_KDB_NOENTRY;

cleanup:
    krb5_free_realm_tree(kdc_context, plist);
    krb5_db_free_principal(kdc_context, server);
    return retval;
}
