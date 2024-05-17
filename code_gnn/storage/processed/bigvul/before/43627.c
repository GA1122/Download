static void basic_http_header(request_rec *r, apr_bucket_brigade *bb,
                              const char *protocol)
{
    char *date = NULL;
    const char *proxy_date = NULL;
    const char *server = NULL;
    const char *us = ap_get_server_banner();
    header_struct h;
    struct iovec vec[4];

    if (r->assbackwards) {
         
        return;
    }

     

    vec[0].iov_base = (void *)protocol;
    vec[0].iov_len  = strlen(protocol);
    vec[1].iov_base = (void *)" ";
    vec[1].iov_len  = sizeof(" ") - 1;
    vec[2].iov_base = (void *)(r->status_line);
    vec[2].iov_len  = strlen(r->status_line);
    vec[3].iov_base = (void *)CRLF;
    vec[3].iov_len  = sizeof(CRLF) - 1;
#if APR_CHARSET_EBCDIC
    {
        char *tmp;
        apr_size_t len;
        tmp = apr_pstrcatv(r->pool, vec, 4, &len);
        ap_xlate_proto_to_ascii(tmp, len);
        apr_brigade_write(bb, NULL, NULL, tmp, len);
    }
#else
    apr_brigade_writev(bb, NULL, NULL, vec, 4);
#endif

    h.pool = r->pool;
    h.bb = bb;

     
    if (r->proxyreq != PROXYREQ_NONE) {
        proxy_date = apr_table_get(r->headers_out, "Date");
        if (!proxy_date) {
             
            date = apr_palloc(r->pool, APR_RFC822_DATE_LEN);
            ap_recent_rfc822_date(date, r->request_time);
        }
        server = apr_table_get(r->headers_out, "Server");
    }
    else {
        date = apr_palloc(r->pool, APR_RFC822_DATE_LEN);
        ap_recent_rfc822_date(date, r->request_time);
    }

    form_header_field(&h, "Date", proxy_date ? proxy_date : date );

    if (!server && *us)
        server = us;
    if (server)
        form_header_field(&h, "Server", server);

    if (APLOGrtrace3(r)) {
        ap_log_rerror(APLOG_MARK, APLOG_TRACE3, 0, r,
                      "Response sent with status %d%s",
                      r->status,
                      APLOGrtrace4(r) ? ", headers:" : "");

         
        ap_log_rerror(APLOG_MARK, APLOG_TRACE5, 0, r, "  Date: %s",
                      proxy_date ? proxy_date : date );
        if (server)
            ap_log_rerror(APLOG_MARK, APLOG_TRACE5, 0, r, "  Server: %s",
                          server);
    }


     
    apr_table_unset(r->headers_out, "Date");         
    if (server) {
        apr_table_unset(r->headers_out, "Server");
    }
}