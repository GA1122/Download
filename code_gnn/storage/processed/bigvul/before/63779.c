static void _sx_sasl_stream(sx_t s, sx_plugin_t p) {
    Gsasl_session *sd = (Gsasl_session *) s->plugin_data[p->index];

     
    if(sd == NULL)
        return;

     
    if(NULL == gsasl_property_fast(sd, GSASL_AUTHID)) {
        _sx_debug(ZONE, "not auth'd, not advancing to auth'd state yet");
        return;
    }

     
    _sx_sasl_open(s, sd);
}
