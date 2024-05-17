static int on_begin_headers_cb(nghttp2_session *ngh2,
                               const nghttp2_frame *frame, void *userp)
{
    h2_session *session = (h2_session *)userp;
    h2_stream *s;
    
     
    (void)ngh2;
    s = get_stream(session, frame->hd.stream_id);
    if (s) {
         
    }
    else {
        s = h2_session_open_stream(userp, frame->hd.stream_id, 0, NULL);
    }
    return s? 0 : NGHTTP2_ERR_START_STREAM_NOT_ALLOWED;
}