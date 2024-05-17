AP_CORE_DECLARE_NONSTD(apr_status_t) ap_http_header_filter(ap_filter_t *f,
                                                           apr_bucket_brigade *b)
{
    request_rec *r = f->r;
    conn_rec *c = r->connection;
    const char *protocol = NULL;
    apr_bucket *e;
    apr_bucket_brigade *b2;
    header_struct h;
    header_filter_ctx *ctx = f->ctx;
    const char *ctype;
    ap_bucket_error *eb = NULL;
    core_server_config *conf;

    AP_DEBUG_ASSERT(!r->main);

    if (r->header_only) {
        if (!ctx) {
            ctx = f->ctx = apr_pcalloc(r->pool, sizeof(header_filter_ctx));
        }
        else if (ctx->headers_sent) {
            apr_brigade_cleanup(b);
            return OK;
        }
    }

    for (e = APR_BRIGADE_FIRST(b);
         e != APR_BRIGADE_SENTINEL(b);
         e = APR_BUCKET_NEXT(e))
    {
        if (AP_BUCKET_IS_ERROR(e) && !eb) {
            eb = e->data;
            continue;
        }
         
        if (AP_BUCKET_IS_EOC(e)) {
            ap_remove_output_filter(f);
            return ap_pass_brigade(f->next, b);
        }
    }
    if (eb) {
        int status;

        status = eb->status;
        apr_brigade_cleanup(b);
        ap_die(status, r);
        return AP_FILTER_ERROR;
    }

    if (r->assbackwards) {
        r->sent_bodyct = 1;
        ap_remove_output_filter(f);
        return ap_pass_brigade(f->next, b);
    }

     
    if (!apr_is_empty_table(r->err_headers_out)) {
        r->headers_out = apr_table_overlay(r->pool, r->err_headers_out,
                                           r->headers_out);
    }

    conf = ap_get_core_module_config(r->server->module_config);
    if (conf->http_conformance & AP_HTTP_CONFORMANCE_STRICT) {
        int ok = check_headers(r);
        if (!ok && !(conf->http_conformance & AP_HTTP_CONFORMANCE_LOGONLY)) {
            ap_die(HTTP_INTERNAL_SERVER_ERROR, r);
            return AP_FILTER_ERROR;
        }
    }

     
    if (apr_table_get(r->subprocess_env, "force-no-vary") != NULL) {
        apr_table_unset(r->headers_out, "Vary");
        r->proto_num = HTTP_VERSION(1,0);
        apr_table_setn(r->subprocess_env, "force-response-1.0", "1");
    }
    else {
        fixup_vary(r);
    }

     
    if (apr_table_get(r->notes, "no-etag") != NULL) {
        apr_table_unset(r->headers_out, "ETag");
    }

     
    basic_http_header_check(r, &protocol);
    ap_set_keepalive(r);

    if (r->chunked) {
        apr_table_mergen(r->headers_out, "Transfer-Encoding", "chunked");
        apr_table_unset(r->headers_out, "Content-Length");
    }

    ctype = ap_make_content_type(r, r->content_type);
    if (ctype) {
        apr_table_setn(r->headers_out, "Content-Type", ctype);
    }

    if (r->content_encoding) {
        apr_table_setn(r->headers_out, "Content-Encoding",
                       r->content_encoding);
    }

    if (!apr_is_empty_array(r->content_languages)) {
        int i;
        char *token;
        char **languages = (char **)(r->content_languages->elts);
        const char *field = apr_table_get(r->headers_out, "Content-Language");

        while (field && (token = ap_get_list_item(r->pool, &field)) != NULL) {
            for (i = 0; i < r->content_languages->nelts; ++i) {
                if (!strcasecmp(token, languages[i]))
                    break;
            }
            if (i == r->content_languages->nelts) {
                *((char **) apr_array_push(r->content_languages)) = token;
            }
        }

        field = apr_array_pstrcat(r->pool, r->content_languages, ',');
        apr_table_setn(r->headers_out, "Content-Language", field);
    }

     
    if (r->no_cache && !apr_table_get(r->headers_out, "Expires")) {
        char *date = apr_palloc(r->pool, APR_RFC822_DATE_LEN);
        ap_recent_rfc822_date(date, r->request_time);
        apr_table_addn(r->headers_out, "Expires", date);
    }

    b2 = apr_brigade_create(r->pool, c->bucket_alloc);
    basic_http_header(r, b2, protocol);

    h.pool = r->pool;
    h.bb = b2;

    if (r->status == HTTP_NOT_MODIFIED) {
        apr_table_do((int (*)(void *, const char *, const char *)) form_header_field,
                     (void *) &h, r->headers_out,
                     "Connection",
                     "Keep-Alive",
                     "ETag",
                     "Content-Location",
                     "Expires",
                     "Cache-Control",
                     "Vary",
                     "Warning",
                     "WWW-Authenticate",
                     "Proxy-Authenticate",
                     "Set-Cookie",
                     "Set-Cookie2",
                     NULL);
    }
    else {
        send_all_header_fields(&h, r);
    }

    terminate_header(b2);

    ap_pass_brigade(f->next, b2);

    if (r->header_only) {
        apr_brigade_cleanup(b);
        ctx->headers_sent = 1;
        return OK;
    }

    r->sent_bodyct = 1;          

    if (r->chunked) {
         
        ap_add_output_filter("CHUNK", NULL, r, r->connection);
    }

     
    ap_remove_output_filter(f);
    return ap_pass_brigade(f->next, b);
}