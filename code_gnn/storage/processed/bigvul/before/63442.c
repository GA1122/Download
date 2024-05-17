decrypt_2ndtkt(kdc_realm_t *kdc_active_realm, krb5_kdc_req *req,
               krb5_flags flags, krb5_db_entry **server_out,
               const char **status)
{
    krb5_error_code retval;
    krb5_db_entry *server = NULL;
    krb5_keyblock *key;
    krb5_kvno kvno;
    krb5_ticket *stkt;

    if (!(req->kdc_options & STKT_OPTIONS))
        return 0;

    stkt = req->second_ticket[0];
    retval = kdc_get_server_key(kdc_context, stkt,
                                flags,
                                TRUE,  
                                &server,
                                &key,
                                &kvno);
    if (retval != 0) {
        *status = "2ND_TKT_SERVER";
        goto cleanup;
    }
    retval = krb5_decrypt_tkt_part(kdc_context, key,
                                   req->second_ticket[0]);
    krb5_free_keyblock(kdc_context, key);
    if (retval != 0) {
        *status = "2ND_TKT_DECRYPT";
        goto cleanup;
    }
    *server_out = server;
    server = NULL;
cleanup:
    krb5_db_free_principal(kdc_context, server);
    return retval;
}