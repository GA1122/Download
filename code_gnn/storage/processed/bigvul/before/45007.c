static ap_filter_t *insert_old_write_filter(request_rec *r)
{
    ap_filter_t *f;
    old_write_filter_ctx *ctx;

     

     
    for (f = r->output_filters; f != NULL; f = f->next) {
        if (ap_old_write_func == f->frec)
            break;
    }

    if (f == NULL) {
         
        ctx = apr_pcalloc(r->pool, sizeof(*ctx));
        ctx->tmpbb = apr_brigade_create(r->pool, r->connection->bucket_alloc);

        ap_add_output_filter("OLD_WRITE", ctx, r, r->connection);
        f = r->output_filters;
    }

    return f;
}
