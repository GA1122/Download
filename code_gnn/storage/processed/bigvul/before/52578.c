static void resume_send(h2o_http2_conn_t *conn)
{
    if (h2o_http2_conn_get_buffer_window(conn) <= 0)
        return;
#if 0  
    if (conn->scheduler.list.size == 0)
        return;
#endif
    request_gathered_write(conn);
}