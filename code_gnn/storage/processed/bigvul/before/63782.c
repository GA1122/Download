static int _sx_sasl_wio(sx_t s, sx_plugin_t p, sx_buf_t buf) {
    sx_error_t sxe;
    size_t len;
    int ret;
    char *out;
    Gsasl_session *sd = (Gsasl_session *) s->plugin_data[p->index];

    _sx_debug(ZONE, "doing sasl encode");

     
    ret = gsasl_encode(sd, buf->data, buf->len, &out, &len);
    if (ret != GSASL_OK) {
        _sx_debug(ZONE, "gsasl_encode failed (%d): %s", ret, gsasl_strerror (ret));
         
        _sx_gen_error(sxe, SX_ERR_AUTH, "SASL Stream encoding failed", (char*) gsasl_strerror (ret));
        _sx_event(s, event_ERROR, (void *) &sxe);
        return -1;
    }

     
    _sx_buffer_set(buf, out, len, NULL);
    free(out);

    _sx_debug(ZONE, "%d bytes encoded for sasl channel", buf->len);

    return 1;
}