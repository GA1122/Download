prepare_error_as(struct kdc_request_state *rstate, krb5_kdc_req *request,
                 krb5_db_entry *local_tgt, int error, krb5_pa_data **e_data_in,
                 krb5_boolean typed_e_data, krb5_principal canon_client,
                 krb5_data **response, const char *status)
{
    krb5_error errpkt;
    krb5_error_code retval;
    krb5_data *scratch = NULL, *e_data_asn1 = NULL, *fast_edata = NULL;
    krb5_pa_data **e_data = NULL, *cookie = NULL;
    kdc_realm_t *kdc_active_realm = rstate->realm_data;
    size_t count;

    errpkt.magic = KV5M_ERROR;

    if (e_data_in != NULL) {
         
        for (count = 0; e_data_in[count] != NULL; count++);
        e_data = calloc(count + 2, sizeof(*e_data));
        if (e_data == NULL)
            return ENOMEM;
        memcpy(e_data, e_data_in, count * sizeof(*e_data));
        retval = kdc_fast_make_cookie(kdc_context, rstate, local_tgt,
                                      request->client, &cookie);
        e_data[count] = cookie;
    }

    errpkt.ctime = request->nonce;
    errpkt.cusec = 0;

    retval = krb5_us_timeofday(kdc_context, &errpkt.stime, &errpkt.susec);
    if (retval)
        goto cleanup;
    errpkt.error = error;
    errpkt.server = request->server;
    errpkt.client = (error == KDC_ERR_WRONG_REALM) ? canon_client :
        request->client;
    errpkt.text = string2data((char *)status);

    if (e_data != NULL) {
        if (typed_e_data)
            retval = encode_krb5_typed_data(e_data, &e_data_asn1);
        else
            retval = encode_krb5_padata_sequence(e_data, &e_data_asn1);
        if (retval)
            goto cleanup;
        errpkt.e_data = *e_data_asn1;
    } else
        errpkt.e_data = empty_data();

    retval = kdc_fast_handle_error(kdc_context, rstate, request, e_data,
                                   &errpkt, &fast_edata);
    if (retval)
        goto cleanup;
    if (fast_edata != NULL)
        errpkt.e_data = *fast_edata;

    scratch = k5alloc(sizeof(*scratch), &retval);
    if (scratch == NULL)
        goto cleanup;
    if (kdc_fast_hide_client(rstate) && errpkt.client != NULL)
        errpkt.client = (krb5_principal)krb5_anonymous_principal();
    retval = krb5_mk_error(kdc_context, &errpkt, scratch);
    if (retval)
        goto cleanup;

    *response = scratch;
    scratch = NULL;

cleanup:
    krb5_free_data(kdc_context, fast_edata);
    krb5_free_data(kdc_context, e_data_asn1);
    free(scratch);
    free(e_data);
    if (cookie != NULL)
        free(cookie->contents);
    free(cookie);
    return retval;
}