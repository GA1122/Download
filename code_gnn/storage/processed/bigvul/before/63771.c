static void _sx_sasl_free(sx_t s, sx_plugin_t p) {
    Gsasl_session *sd = (Gsasl_session *) s->plugin_data[p->index];
    _sx_sasl_sess_t sctx;

    if(sd == NULL)
        return;

    _sx_debug(ZONE, "cleaning up conn state");

     
    sctx = gsasl_session_hook_get(sd);
    if (sctx != NULL){
        free(sctx);
        gsasl_session_hook_set(sd, (void *) NULL);
    }

    gsasl_finish(sd);
    s->plugin_data[p->index] = NULL;
}