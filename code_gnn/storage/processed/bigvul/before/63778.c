static void _sx_sasl_server_process(sx_t s, sx_plugin_t p, Gsasl_session *sd, const char *in, int inlen) {
    char *buf = NULL, *out = NULL;
    size_t buflen, outlen;
    int ret;

    _sx_debug(ZONE, "data from client");

     
    ret = gsasl_base64_from(in, inlen, &buf, &buflen);

    if (ret == GSASL_OK) {
        _sx_debug(ZONE, "decoded data: %.*s", buflen, buf);

         
        ret = gsasl_step(sd, buf, buflen, &out, &outlen);
        if(buf != NULL) free(buf);
        buf = NULL;

         
        if(ret == GSASL_OK || ret == GSASL_NEEDS_MORE) {
            _sx_debug(ZONE, "sasl handshake in progress (response: %.*s)", outlen, out);

             
            ret = gsasl_base64_to(out, outlen, &buf, &buflen);

            if (ret == GSASL_OK) {
                _sx_nad_write(s, _sx_sasl_response(s, buf, buflen), 0);
            }

            if(out != NULL) free(out);
            if(buf != NULL) free(buf);

            return;
        }
    }
    if(out != NULL) free(out);
    if(buf != NULL) free(buf);

     
    _sx_debug(ZONE, "sasl handshake aborted; (%d): %s", ret, gsasl_strerror(ret));

    _sx_nad_write(s, _sx_sasl_abort(s), 0);
}