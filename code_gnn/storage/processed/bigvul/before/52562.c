static int handle_incoming_request(h2o_http2_conn_t *conn, h2o_http2_stream_t *stream, const uint8_t *src, size_t len,
                                   const char **err_desc)
{
    int ret, header_exists_map;

    assert(stream->state == H2O_HTTP2_STREAM_STATE_RECV_HEADERS);

    header_exists_map = 0;
    if ((ret = h2o_hpack_parse_headers(&stream->req, &conn->_input_header_table, src, len, &header_exists_map,
                                       &stream->_expected_content_length, err_desc)) != 0)
        return ret;

#define EXPECTED_MAP                                                                                                               \
    (H2O_HPACK_PARSE_HEADERS_METHOD_EXISTS | H2O_HPACK_PARSE_HEADERS_PATH_EXISTS | H2O_HPACK_PARSE_HEADERS_SCHEME_EXISTS)
    if ((header_exists_map & EXPECTED_MAP) != EXPECTED_MAP) {
        ret = H2O_HTTP2_ERROR_PROTOCOL;
        goto SendRSTStream;
    }
#undef EXPECTED_MAP

     
    if (conn->num_streams.pull.open > H2O_HTTP2_SETTINGS_HOST.max_concurrent_streams) {
        ret = H2O_HTTP2_ERROR_REFUSED_STREAM;
        goto SendRSTStream;
    }

    if (stream->_req_body == NULL) {
        execute_or_enqueue_request(conn, stream);
    } else {
        h2o_http2_stream_set_state(conn, stream, H2O_HTTP2_STREAM_STATE_RECV_BODY);
    }
    return 0;

SendRSTStream:
    send_stream_error(conn, stream->stream_id, ret);
    h2o_http2_stream_reset(conn, stream);
    return 0;
}