h_conn_timeout(conn c)
{
    int r, should_timeout = 0;
    job j;

     
    if (conn_waiting(c) && conn_has_close_deadline(c)) should_timeout = 1;

     
    while ((j = soonest_job(c))) {
        if (j->deadline_at >= now_usec()) break;

         
        if (j == c->out_job) {
            c->out_job = job_copy(c->out_job);
        }

        timeout_ct++;  
        j->timeout_ct++;
        r = enqueue_job(remove_this_reserved_job(c, j), 0, 0);
        if (r < 1) bury_job(j, 0);  
        r = conn_update_evq(c, c->evq.ev_events);
        if (r == -1) return twarnx("conn_update_evq() failed"), conn_close(c);
    }

    if (should_timeout) {
        dprintf("conn_waiting(%p) = %d\n", c, conn_waiting(c));
        return reply_msg(remove_waiting_conn(c), MSG_DEADLINE_SOON);
    } else if (conn_waiting(c) && c->pending_timeout >= 0) {
        dprintf("conn_waiting(%p) = %d\n", c, conn_waiting(c));
        c->pending_timeout = -1;
        return reply_msg(remove_waiting_conn(c), MSG_TIMED_OUT);
    }
}
