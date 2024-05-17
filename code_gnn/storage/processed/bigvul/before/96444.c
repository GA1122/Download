authorize_cert(krb5_context context, certauth_handle *certauth_modules,
               pkinit_kdc_context plgctx, pkinit_kdc_req_context reqctx,
               krb5_kdcpreauth_callbacks cb, krb5_kdcpreauth_rock rock,
               krb5_principal client)
{
    krb5_error_code ret;
    certauth_handle h;
    struct certauth_req_opts opts;
    krb5_boolean accepted = FALSE;
    uint8_t *cert;
    size_t i, cert_len;
    void *db_ent = NULL;
    char **ais = NULL, **ai = NULL;

     
    ret = crypto_encode_der_cert(context, reqctx->cryptoctx, &cert, &cert_len);
    if (ret)
        goto cleanup;

     
    opts.plgctx = plgctx;
    opts.reqctx = reqctx;
    opts.cb = cb;
    opts.rock = rock;

    db_ent = cb->client_entry(context, rock);

     
    ret = KRB5_PLUGIN_NO_HANDLE;
    for (i = 0; certauth_modules != NULL && certauth_modules[i] != NULL; i++) {
        h = certauth_modules[i];
        TRACE_PKINIT_SERVER_CERT_AUTH(context, h->vt.name);
        ret = h->vt.authorize(context, h->moddata, cert, cert_len, client,
                              &opts, db_ent, &ais);
        if (ret == 0)
            accepted = TRUE;
        else if (ret != KRB5_PLUGIN_NO_HANDLE)
            goto cleanup;

        if (ais != NULL) {
             
            for (ai = ais; *ai != NULL; ai++) {
                ret = cb->add_auth_indicator(context, rock, *ai);
                if (ret)
                    goto cleanup;
            }
            h->vt.free_ind(context, h->moddata, ais);
            ais = NULL;
        }
    }

    ret = accepted ? 0 : KRB5KDC_ERR_CLIENT_NAME_MISMATCH;

cleanup:
    free(cert);
    return ret;
}