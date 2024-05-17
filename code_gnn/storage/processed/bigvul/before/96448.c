dbmatch_authorize(krb5_context context, krb5_certauth_moddata moddata,
                  const uint8_t *cert, size_t cert_len,
                  krb5_const_principal princ, const void *opts,
                  const struct _krb5_db_entry_new *db_entry,
                  char ***authinds_out)
{
    krb5_error_code ret;
    const struct certauth_req_opts *req_opts = opts;
    char *pattern;
    krb5_boolean matched;

    *authinds_out = NULL;

     
    ret = req_opts->cb->get_string(context, req_opts->rock,
                                   "pkinit_cert_match", &pattern);
    if (ret)
        return ret;
    if (pattern == NULL)
        return KRB5_PLUGIN_NO_HANDLE;

     
    ret = pkinit_client_cert_match(context, req_opts->plgctx->cryptoctx,
                                   req_opts->reqctx->cryptoctx, pattern,
                                   &matched);
    req_opts->cb->free_string(context, req_opts->rock, pattern);
    if (ret)
        return ret;
    return matched ? 0 : KRB5KDC_ERR_CERTIFICATE_MISMATCH;
}
