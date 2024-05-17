 void send_stream_error(h2o_http2_conn_t *conn, uint32_t stream_id, int errnum)
{
    assert(stream_id != 0);
    assert(conn->state < H2O_HTTP2_CONN_STATE_IS_CLOSING);

    h2o_http2_encode_rst_stream_frame(&conn->_write.buf, stream_id, -errnum);
    h2o_http2_conn_request_write(conn);
}