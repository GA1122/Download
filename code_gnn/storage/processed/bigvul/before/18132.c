do_stats(conn c, fmt_fn fmt, void *data)
{
    int r, stats_len;

     
    stats_len = fmt(NULL, 0, data) + 16;

    c->out_job = allocate_job(stats_len);  
    if (!c->out_job) return reply_serr(c, MSG_OUT_OF_MEMORY);

     
    c->out_job->state = JOB_STATE_COPY;

     
    r = fmt(c->out_job->body, stats_len, data);
     
    c->out_job->body_size = r;
    if (r > stats_len) return reply_serr(c, MSG_INTERNAL_ERROR);

    c->out_job_sent = 0;
    return reply_line(c, STATE_SENDJOB, "OK %d\r\n", r - 2);
}
