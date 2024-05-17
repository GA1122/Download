AP_DECLARE_NONSTD(int) ap_send_http_trace(request_rec *r)
{
    core_server_config *conf;
    int rv;
    apr_bucket_brigade *bb;
    header_struct h;
    apr_bucket *b;
    int body;
    char *bodyread = NULL, *bodyoff;
    apr_size_t bodylen = 0;
    apr_size_t bodybuf;
    long res = -1;  

    if (r->method_number != M_TRACE) {
        return DECLINED;
    }

     
    while (r->prev) {
        r = r->prev;
    }
    conf = ap_get_core_module_config(r->server->module_config);

    if (conf->trace_enable == AP_TRACE_DISABLE) {
        apr_table_setn(r->notes, "error-notes",
                      "TRACE denied by server configuration");
        return HTTP_METHOD_NOT_ALLOWED;
    }

    if (conf->trace_enable == AP_TRACE_EXTENDED)
         
        body = REQUEST_CHUNKED_DECHUNK;
    else
        body = REQUEST_NO_BODY;

    if ((rv = ap_setup_client_block(r, body))) {
        if (rv == HTTP_REQUEST_ENTITY_TOO_LARGE)
            apr_table_setn(r->notes, "error-notes",
                          "TRACE with a request body is not allowed");
        return rv;
    }

    if (ap_should_client_block(r)) {

        if (r->remaining > 0) {
            if (r->remaining > 65536) {
                apr_table_setn(r->notes, "error-notes",
                       "Extended TRACE request bodies cannot exceed 64k\n");
                return HTTP_REQUEST_ENTITY_TOO_LARGE;
            }
             
            bodybuf = (apr_size_t)r->remaining + 32;
        }
        else {
             
            bodybuf = 73730;
        }

        bodyoff = bodyread = apr_palloc(r->pool, bodybuf);

         
        while ((!bodylen || bodybuf >= 32) &&
               (res = ap_get_client_block(r, bodyoff, bodybuf)) > 0) {
            bodylen += res;
            bodybuf -= res;
            bodyoff += res;
        }
        if (res > 0 && bodybuf < 32) {
             
            while (ap_get_client_block(r, bodyread, bodylen) > 0)
                ;
            apr_table_setn(r->notes, "error-notes",
                   "Extended TRACE request bodies cannot exceed 64k\n");
            return HTTP_REQUEST_ENTITY_TOO_LARGE;
        }

        if (res < 0) {
            return HTTP_BAD_REQUEST;
        }
    }

    ap_set_content_type(r, "message/http");

     

    bb = apr_brigade_create(r->pool, r->connection->bucket_alloc);
#if APR_CHARSET_EBCDIC
    {
        char *tmp;
        apr_size_t len;
        len = strlen(r->the_request);
        tmp = apr_pmemdup(r->pool, r->the_request, len);
        ap_xlate_proto_to_ascii(tmp, len);
        apr_brigade_putstrs(bb, NULL, NULL, tmp, CRLF_ASCII, NULL);
    }
#else
    apr_brigade_putstrs(bb, NULL, NULL, r->the_request, CRLF, NULL);
#endif
    h.pool = r->pool;
    h.bb = bb;
    apr_table_do((int (*) (void *, const char *, const char *))
                 form_header_field, (void *) &h, r->headers_in, NULL);
    apr_brigade_puts(bb, NULL, NULL, CRLF_ASCII);

     
    if (bodylen) {
        b = apr_bucket_pool_create(bodyread, bodylen,
                                   r->pool, bb->bucket_alloc);
        APR_BRIGADE_INSERT_TAIL(bb, b);
    }

    ap_pass_brigade(r->output_filters,  bb);

    return DONE;
}