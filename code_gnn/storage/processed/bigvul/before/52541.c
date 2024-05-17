static h2o_http2_conn_t *create_conn(h2o_context_t *ctx, h2o_hostconf_t **hosts, h2o_socket_t *sock, struct timeval connected_at)
{
    static const h2o_conn_callbacks_t callbacks = {get_sockname, get_peername, push_path};
    h2o_http2_conn_t *conn = h2o_mem_alloc(sizeof(*conn));

     
    memset(conn, 0, sizeof(*conn));
    conn->super.ctx = ctx;
    conn->super.hosts = hosts;
    conn->super.connected_at = connected_at;
    conn->super.callbacks = &callbacks;
    conn->sock = sock;
    conn->peer_settings = H2O_HTTP2_SETTINGS_DEFAULT;
    conn->streams = kh_init(h2o_http2_stream_t);
    h2o_http2_scheduler_init(&conn->scheduler);
    conn->state = H2O_HTTP2_CONN_STATE_OPEN;
    h2o_linklist_insert(&ctx->http2._conns, &conn->_conns);
    conn->_read_expect = expect_preface;
    conn->_input_header_table.hpack_capacity = conn->_input_header_table.hpack_max_capacity =
        H2O_HTTP2_SETTINGS_DEFAULT.header_table_size;
    h2o_http2_window_init(&conn->_input_window, &H2O_HTTP2_SETTINGS_DEFAULT);
    conn->_output_header_table.hpack_capacity = H2O_HTTP2_SETTINGS_HOST.header_table_size;
    h2o_linklist_init_anchor(&conn->_pending_reqs);
    h2o_buffer_init(&conn->_write.buf, &wbuf_buffer_prototype);
    h2o_linklist_init_anchor(&conn->_write.streams_to_proceed);
    conn->_write.timeout_entry.cb = emit_writereq;
    h2o_http2_window_init(&conn->_write.window, &conn->peer_settings);

    return conn;
}