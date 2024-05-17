static void initiate_graceful_shutdown(h2o_context_t *ctx)
{
     
    h2o_linklist_t *node;

     
    if (ctx->http2._graceful_shutdown_timeout.cb != NULL)
        return;
    ctx->http2._graceful_shutdown_timeout.cb = graceful_shutdown_resend_goaway;

    for (node = ctx->http2._conns.next; node != &ctx->http2._conns; node = node->next) {
        h2o_http2_conn_t *conn = H2O_STRUCT_FROM_MEMBER(h2o_http2_conn_t, _conns, node);
        if (conn->state < H2O_HTTP2_CONN_STATE_HALF_CLOSED) {
            h2o_http2_encode_goaway_frame(&conn->_write.buf, INT32_MAX, H2O_HTTP2_ERROR_NONE,
                                          (h2o_iovec_t){H2O_STRLIT("graceful shutdown")});
            h2o_http2_conn_request_write(conn);
        }
    }
    h2o_timeout_link(ctx->loop, &ctx->one_sec_timeout, &ctx->http2._graceful_shutdown_timeout);
}
