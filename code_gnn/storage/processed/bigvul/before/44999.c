AP_DECLARE(void) ap_send_interim_response(request_rec *r, int send_headers)
{
    hdr_ptr x;
    char *status_line = NULL;
    request_rec *rr;

    if (r->proto_num < HTTP_VERSION(1,1)) {
         
        return;
    }
    if (!ap_is_HTTP_INFO(r->status)) {
        ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, APLOGNO(00575)
                      "Status is %d - not sending interim response", r->status);
        return;
    }
    if ((r->status == HTTP_CONTINUE) && !r->expecting_100) {
         
        return;
    }

     
    for (rr = r; rr != NULL; rr = rr->main) {
        rr->expecting_100 = 0;
    }

    status_line = apr_pstrcat(r->pool, AP_SERVER_PROTOCOL, " ", r->status_line, CRLF, NULL);
    ap_xlate_proto_to_ascii(status_line, strlen(status_line));

    x.f = r->connection->output_filters;
    x.bb = apr_brigade_create(r->pool, r->connection->bucket_alloc);

    ap_fputs(x.f, x.bb, status_line);
    if (send_headers) {
        apr_table_do(send_header, &x, r->headers_out, NULL);
        apr_table_clear(r->headers_out);
    }
    ap_fputs(x.f, x.bb, CRLF_ASCII);
    ap_fflush(x.f, x.bb);
    apr_brigade_destroy(x.bb);
}
