static void push_path(h2o_req_t *src_req, const char *abspath, size_t abspath_len)
{
    h2o_http2_conn_t *conn = (void *)src_req->conn;
    h2o_http2_stream_t *src_stream = H2O_STRUCT_FROM_MEMBER(h2o_http2_stream_t, req, src_req);

    if (!conn->peer_settings.enable_push || conn->num_streams.push.open >= conn->peer_settings.max_concurrent_streams)
        return;
    if (conn->push_stream_ids.max_open >= 0x7ffffff0)
        return;
    if (!(h2o_linklist_is_empty(&conn->_pending_reqs) && can_run_requests(conn)))
        return;

     
    if (src_stream->req.hostconf->http2.casper.capacity_bits != 0 && !h2o_http2_stream_is_push(src_stream->stream_id)) {
        size_t header_index;
        switch (src_stream->pull.casper_state) {
        case H2O_HTTP2_STREAM_CASPER_STATE_TBD:
             
            if (h2o_find_header(&src_stream->req.headers, H2O_TOKEN_X_FORWARDED_FOR, -1) != -1) {
                src_stream->pull.casper_state = H2O_HTTP2_STREAM_CASPER_DISABLED;
                return;
            }
             
            if (conn->casper == NULL)
                h2o_http2_conn_init_casper(conn, src_stream->req.hostconf->http2.casper.capacity_bits);
             
            for (header_index = -1;
                 (header_index = h2o_find_header(&src_stream->req.headers, H2O_TOKEN_COOKIE, header_index)) != -1;) {
                h2o_header_t *header = src_stream->req.headers.entries + header_index;
                h2o_http2_casper_consume_cookie(conn->casper, header->value.base, header->value.len);
            }
            src_stream->pull.casper_state = H2O_HTTP2_STREAM_CASPER_READY;
        case H2O_HTTP2_STREAM_CASPER_READY:
            break;
        case H2O_HTTP2_STREAM_CASPER_DISABLED:
            return;
        }
    }

     
    conn->push_stream_ids.max_open += 2;
    h2o_http2_stream_t *stream = h2o_http2_stream_open(conn, conn->push_stream_ids.max_open, NULL);
    stream->push.parent_stream_id = src_stream->stream_id;
    h2o_http2_scheduler_open(&stream->_refs.scheduler, &src_stream->_refs.scheduler.node, 16, 0);
    h2o_http2_stream_prepare_for_request(conn, stream);

     
    stream->req.input.method = (h2o_iovec_t){H2O_STRLIT("GET")};
    stream->req.input.scheme = src_stream->req.input.scheme;
    stream->req.input.authority =
        h2o_strdup(&stream->req.pool, src_stream->req.input.authority.base, src_stream->req.input.authority.len);
    stream->req.input.path = h2o_strdup(&stream->req.pool, abspath, abspath_len);
    stream->req.version = 0x200;

    {  
        size_t i;
        for (i = 0; i != src_stream->req.headers.size; ++i) {
            h2o_header_t *src_header = src_stream->req.headers.entries + i;
            if (h2o_iovec_is_token(src_header->name)) {
                h2o_token_t *token = H2O_STRUCT_FROM_MEMBER(h2o_token_t, buf, src_header->name);
                if (token->copy_for_push_request) {
                    h2o_add_header(&stream->req.pool, &stream->req.headers, token,
                                   h2o_strdup(&stream->req.pool, src_header->value.base, src_header->value.len).base,
                                   src_header->value.len);
                }
            }
        }
    }

    execute_or_enqueue_request(conn, stream);

     
    if (!stream->push.promise_sent && stream->state != H2O_HTTP2_STREAM_STATE_END_STREAM)
        h2o_http2_stream_send_push_promise(conn, stream);
}