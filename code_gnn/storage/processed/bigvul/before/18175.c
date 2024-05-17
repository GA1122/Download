reset_conn(conn c)
{
    int r;

    r = conn_update_evq(c, EV_READ | EV_PERSIST);
    if (r == -1) return twarnx("update events failed"), conn_close(c);

     
    if (c->out_job && c->out_job->state == JOB_STATE_COPY) job_free(c->out_job);
    c->out_job = NULL;

    c->reply_sent = 0;  
    c->state = STATE_WANTCOMMAND;
}