static void _sx_sasl_features(sx_t s, sx_plugin_t p, nad_t nad) {
    _sx_sasl_t ctx = (_sx_sasl_t) p->private;
    Gsasl_session *sd = (Gsasl_session *) s->plugin_data[p->index];
    int nmechs, ret;
    char *mechs, *mech, *c;

    if(s->type != type_SERVER)
        return;

    if(sd != NULL) {
        _sx_debug(ZONE, "already auth'd, not offering sasl mechanisms");
        return;
    }

    if(!(s->flags & SX_SASL_OFFER)) {
        _sx_debug(ZONE, "application didn't ask us to offer sasl, so we won't");
        return;
    }

#ifdef HAVE_SSL
    if((s->flags & SX_SSL_STARTTLS_REQUIRE) && s->ssf == 0) {
        _sx_debug(ZONE, "ssl not established yet but the app requires it, not offering mechanisms");
        return;
    }
#endif

    _sx_debug(ZONE, "offering sasl mechanisms");

    ret = gsasl_server_mechlist(ctx->gsasl_ctx, &mechs);
    if(ret != GSASL_OK) {
        _sx_debug(ZONE, "gsasl_server_mechlist failed (%d): %s, not offering sasl for this conn", ret, gsasl_strerror (ret));
        return;
    }

    mech = mechs;
    nmechs = 0;
    while(mech != NULL) {
        c = strchr(mech, ' ');
        if(c != NULL)
            *c = '\0';

        if ((ctx->cb)(sx_sasl_cb_CHECK_MECH, mech, NULL, s, ctx->cbarg)==sx_sasl_ret_OK) {
            if (nmechs == 0) {
                int ns = nad_add_namespace(nad, uri_SASL, NULL);
                nad_append_elem(nad, ns, "mechanisms", 1);
            }
            _sx_debug(ZONE, "offering mechanism: %s", mech);

            nad_append_elem(nad, -1  , "mechanism", 2);
            nad_append_cdata(nad, mech, strlen(mech), 3);
            nmechs++;
        }

        if(c == NULL)
            mech = NULL;
        else
            mech = ++c;
    }

    free(mechs);
}