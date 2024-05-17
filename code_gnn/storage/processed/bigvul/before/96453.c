pkinit_san_authorize(krb5_context context, krb5_certauth_moddata moddata,
                     const uint8_t *cert, size_t cert_len,
                     krb5_const_principal princ, const void *opts,
                     const struct _krb5_db_entry_new *db_entry,
                     char ***authinds_out)
{
    krb5_error_code ret;
    int valid_san;
    const struct certauth_req_opts *req_opts = opts;

    *authinds_out = NULL;

    ret = verify_client_san(context, req_opts->plgctx, req_opts->reqctx,
                            req_opts->cb, req_opts->rock, princ, &valid_san);
    if (ret == ENOENT)
        return KRB5_PLUGIN_NO_HANDLE;
    else if (ret)
        return ret;

    if (!valid_san) {
        TRACE_PKINIT_SERVER_SAN_REJECT(context);
        return KRB5KDC_ERR_CLIENT_NAME_MISMATCH;
    }

    return 0;
}
