AP_DECLARE(long) ap_get_client_block(request_rec *r, char *buffer,
                                     apr_size_t bufsiz)
{
    apr_status_t rv;
    apr_bucket_brigade *bb;

    if (r->remaining < 0 || (!r->read_chunked && r->remaining == 0)) {
        return 0;
    }

    bb = apr_brigade_create(r->pool, r->connection->bucket_alloc);
    if (bb == NULL) {
        r->connection->keepalive = AP_CONN_CLOSE;
        return -1;
    }

    rv = ap_get_brigade(r->input_filters, bb, AP_MODE_READBYTES,
                        APR_BLOCK_READ, bufsiz);

     
    if (rv == AP_FILTER_ERROR) {
         
        apr_brigade_destroy(bb);
        return -1;
    }
    if (rv != APR_SUCCESS) {
        apr_bucket *e;

         
        apr_brigade_cleanup(bb);

        e = ap_bucket_error_create(
                ap_map_http_request_error(rv, HTTP_BAD_REQUEST), NULL, r->pool,
                r->connection->bucket_alloc);
        APR_BRIGADE_INSERT_TAIL(bb, e);

        e = apr_bucket_eos_create(r->connection->bucket_alloc);
        APR_BRIGADE_INSERT_TAIL(bb, e);

        rv = ap_pass_brigade(r->output_filters, bb);
        if (APR_SUCCESS != rv) {
            ap_log_rerror(APLOG_MARK, APLOG_INFO, rv, r, APLOGNO(02484)
                          "Error while writing error response");
        }

         
        r->connection->keepalive = AP_CONN_CLOSE;
        apr_brigade_destroy(bb);
        return -1;
    }

     
    AP_DEBUG_ASSERT(!APR_BRIGADE_EMPTY(bb));

     
    if (APR_BUCKET_IS_EOS(APR_BRIGADE_LAST(bb))) {
        if (r->read_chunked) {
            r->remaining = -1;
        }
        else {
            r->remaining = 0;
        }
    }

    rv = apr_brigade_flatten(bb, buffer, &bufsiz);
    if (rv != APR_SUCCESS) {
        apr_brigade_destroy(bb);
        return -1;
    }

     
    r->read_length += bufsiz;

    apr_brigade_destroy(bb);
    return bufsiz;
}
