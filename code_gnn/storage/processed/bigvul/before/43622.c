apr_status_t ap_http_outerror_filter(ap_filter_t *f,
                                     apr_bucket_brigade *b)
{
    request_rec *r = f->r;
    outerror_filter_ctx_t *ctx = (outerror_filter_ctx_t *)(f->ctx);
    apr_bucket *e;

     
    if (!ctx) {
        ctx = apr_pcalloc(r->pool, sizeof(outerror_filter_ctx_t));
        f->ctx = ctx;
    }
    for (e = APR_BRIGADE_FIRST(b);
         e != APR_BRIGADE_SENTINEL(b);
         e = APR_BUCKET_NEXT(e))
    {
        if (AP_BUCKET_IS_ERROR(e)) {
             
            if (((ap_bucket_error *)(e->data))->status == HTTP_BAD_GATEWAY ||
                ((ap_bucket_error *)(e->data))->status == HTTP_GATEWAY_TIME_OUT) {
                 
                r->connection->keepalive = AP_CONN_CLOSE;
            }
            continue;
        }
         
        if (AP_BUCKET_IS_EOC(e)) {
            ctx->seen_eoc = 1;
        }
    }
     
    if (ctx->seen_eoc) {
        for (e = APR_BRIGADE_FIRST(b);
             e != APR_BRIGADE_SENTINEL(b);
             e = APR_BUCKET_NEXT(e))
        {
            if (!APR_BUCKET_IS_METADATA(e)) {
                APR_BUCKET_REMOVE(e);
            }
        }
    }

    return ap_pass_brigade(f->next,  b);
}
