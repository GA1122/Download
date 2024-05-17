static void basic_http_header_check(request_rec *r,
                                    const char **protocol)
{
    apr_status_t rv;

    if (r->assbackwards) {
         
        return;
    }

    rv = validate_status_line(r);

    if (!r->status_line) {
        r->status_line = ap_get_status_line(r->status);
    } else if (rv != APR_SUCCESS) {
         
        const char *tmp = ap_get_status_line(r->status);
        if (!strncmp(tmp, r->status_line, 3)) {
            r->status_line = tmp;
        }
    }

     
    if (r->proto_num > HTTP_VERSION(1,0)
        && apr_table_get(r->subprocess_env, "downgrade-1.0")) {
        r->proto_num = HTTP_VERSION(1,0);
    }

     
    if (r->proto_num == HTTP_VERSION(1,0)
        && apr_table_get(r->subprocess_env, "force-response-1.0")) {
        *protocol = "HTTP/1.0";
        r->connection->keepalive = AP_CONN_CLOSE;
    }
    else {
        *protocol = AP_SERVER_PROTOCOL;
    }

}
