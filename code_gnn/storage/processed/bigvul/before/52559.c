static int handle_data_frame(h2o_http2_conn_t *conn, h2o_http2_frame_t *frame, const char **err_desc)
{
    h2o_http2_data_payload_t payload;
    h2o_http2_stream_t *stream;
    int ret;

    if ((ret = h2o_http2_decode_data_payload(&payload, frame, err_desc)) != 0)
        return ret;

    if (conn->state >= H2O_HTTP2_CONN_STATE_HALF_CLOSED)
        return 0;

    stream = h2o_http2_conn_get_stream(conn, frame->stream_id);

     
    if (stream == NULL) {
        if (frame->stream_id <= conn->pull_stream_ids.max_open) {
            send_stream_error(conn, frame->stream_id, H2O_HTTP2_ERROR_STREAM_CLOSED);
        } else {
            *err_desc = "invalid DATA frame";
            return H2O_HTTP2_ERROR_PROTOCOL;
        }
    } else if (stream->state != H2O_HTTP2_STREAM_STATE_RECV_BODY) {
        send_stream_error(conn, frame->stream_id, H2O_HTTP2_ERROR_STREAM_CLOSED);
        h2o_http2_stream_reset(conn, stream);
        stream = NULL;
    } else if (stream->_req_body->size + payload.length > conn->super.ctx->globalconf->max_request_entity_size) {
        send_stream_error(conn, frame->stream_id, H2O_HTTP2_ERROR_REFUSED_STREAM);
        h2o_http2_stream_reset(conn, stream);
        stream = NULL;
    } else {
        h2o_iovec_t buf = h2o_buffer_reserve(&stream->_req_body, payload.length);
        if (buf.base != NULL) {
            memcpy(buf.base, payload.data, payload.length);
            stream->_req_body->size += payload.length;
             
            if ((frame->flags & H2O_HTTP2_FRAME_FLAG_END_STREAM) != 0) {
                stream->req.entity = h2o_iovec_init(stream->_req_body->bytes, stream->_req_body->size);
                execute_or_enqueue_request(conn, stream);
                stream = NULL;  
            }
        } else {
             
            send_stream_error(conn, frame->stream_id, H2O_HTTP2_ERROR_STREAM_CLOSED);
            h2o_http2_stream_reset(conn, stream);
            stream = NULL;
        }
    }

     
    update_input_window(conn, 0, &conn->_input_window, frame->length);
    if (stream != NULL)
        update_input_window(conn, stream->stream_id, &stream->input_window, frame->length);

    return 0;
}