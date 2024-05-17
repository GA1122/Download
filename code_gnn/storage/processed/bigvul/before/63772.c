static int _sx_sasl_gsasl_callback(Gsasl *gsasl_ctx, Gsasl_session *sd, Gsasl_property prop) {
    _sx_sasl_sess_t sctx = gsasl_session_hook_get(sd);
    _sx_sasl_t ctx = NULL;
    struct sx_sasl_creds_st creds = {NULL, NULL, NULL, NULL};
    char *value, *node, *host;
    int len, i;

     
     if(sctx != NULL) {
         ctx = sctx->ctx;
     }

    _sx_debug(ZONE, "in _sx_sasl_gsasl_callback, property: %d", prop);

    switch(prop) {
        case GSASL_PASSWORD:
             
            assert(ctx);
            assert(ctx->cb);
            creds.authnid = gsasl_property_fast(sd, GSASL_AUTHID);
            creds.realm   = gsasl_property_fast(sd, GSASL_REALM);
            if(!creds.authnid) return GSASL_NO_AUTHID;
            if(!creds.realm) return GSASL_NO_AUTHZID;
            if((ctx->cb)(sx_sasl_cb_GET_PASS, &creds, (void **)&value, sctx->s, ctx->cbarg) == sx_sasl_ret_OK) {
                gsasl_property_set(sd, GSASL_PASSWORD, value);
            }
            return GSASL_NEEDS_MORE;

        case GSASL_SERVICE:
            gsasl_property_set(sd, GSASL_SERVICE, "xmpp");
            return GSASL_OK;

        case GSASL_HOSTNAME:
            {
                char hostname[256];
                 
                hostname[0] = '\0';
                gethostname(hostname, 256);
                hostname[255] = '\0';

                gsasl_property_set(sd, GSASL_HOSTNAME, hostname);
           }
           return GSASL_OK;

        case GSASL_VALIDATE_SIMPLE:
             
            assert(ctx);
            assert(ctx->cb);
            creds.authnid = gsasl_property_fast(sd, GSASL_AUTHID);
            creds.realm   = gsasl_property_fast(sd, GSASL_REALM);
            creds.pass    = gsasl_property_fast(sd, GSASL_PASSWORD);
            if(!creds.authnid) return GSASL_NO_AUTHID;
            if(!creds.realm) return GSASL_NO_AUTHZID;
            if(!creds.pass) return GSASL_NO_PASSWORD;
            if((ctx->cb)(sx_sasl_cb_CHECK_PASS, &creds, NULL, sctx->s, ctx->cbarg) == sx_sasl_ret_OK)
                return GSASL_OK;
            else
                return GSASL_AUTHENTICATION_ERROR;

        case GSASL_VALIDATE_GSSAPI:
             
            creds.authnid = gsasl_property_fast(sd, GSASL_GSSAPI_DISPLAY_NAME);
            if(!creds.authnid) return GSASL_NO_AUTHID;
            creds.authzid = gsasl_property_fast(sd, GSASL_AUTHZID);
            if(!creds.authzid) return GSASL_NO_AUTHZID;
            gsasl_property_set(sd, GSASL_AUTHID, creds.authnid);
            return GSASL_OK;

        case GSASL_VALIDATE_ANONYMOUS:
             
            creds.authnid = gsasl_property_fast(sd, GSASL_ANONYMOUS_TOKEN);
            if(!creds.authnid) return GSASL_NO_ANONYMOUS_TOKEN;
             
            gsasl_property_set(sd, GSASL_AUTHID, creds.authnid);
            return GSASL_OK;

        case GSASL_VALIDATE_EXTERNAL:
             
            assert(ctx);
            assert(ctx->ext_id);
            creds.authzid = gsasl_property_fast(sd, GSASL_AUTHZID);
            _sx_debug(ZONE, "sasl external");
            _sx_debug(ZONE, "sasl creds.authzid is '%s'", creds.authzid);

            for (i = 0; i < SX_CONN_EXTERNAL_ID_MAX_COUNT; i++) {
                if (ctx->ext_id[i] == NULL)
                    break;
                _sx_debug(ZONE, "sasl ext_id(%d) is '%s'", i, ctx->ext_id[i]);
                 
                value = strstr(ctx->ext_id[i], "@");

                if(value == NULL && creds.authzid != NULL && strcmp(ctx->ext_id[i], creds.authzid) == 0) {
                     
                    _sx_debug(ZONE, "sasl ctx->ext_id doesn't have '@' in it. Assuming s2s");
                    return GSASL_OK;
                }
                if(value != NULL &&
                    ((creds.authzid != NULL && strcmp(ctx->ext_id[i], creds.authzid) == 0) ||
                     (creds.authzid == NULL)) ) {

                    len = value - ctx->ext_id[i];
                    node = (char *) malloc(sizeof(char) * (len + 1));  
                    strncpy(node, ctx->ext_id[i], len);
                    node[len] = '\0';  
                    len = strlen(value) - 1 + 1;  
                    host = (char *) malloc(sizeof(char) * (len));
                    strcpy(host, value + 1);  
                    gsasl_property_set(sd, GSASL_AUTHID, node);
                    gsasl_property_set(sd, GSASL_REALM, host);
                    return GSASL_OK;
                }

            }
            return GSASL_AUTHENTICATION_ERROR;

        default:
            break;
    }

    return GSASL_NO_CALLBACK;
}
