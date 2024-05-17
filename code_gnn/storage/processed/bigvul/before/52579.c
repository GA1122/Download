static void run_pending_requests(h2o_http2_conn_t *conn)
{
    while (!h2o_linklist_is_empty(&conn->_pending_reqs) && can_run_requests(conn)) {
         
        h2o_http2_stream_t *stream = H2O_STRUCT_FROM_MEMBER(h2o_http2_stream_t, _refs.link, conn->_pending_reqs.next);
        h2o_linklist_unlink(&stream->_refs.link);
         
        h2o_http2_stream_set_state(conn, stream, H2O_HTTP2_STREAM_STATE_SEND_HEADERS);
        if (!h2o_http2_stream_is_push(stream->stream_id) && conn->pull_stream_ids.max_processed < stream->stream_id)
            conn->pull_stream_ids.max_processed = stream->stream_id;
        h2o_process_request(&stream->req);
    }
}