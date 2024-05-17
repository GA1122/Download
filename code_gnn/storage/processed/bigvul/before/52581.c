static void set_priority(h2o_http2_conn_t *conn, h2o_http2_stream_t *stream, const h2o_http2_priority_t *priority,
                         int scheduler_is_open)
{
    h2o_http2_scheduler_node_t *parent_sched;

     
    if (priority->dependency != 0) {
        h2o_http2_stream_t *parent_stream = h2o_http2_conn_get_stream(conn, priority->dependency);
        if (parent_stream != NULL) {
            parent_sched = &parent_stream->_refs.scheduler.node;
        } else {
             
            parent_sched = &conn->scheduler;
            priority = &h2o_http2_default_priority;
        }
    } else {
        parent_sched = &conn->scheduler;
    }

     
    if (!scheduler_is_open) {
        h2o_http2_scheduler_open(&stream->_refs.scheduler, parent_sched, priority->weight, priority->exclusive);
    } else {
        h2o_http2_scheduler_rebind(&stream->_refs.scheduler, parent_sched, priority->weight, priority->exclusive);
    }
}
