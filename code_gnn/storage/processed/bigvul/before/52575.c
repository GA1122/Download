static void on_write_complete(h2o_socket_t *sock, int status)
{
    h2o_http2_conn_t *conn = sock->data;

    assert(conn->_write.buf_in_flight != NULL);

     
    if (status != 0) {
        close_connection_now(conn);
        return;
    }

     
    h2o_buffer_dispose(&conn->_write.buf_in_flight);
    assert(conn->_write.buf_in_flight == NULL);

     
    if (status == 0 && conn->state < H2O_HTTP2_CONN_STATE_IS_CLOSING) {
        while (!h2o_linklist_is_empty(&conn->_write.streams_to_proceed)) {
            h2o_http2_stream_t *stream =
                H2O_STRUCT_FROM_MEMBER(h2o_http2_stream_t, _refs.link, conn->_write.streams_to_proceed.next);
            assert(!h2o_http2_stream_has_pending_data(stream));
            h2o_linklist_unlink(&stream->_refs.link);
            h2o_http2_stream_proceed(conn, stream);
        }
    }

     
    if (h2o_timeout_is_linked(&conn->_write.timeout_entry))
        h2o_timeout_unlink(&conn->_write.timeout_entry);

     
    if (do_emit_writereq(conn))
        return;

     
    switch (conn->state) {
    case H2O_HTTP2_CONN_STATE_OPEN:
        break;
    case H2O_HTTP2_CONN_STATE_HALF_CLOSED:
        if (conn->num_streams.pull.half_closed + conn->num_streams.push.half_closed != 0)
            break;
        conn->state = H2O_HTTP2_CONN_STATE_IS_CLOSING;
     
    case H2O_HTTP2_CONN_STATE_IS_CLOSING:
        close_connection_now(conn);
        return;
    }

     
    if (conn->sock->input->size != 0)
        parse_input(conn);
}
