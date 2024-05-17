static apr_status_t fill_buffer(h2_stream *stream, apr_size_t amount)
{
    conn_rec *c = stream->session->c;
    apr_bucket *b;
    apr_status_t status;
    
    if (!stream->output) {
        return APR_EOF;
    }
    status = h2_beam_receive(stream->output, stream->out_buffer, 
                             APR_NONBLOCK_READ, amount);
    ap_log_cerror(APLOG_MARK, APLOG_TRACE2, status, stream->session->c,
                  "h2_stream(%ld-%d): beam_received",
                  stream->session->id, stream->id);
     
    for (b = APR_BRIGADE_FIRST(stream->out_buffer);
         b != APR_BRIGADE_SENTINEL(stream->out_buffer);
         b = APR_BUCKET_NEXT(b)) {
        if (APR_BUCKET_IS_FILE(b)) {
            apr_bucket_file *f = (apr_bucket_file *)b->data;
            apr_pool_t *fpool = apr_file_pool_get(f->fd);
            if (fpool != c->pool) {
                apr_bucket_setaside(b, c->pool);
                if (!stream->files) {
                    stream->files = apr_array_make(stream->pool, 
                                                   5, sizeof(apr_file_t*));
                }
                APR_ARRAY_PUSH(stream->files, apr_file_t*) = f->fd;
            }
        }
    }
    return status;
}
