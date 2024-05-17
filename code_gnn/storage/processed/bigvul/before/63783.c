int sx_sasl_auth(sx_plugin_t p, sx_t s, const char *appname, const char *mech, const char *user, const char *pass) {
    _sx_sasl_t ctx = (_sx_sasl_t) p->private;
    _sx_sasl_sess_t sctx = NULL;
    Gsasl_session *sd;
    char *buf = NULL, *out = NULL;
    char hostname[256];
    int ret, ns;
    size_t buflen, outlen;
    nad_t nad;

    assert((p != NULL));
    assert((s != NULL));
    assert((appname != NULL));
    assert((mech != NULL));
    assert((user != NULL));
    assert((pass != NULL));

    if(s->type != type_CLIENT || s->state != state_STREAM) {
        _sx_debug(ZONE, "need client in stream state for sasl auth");
        return 1;
     }

     
    ret = gsasl_client_start(ctx->gsasl_ctx, mech, &sd);
    if(ret != GSASL_OK) {
        _sx_debug(ZONE, "gsasl_client_start failed, not authing; (%d): %s", ret, gsasl_strerror(ret));

        return 1;
    }

     
    hostname[0] = '\0';
    gethostname(hostname, 256);
    hostname[255] = '\0';

     
    sctx = gsasl_session_hook_get(sd);
    if (sctx != NULL) free(sctx);

     
    sctx = (_sx_sasl_sess_t) calloc(1, sizeof(struct _sx_sasl_sess_st));
    sctx->s = s;
    sctx->ctx = ctx;

     
    gsasl_session_hook_set(sd, (void *) sctx);
    gsasl_property_set(sd, GSASL_AUTHID, user);
    gsasl_property_set(sd, GSASL_PASSWORD, pass);
    gsasl_property_set(sd, GSASL_SERVICE, appname);
    gsasl_property_set(sd, GSASL_HOSTNAME, hostname);

     
    ret = gsasl_step(sd, NULL, 0, &out, &outlen);
    if(ret != GSASL_OK && ret != GSASL_NEEDS_MORE) {
        _sx_debug(ZONE, "gsasl_step failed, not authing; (%d): %s", ret, gsasl_strerror(ret));

        gsasl_finish(sd);

        return 1;
    }

     
    s->plugin_data[p->index] = (void *) sd;

     
    _sx_debug(ZONE, "sending auth request to server, mech '%s': %.*s", mech, outlen, out);

     
    ret = gsasl_base64_to(out, outlen, &buf, &buflen);
    if(ret != GSASL_OK) {
        _sx_debug(ZONE, "gsasl_base64_to failed, not authing; (%d): %s", ret, gsasl_strerror(ret));

        gsasl_finish(sd);

        if (out != NULL) free(out);
        return 1;
    }
    free(out);

     
    nad = nad_new();
    ns = nad_add_namespace(nad, uri_SASL, NULL);

    nad_append_elem(nad, ns, "auth", 0);
    nad_append_attr(nad, -1, "mechanism", mech);
    if(buf != NULL) {
        nad_append_cdata(nad, buf, buflen, 1);
        free(buf);
    }

     
    sx_nad_write(s, nad);

    return 0;
}