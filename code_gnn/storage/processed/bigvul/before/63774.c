void _sx_sasl_open(sx_t s, Gsasl_session *sd) {
    char *method, *authzid;
    const char *realm = NULL;
    struct sx_sasl_creds_st creds = {NULL, NULL, NULL, NULL};
    _sx_sasl_sess_t sctx = gsasl_session_hook_get(sd);
    _sx_sasl_t ctx = sctx->ctx;
    const char *mechname = gsasl_mechanism_name (sd);

     
    method = (char *) malloc(sizeof(char) * (strlen(mechname) + 6));
    sprintf(method, "SASL/%s", mechname);

     
    creds.authzid = gsasl_property_fast(sd, GSASL_AUTHZID);
    creds.authnid = gsasl_property_fast(sd, GSASL_AUTHID);
    creds.realm   = gsasl_property_fast(sd, GSASL_REALM);

    if(0 && ctx && ctx->cb) {  
        if((ctx->cb)(sx_sasl_cb_CHECK_AUTHZID, &creds, NULL, s, ctx->cbarg)!=sx_sasl_ret_OK) {
            _sx_debug(ZONE, "stream authzid: %s verification failed, not advancing to auth state", creds.authzid);
            free(method);
            return;
        }
    } else if (NULL != gsasl_property_fast(sd, GSASL_GSSAPI_DISPLAY_NAME)) {
        creds.authzid = strdup(gsasl_property_fast(sd, GSASL_GSSAPI_DISPLAY_NAME));
        authzid = NULL;
    } else {
         
        if(creds.realm && creds.realm[0] != '\0') {
            realm = creds.realm;
        } else {
            realm = s->req_to;
        }
        authzid = (char *) malloc(sizeof(char) * (strlen(creds.authnid) + strlen(realm) + 2));
        sprintf(authzid, "%s@%s", creds.authnid, realm);
        creds.authzid = authzid;
    }

     
    sx_auth(s, method, creds.authzid);

    free(method);
    if(authzid) free(authzid);
}